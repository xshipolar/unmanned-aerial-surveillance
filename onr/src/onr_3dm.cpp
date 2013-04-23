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
string mainPath = "../test_data/";

/////////////////////////////////////////////////////////////////////
///////////////////////// System Status /////////////////////////////
/////////////////////////////////////////////////////////////////////
bool alive = true;
bool active = true;
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
////////////////////////// Functions ////////////////////////////////
///////////////////////////////////////////////////////////////////// 
unsigned long microSecond()
{
    timeval currentTime;
    gettimeofday(&currentTime, NULL);
    unsigned long usec = currentTime.tv_sec * 1000000 + currentTime.tv_usec - startTime;
    return usec;
}

void captureVideo()
{
    Mat frame;
    videoSrc0>>frame;
#if DISPLAY == ENABLED
    char textOverlay1[100];
    char textOverlay2[100];
    char textOverlay3[100];
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
    Mat frame_to_show;
    frame.copyTo(frame_to_show);
    CvScalar yellow = CV_RGB(255, 255, 0);
    CvScalar navy   = CV_RGB(0, 0, 128);
    CvScalar black  = CV_RGB(0, 0, 0);
    CvScalar white  = CV_RGB(255, 255, 255);
    putText(frame_to_show, textOverlay1, Point(25, 25),  FONT_HERSHEY_SIMPLEX, 0.5, navy, 2);
    putText(frame_to_show, textOverlay2, Point(180, 50), FONT_HERSHEY_SIMPLEX, 0.5, navy, 2);
    putText(frame_to_show, textOverlay3, Point(180, 75), FONT_HERSHEY_SIMPLEX, 0.5, navy, 2);
    imshow(window1, frame_to_show);
#endif    
    frameCount++;
    char imgName[14];
    sprintf(imgName, "%010d.jpg", frameCount);
    string imgPath = mainPath + "/img/" + imgName;
    if (active) {
        fprintf(logFrame,"%10d,%010d\n" ,microSecond(),frameCount);
        imwrite(imgPath, frame);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                  Print Packet Statistics                                      //
///////////////////////////////////////////////////////////////////////////////////////////////////
void print_packet_stats() {
    printf("%u AHRS (%u timeout, %u checksum error)\n", ahrs_valid_packet_count, ahrs_timeout_packet_count, ahrs_checksum_error_packet_count);
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
        mip_interface_update(&ahrs_gx3_25);
        usleep(100);
    }
}

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
    printf("Initializing AHRS.....\n");
    while (mip_interface_init(0, 115200, &ahrs_gx3_25, DEFAULT_PACKET_TIMEOUT_MS) != MIP_INTERFACE_OK) {
        printf("Failed to initialize AHRS, Retrying..... \n");
        return -1;
    } 
    printf("AHRS port: open\n");

    // printf("Resetting AHRS.....\n");
    // while(mip_base_cmd_reset_device(&ahrs_gx3_25) != MIP_INTERFACE_OK){
    //     printf("Failed, Retrying.....\n");
    // } // resetting AHRS device
    // printf("AHRS: reset\n");

    printf("Idling AHRS.....\n");
    while(mip_base_cmd_idle(&ahrs_gx3_25) != MIP_INTERFACE_OK){
        printf("Failed, Retrying.....\n");
    } // idling AHRS device
    printf("AHRS: idle\n");

    // Set callback
    if(mip_interface_add_descriptor_set_callback(&ahrs_gx3_25, MIP_AHRS_DATA_SET, NULL, &ahrs_packet_callback) != MIP_INTERFACE_OK) {
        printf("Failed to register callback\n");
        return -1;
    } else {
        printf("Registered callback function succesfully\n");
    }

    // Setup AHRS
    u8 enable = 1;
    u16 ahrs_rate = 0;
        while(mip_3dm_cmd_get_ahrs_base_rate(&ahrs_gx3_25, &ahrs_rate) != MIP_INTERFACE_OK){
        printf("Failed!! Retrying.....\n");
    }
    printf("Rate of AHRS message is: %d\n", ahrs_rate);

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
    printf("\n\nSetting the AHRS datastream format....\n\n");
    while(mip_3dm_cmd_ahrs_message_format(&ahrs_gx3_25, MIP_FUNCTION_SELECTOR_WRITE, &data_stream_format_num_entries, 
                                       data_stream_format_descriptors, data_stream_format_decimation) != MIP_INTERFACE_OK) {
        printf("Failed!! Retrying.....\n");
    }
    printf("AHRS datastream format: set\n");

    printf("Enabling continuous data stream.....\n");
    while(mip_3dm_cmd_continuous_data_stream(&ahrs_gx3_25, MIP_FUNCTION_SELECTOR_WRITE, MIP_3DM_AHRS_DATASTREAM, &enable) != MIP_INTERFACE_OK){
        printf("Failed!! Retrying.....\n");
    }
    printf("Continuous data stream : enabled\n");
    
    // Video Display
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
    string accPath, gyroPath, framePath, imgPath, attPath;
    imgPath   = mainPath+"/img";
    accPath   = mainPath+"/acc";
    gyroPath  = mainPath+"/gyro";
    attPath   = mainPath+"/attitude";
    framePath = mainPath+"/framedata";
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
    fprintf(logACC,  "     time(us)    xacc(g)    yacc(g)    zacc(g)\n");
    fprintf(logGYRO, "     time(us)    xgyro      ygyro      zgyro\n");
    fprintf(logATT,  "     time(us)    roll(rad)   pitch(rad)     yaw(rad)\n");
    // Mainloop
    pthread_t thread1, thread2, thread3;

    pthread_create( &thread1, NULL, runThread1, NULL);
    pthread_create( &thread2, NULL, runThread2, NULL);
    pthread_join( thread1, NULL );
    pthread_join( thread2, NULL );

    return 0;
}