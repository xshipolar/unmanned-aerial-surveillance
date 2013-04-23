/* Video Sync code for onr research
 *
 * Created on : 03-19-2013
 *     Author : Xichen Shi
 *       Rev  : 
 */

#include <iostream>
#include <inttypes.h>
#include <sys/time.h>
#include <ctime>
#include <cstdio>
#include <cstring>
#include <opencv2/opencv.hpp>

#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

extern "C" {
    #include <mip_sdk.h>
}

#include "comsys.hpp"
#include <mavlink/mavlink_types.h>
#include <mavlink/ardupilotmega/mavlink.h>
#include "UAS_serial.hpp"

using namespace std;
using namespace cv;
/////////////////////////////////////////////////////////////////////
///////////////////////// System Condition //////////////////////////
/////////////////////////////////////////////////////////////////////
#define ENABLED              1
#define DISABLED            -1
#define DISPLAY             ENABLED

pthread_mutex_t mutex_alive = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_log   = PTHREAD_MUTEX_INITIALIZER;

/////////////////////////////////////////////////////////////////////
/////////////////////////// Data Log ////////////////////////////////
/////////////////////////////////////////////////////////////////////
FILE *logACC;
FILE *logGYRO;
FILE *logFrame;
FILE *logATT;
FILE *logALT;
string mainPath = "../test_data/";

/////////////////////////////////////////////////////////////////////
///////////////////////// System Status /////////////////////////////
/////////////////////////////////////////////////////////////////////
bool alive = true;
bool active = false;
bool logging = false;
uint8_t buf[MAVLINK_MAX_PACKET_LEN];
static unsigned long startTime = 0;
static unsigned long lastActiveTime = 0;
static unsigned long lastSendTime = 0;

/////////////////////////////////////////////////////////////////////
/////////////////////// MicroStrain AHRS ////////////////////////////
/////////////////////////////////////////////////////////////////////
mip_interface ahrs_gx3_25;
//Packet Counters (valid, timeout, and checksum errors)
u32 ahrs_valid_packet_count = 0;
u32 ahrs_timeout_packet_count = 0;
u32 ahrs_checksum_error_packet_count = 0;
//AHRS
mip_ahrs_scaled_gyro  curr_ahrs_gyro={};
mip_ahrs_scaled_accel curr_ahrs_accel={};
mip_ahrs_euler_angles curr_ahrs_euler={};

#define DEFAULT_PACKET_TIMEOUT_MS  1000 //milliseconds

/////////////////////////////////////////////////////////////////////
////////////////////////// Video Src ////////////////////////////////
/////////////////////////////////////////////////////////////////////
VideoCapture videoSrc0(0);
const char *window1 = "Video";
int frameCount = 0;

/////////////////////////////////////////////////////////////////////
/////////////////////////////// APM /////////////////////////////////
/////////////////////////////////////////////////////////////////////
UAS_serial apm_link;
float curr_alt = 0;

/////////////////////////////////////////////////////////////////////
////////////////////////// Functions ////////////////////////////////
///////////////////////////////////////////////////////////////////// 
unsigned long microSecond()
{
    timeval currentTime;
    gettimeofday(&currentTime, NULL);
    unsigned long usec = currentTime.tv_sec * 1000000 + currentTime.tv_usec - startTime;
    return usec;
}

static inline void comm_send_ch(mavlink_channel_t chan, uint8_t ch)
{
    if (chan == MAVLINK_COMM_0)
    {
        apm_link.send(&ch,1);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                  Scanning for specific Device                                 //
///////////////////////////////////////////////////////////////////////////////////////////////////
char* scandev(const char* search_name){
  
  FILE *instream;
  char devnames[255][255];//allows for up to 256 devices with path links up to 255 characters long each
  int devct=0; //counter for number of devices
  int i=0;
  int j=0;
  int userchoice=0;
  char* device;

  char command[50];
  sprintf(command,"find /dev/serial -print | grep -i %s",search_name);//search /dev/serial for devices
  
  printf("Searching for devices containing %s...\n",search_name);

  instream=popen(command, "r");//execute piped command in read mode

  if(!instream){//SOMETHING WRONG WITH THE SYSTEM COMMAND PIPE...EXITING
    printf("ERROR BROKEN PIPELINE %s\n", command);
    return device;
  }

  for(i=0;i<255&&(fgets(devnames[i],sizeof(devnames[i]), instream));i++){//load char array of device addresses
    ++devct;
  }

  for(i=0;i<devct;i++){
    for(j=0;j<sizeof(devnames[i]);j++){
      if(devnames[i][j]=='\n'){
        devnames[i][j]='\0';//replaces newline inserted by pipe reader with char array terminator character 
        break;//breaks loop after replacement
      }
    }
    printf("Device Found:\n%d: %s\n\n\n",i,devnames[i]);
  }

  //CHOOSE DEVICE TO CONNECT TO AND CONNECT TO IT (IF THERE ARE CONNECTED DEVICES)

  if(devct>0){
    printf("Number of devices = %d\n", devct);
    if(devct>1){
      printf("Please choose the number of the device to connect to (0 to %i):\n",devct-1);
        while(scanf("%i",&userchoice)==0||userchoice<0||userchoice>devct-1){//check that there's input and in the correct range
          printf("Invalid choice...Please choose again between 0 and %d:\n", devct-1);
          getchar();//clear carriage return from keyboard buffer after invalid choice
        }
    }
    device=devnames[userchoice];
    return device;

  }
  else{
    printf("No %s devices found.\n",search_name);
    return device; 
  }

}

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                  Capture/Record/Display Video                                 //
///////////////////////////////////////////////////////////////////////////////////////////////////
void captureVideo()
{
    Mat frame;
    videoSrc0>>frame;
#if DISPLAY == ENABLED
    char textOverlay1[100];
    char textOverlay2[100];
    char textOverlay3[100];
    char textOverlay4[50];
    char textOverlay5[10];
    sprintf(textOverlay1,"Time: %10d, Roll: %8.5f, Pitch: %8.5f, Yaw:  %8.5f",
            microSecond(), 
            curr_ahrs_euler.roll, 
            curr_ahrs_euler.pitch, 
            curr_ahrs_euler.yaw);
    sprintf(textOverlay2,"xacc: %8.5f, yacc:  %8.5f, zacc: %8.5f",
            curr_ahrs_accel.scaled_accel[0], 
            curr_ahrs_accel.scaled_accel[1], 
            curr_ahrs_accel.scaled_accel[2]);
    sprintf(textOverlay3,"xgyro: %8.5f, ygyro:  %8.5f, zgyro: %8.5f",
            curr_ahrs_gyro.scaled_gyro[0], 
            curr_ahrs_gyro.scaled_gyro[1], 
            curr_ahrs_gyro.scaled_gyro[2]);
    sprintf(textOverlay4,"Altitude: %4.2f",curr_alt);
    Mat frame_to_show;
    frame.copyTo(frame_to_show);
    CvScalar yellow = CV_RGB(255, 255, 0);
    CvScalar navy   = CV_RGB(0, 0, 128);
    CvScalar black  = CV_RGB(0, 0, 0);
    CvScalar white  = CV_RGB(255, 255, 255);
    putText(frame_to_show, textOverlay1, Point(25, 25),   FONT_HERSHEY_SIMPLEX, 0.5, navy, 2);
    putText(frame_to_show, textOverlay2, Point(180, 50),  FONT_HERSHEY_SIMPLEX, 0.5, navy, 2);
    putText(frame_to_show, textOverlay3, Point(180, 75),  FONT_HERSHEY_SIMPLEX, 0.5, navy, 2);
    putText(frame_to_show, textOverlay4, Point(180, 100), FONT_HERSHEY_SIMPLEX, 0.5, navy, 2);
    if(logging){
        sprintf(textOverlay4,"! Logging !");
        putText(frame_to_show, textOverlay4, Point(25, 75), FONT_HERSHEY_SIMPLEX, 0.8, yellow, 2);
    }
    imshow(window1, frame_to_show);
#endif    
    frameCount++;
    char imgName[14];
    sprintf(imgName, "%010d.jpg", frameCount);
    string imgPath = mainPath + "/img/" + imgName;
    if (logging) {
        fprintf(logFrame,"%10d,%010d\n" ,microSecond(),frameCount);
        imwrite(imgPath, frame);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                  APM Message Handling                                         //
///////////////////////////////////////////////////////////////////////////////////////////////////
void sendMessage(int msgID)
{
    mavlink_message_t msg;
    uint16_t len;
    switch (msgID) {
    case MAVLINK_MSG_ID_HEARTBEAT:
        mavlink_msg_heartbeat_send(MAVLINK_COMM_0, 0, MAV_AUTOPILOT_ARDUPILOTMEGA, 0,0,0);
        break;
    case MAVLINK_MSG_ID_REQUEST_DATA_STREAM:
        mavlink_msg_request_data_stream_pack(255,0,&msg, 1, 1, MAV_DATA_STREAM_EXTRA2, 50, 1);
        len = mavlink_msg_to_send_buffer(buf, &msg);
        if(apm_link.send(&buf,len)==len){
            printf("Extra2 stream request sent.\n");
        }
        break;
    default:
        break;
    }
}

void handleMessage()
{
    static unsigned char c;
    mavlink_message_t msg;
    mavlink_status_t status;
    while(apm_link.available()>0) {
        apm_link.fetch(&c,1);
        if (mavlink_parse_char(0, c, &msg, &status)){
            switch (msg.msgid) {
            case MAVLINK_MSG_ID_VFR_HUD:
                mavlink_vfr_hud_t vfr;
                mavlink_msg_vfr_hud_decode(&msg, &vfr);
                curr_alt = vfr.alt;
                //printf("%12d,%12f,\n",microSecond(), vfr.alt);
                logging = true;
                fprintf(logALT, "%12d,%12f,\n",microSecond(), vfr.alt);
                break;
                
            case MAVLINK_MSG_ID_HEARTBEAT:
                mavlink_heartbeat_t heartbeat;
                mavlink_msg_heartbeat_decode(&msg, &heartbeat);
                if (heartbeat.system_status == MAV_STATE_ACTIVE){
                    active = true;
                    lastActiveTime = microSecond();
                    //printf("APM is active \n");
                } else if (heartbeat.system_status == MAV_STATE_CALIBRATING) {
                    //printf("APM is initialising \n");
                    active = false;
                }
                break;
            
            default:
                break;
            }   
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                  Print Packet Statistics                                      //
///////////////////////////////////////////////////////////////////////////////////////////////////
void print_packet_stats() {
    // printf("%u AHRS (%u timeout, %u checksum error)\n", ahrs_valid_packet_count, ahrs_timeout_packet_count, ahrs_checksum_error_packet_count);
    // printf("%12d, %10.5f,%10.5f,%10.5f,%10.5f,%10.5f,%10.5f\n", microSecond(), curr_ahrs_accel.scaled_accel[0], curr_ahrs_accel.scaled_accel[1], curr_ahrs_accel.scaled_accel[2],
    //                                                       curr_ahrs_gyro.scaled_gyro[0], curr_ahrs_gyro.scaled_gyro[1], curr_ahrs_gyro.scaled_gyro[2]);
    // printf("%12d,%10.5f,%10.5f,%10.5f\n", microSecond(), curr_ahrs_euler.roll, curr_ahrs_euler.pitch, curr_ahrs_euler.yaw);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                  AHRS Packet Callback                                         //
///////////////////////////////////////////////////////////////////////////////////////////////////
void ahrs_packet_callback(void *user_ptr, u8 *packet, u16 packet_size, u8 callback_type) {
    mip_field_header *field_header;
    u8               *field_data;
    u16              field_offset = 0;

    //The packet callback can have several types, process them all
    switch(callback_type) {
    case MIP_INTERFACE_CALLBACK_VALID_PACKET:
        ahrs_valid_packet_count++;
        while(mip_get_next_field(packet, &field_header, &field_data, &field_offset) == MIP_OK) {
            switch(field_header->descriptor) {

            case MIP_AHRS_DATA_ACCEL_SCALED:
                memcpy(&curr_ahrs_accel, field_data, sizeof(mip_ahrs_scaled_accel));
                mip_ahrs_scaled_accel_byteswap(&curr_ahrs_accel);
                fprintf(logACC, "%12d,%10.5f,%10.5f,%10.5f\n", 
                        microSecond(), 
                        curr_ahrs_accel.scaled_accel[0], 
                        curr_ahrs_accel.scaled_accel[1], 
                        curr_ahrs_accel.scaled_accel[2]);
            break;

            case MIP_AHRS_DATA_GYRO_SCALED:
                memcpy(&curr_ahrs_gyro, field_data, sizeof(mip_ahrs_scaled_gyro));
                mip_ahrs_scaled_gyro_byteswap(&curr_ahrs_gyro);
                fprintf(logGYRO, "%12d,%10.5f,%10.5f,%10.5f\n", 
                        microSecond(),     
                        curr_ahrs_gyro.scaled_gyro[0], 
                        curr_ahrs_gyro.scaled_gyro[1], 
                        curr_ahrs_gyro.scaled_gyro[2]);
            break;

            case MIP_AHRS_DATA_EULER_ANGLES:
                memcpy(&curr_ahrs_euler, field_data, sizeof(mip_ahrs_euler_angles));
                mip_ahrs_euler_angles_byteswap(&curr_ahrs_euler);
                fprintf(logATT, "%12d,%10.5f,%10.5f,%10.5f\n", 
                        microSecond(), 
                        curr_ahrs_euler.roll, 
                        curr_ahrs_euler.pitch, 
                        curr_ahrs_euler.yaw);
            default: break;
            }
        } 
    break;

    case MIP_INTERFACE_CALLBACK_CHECKSUM_ERROR:
        ahrs_checksum_error_packet_count++;
    break;

    case MIP_INTERFACE_CALLBACK_TIMEOUT:
        ahrs_timeout_packet_count++;
    break;

    default: 
    break;
    }
    print_packet_stats();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                  Video Capture Thread                                         //
///////////////////////////////////////////////////////////////////////////////////////////////////
void *runThread1(void*)
{
    printf("Video Thread Starting.....\n");
    while(alive)
    {
        captureVideo();
#if DISPLAY == ENABLED
        char c = waitKey(33);
        if (c==27)
        {
            pthread_mutex_lock( &mutex_alive );
            alive=false;
            mip_base_cmd_idle(&ahrs_gx3_25); // idling AHRS device
            pthread_mutex_unlock( &mutex_alive );
        }
#endif
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                  AHRS Handling Thread                                         //
///////////////////////////////////////////////////////////////////////////////////////////////////
void *runThread2(void*)
{
    printf("AHRS Thread Starting.....\n");
    while(alive)
    {
        if(logging) mip_interface_update(&ahrs_gx3_25);
        usleep(100);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                  APM Handling Thread                                          //
///////////////////////////////////////////////////////////////////////////////////////////////////
void *runThread3(void*)
{
    while(alive)
    {
        handleMessage();
        if (microSecond() - lastActiveTime > 1500000) active = false;
    }
}

void *runThread4(void*)
{
    bool setStream = true;
    while(alive)
    {
        if (active && setStream) {
            sendMessage(MAVLINK_MSG_ID_REQUEST_DATA_STREAM);
            setStream = false;
        }
        if (microSecond() - lastSendTime > 1000000) {
            sendMessage(MAVLINK_MSG_ID_HEARTBEAT);
            lastSendTime = microSecond();
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                  Time Conversion                                              //
///////////////////////////////////////////////////////////////////////////////////////////////////
string getNumTime(time_t *rawTime)
{
    string stringTime = ctime(rawTime);
    string date, year, month, time;
    for (int i=0;i<24;i++){
        switch (i) {
        case 4:
        case 5:
        case 6:
            month += stringTime[i];
            break;
        case 8:
        case 9:
            if (stringTime[i] == ' ') stringTime[i] = '0';
            date += stringTime[i];
            break;
        case 11:
        case 12:
        case 14:
        case 15:
            time += stringTime[i];
            break;
        case 20:
        case 21:
        case 22:
        case 23:
            year += stringTime[i];
            break;
        default:
            break;
        }
    }
    string numTime_t = month + date + year + time;
    return numTime_t;
}     

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                               //
//                                            Main                                               //
//                                                                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{   
    // start system timer
    startTime = microSecond();
    
    // Get current data and time
    time_t rawTime;
    time(&rawTime);
    string numTime;
    numTime = getNumTime(&rawTime);
    printf ( "The current local time is: %s \n", ctime(&rawTime) );
    
    // Initialize AHRS
    char* gx3_dev_name;
    // char a='\0';
    // gx3_dev_name = &a;
    gx3_dev_name = scandev("microstrain");
    if(strcmp(gx3_dev_name,"")==0){
        printf("Failed to find attached microstrain device.\n");
        return -1;
    }

    printf("Initializing AHRS.....\n");
    while (mip_interface_init(gx3_dev_name, 115200, &ahrs_gx3_25, DEFAULT_PACKET_TIMEOUT_MS) != MIP_INTERFACE_OK) {
        printf("Failed to initialize AHRS, Retrying..... \n");
        return -1;
    } 
    printf("AHRS port: open\n\n");

    // printf("Resetting AHRS.....\n");
    // while(mip_base_cmd_reset_device(&ahrs_gx3_25) != MIP_INTERFACE_OK){
    //     printf("Failed, Retrying.....\n");
    // } // resetting AHRS device
    // printf("AHRS: reset\n");

    printf("Idling AHRS.....\n");
    while(mip_base_cmd_idle(&ahrs_gx3_25) != MIP_INTERFACE_OK){
        printf("Failed, Retrying.....\n");
    } // idling AHRS device
    printf("AHRS: idle\n\n");

    // Set callback
    if(mip_interface_add_descriptor_set_callback(&ahrs_gx3_25, MIP_AHRS_DATA_SET, NULL, &ahrs_packet_callback) != MIP_INTERFACE_OK) {
        printf("Failed to register callback\n");
        return -1;
    } else {
        printf("Registered callback function succesfully\n\n");
    }

    // Setup AHRS
    u8 enable = 1;
    u16 ahrs_rate = 0;
        while(mip_3dm_cmd_get_ahrs_base_rate(&ahrs_gx3_25, &ahrs_rate) != MIP_INTERFACE_OK){
        printf("Failed!! Retrying.....\n");
    }
    printf("Rate of AHRS message is: %d\n\n", ahrs_rate);

    // Setup the AHRS message format and verify via read-back
    u8  data_stream_format_descriptors[10] = {0};
    u16 data_stream_format_decimation[10]  = {0};
    u8  data_stream_format_num_entries     =  0;

    data_stream_format_descriptors[0] = MIP_AHRS_DATA_ACCEL_SCALED; 
    data_stream_format_descriptors[1] = MIP_AHRS_DATA_GYRO_SCALED; 
    data_stream_format_descriptors[2] = MIP_AHRS_DATA_EULER_ANGLES;

    data_stream_format_decimation[0]  = 0x02; 
    data_stream_format_decimation[1]  = 0x02; 
    data_stream_format_decimation[2]  = 0x02; 

    data_stream_format_num_entries = 3;
 
    //Set the message format
    printf("Setting the AHRS datastream format....");
    while(mip_3dm_cmd_ahrs_message_format(&ahrs_gx3_25, MIP_FUNCTION_SELECTOR_WRITE, &data_stream_format_num_entries, 
                                       data_stream_format_descriptors, data_stream_format_decimation) != MIP_INTERFACE_OK) {
        printf("Failed!! Retrying.....\n");
    }
    printf("AHRS datastream format: set\n\n");

    printf("Enabling continuous data stream.....\n");
    while(mip_3dm_cmd_continuous_data_stream(&ahrs_gx3_25, MIP_FUNCTION_SELECTOR_WRITE, MIP_3DM_AHRS_DATASTREAM, &enable) != MIP_INTERFACE_OK){
        printf("Failed!! Retrying.....\n");
    }
    printf("Continuous data stream : enabled\n\n");



    /***************************** APM Initialize ************************************************/
    char* apm_dev_name;
    // char a='\0';
    // apm_dev_name = &a;
    apm_dev_name = scandev("arduino_mega");
    if(strcmp(apm_dev_name,"")==0){
        printf("Failed to find attached APM device.\n");
        return -1;
    }
    apm_link.beginPort(apm_dev_name,115200);


    /***************************** Video Initialize **********************************************/
#if DISPLAY == ENABLED
    namedWindow(window1,CV_WINDOW_AUTOSIZE);
#endif
    
    // Check capture device status
    videoSrc0.set(CV_CAP_PROP_FRAME_WIDTH, 1024);
    videoSrc0.set(CV_CAP_PROP_FRAME_HEIGHT, 768);
    if (!videoSrc0.isOpened()) {
        printf("capture device failed to open!\n");
        return -1;
    } else {
        printf("capture device : open\n\n\n");
    }

    // create dir and files
    mainPath += numTime;
    cout<<"saving files to "<<mainPath<<endl;
    string accPath, gyroPath, framePath, imgPath, attPath, altPath;
    imgPath   = mainPath+"/img";
    accPath   = mainPath+"/acc";
    gyroPath  = mainPath+"/gyro";
    attPath   = mainPath+"/attitude";
    framePath = mainPath+"/framedata";
    altPath   = mainPath+"/alt";
    string dir_cmd1 = "mkdir "+ mainPath;
    string dir_cmd2 = "mkdir "+ imgPath;
    // creating paths
    system(dir_cmd1.c_str());
    system(dir_cmd2.c_str());
    // openning files
    logACC   = fopen(accPath.c_str(), "w");
    logGYRO  = fopen(gyroPath.c_str(), "w");
    logFrame = fopen(framePath.c_str(),"w");
    logATT   = fopen(attPath.c_str(),"w");
    logALT   = fopen(altPath.c_str(),"w");
    fprintf(logACC,  "     time(us)    xacc(g)    yacc(g)    zacc(g)\n");
    fprintf(logGYRO, "     time(us)    xgyro      ygyro      zgyro\n");
    fprintf(logATT,  "     time(us)    roll(rad)   pitch(rad)     yaw(rad)\n");
    fprintf(logALT,  "     time(us)  altitude(m)\n");
    // Mainloop
    pthread_t thread1, thread2, thread3, thread4;

    pthread_create( &thread1, NULL, runThread1, NULL);
    pthread_create( &thread2, NULL, runThread2, NULL);
    pthread_create( &thread3, NULL, runThread3, NULL);
    pthread_create( &thread4, NULL, runThread4, NULL);
    pthread_join( thread1, NULL );
    pthread_join( thread2, NULL );
    pthread_join( thread3, NULL );
    pthread_join( thread4, NULL );

    return 0;
}