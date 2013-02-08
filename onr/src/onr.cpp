/* Video Sync code for onr research
 *
 * Created on : 07-01-2012
 *     Author : Xichen Shi
 *       Rev  : 11-25-2012
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

#include <mavlink/mavlink_types.h>
#include <mavlink/ardupilotmega/mavlink.h>

using namespace std;
using namespace cv;
/////////////////////////////////////////////////////////////////////
///////////////////////// System Condition //////////////////////////
/////////////////////////////////////////////////////////////////////
#define ENABLED              1
#define DISABLED            -1
#define DISPLAY             DISABLED

/////////////////////////////////////////////////////////////////////
/////////////////////////// Data Log ////////////////////////////////
/////////////////////////////////////////////////////////////////////
FILE *logIMU;
FILE *logGPS;
FILE *logFrame;
FILE *logSO;
FILE *logATT;
FILE *logDRIFT;
FILE *logALT;
string mainPath = "../test_data/";

/////////////////////////////////////////////////////////////////////
///////////////////////// System Status /////////////////////////////
/////////////////////////////////////////////////////////////////////
bool alive = true;
bool active = false;
uint8_t buf[MAVLINK_MAX_PACKET_LEN];
static unsigned long startTime = 0;
static unsigned long lastActiveTime = 0;
static unsigned long lastSendTime = 0;

/////////////////////////////////////////////////////////////////////
////////////////////////// Video Src ////////////////////////////////
/////////////////////////////////////////////////////////////////////
VideoCapture videoSrc0(0);
const char *window1 = "Video";
int frameCount = 0;
   
/////////////////////////////////////////////////////////////////////
//////////////////////// Serial Port ////////////////////////////////
/////////////////////////////////////////////////////////////////////
const char *serialName = "/dev/ttyACM0";
termios serialConfig;
int Serial;

/////////////////////////////////////////////////////////////////////
////////////////////////// Functions ////////////////////////////////
/////////////////////////////////////////////////////////////////////
void setSerialPort(termios *serialIO, int& tid)
{
    serialIO->c_iflag = 0;
    serialIO->c_oflag = 0;
    serialIO->c_cflag = CS8|CREAD|CLOCAL;
    serialIO->c_lflag = 0;
    serialIO->c_cc[VMIN] = 0;
    serialIO->c_cc[VTIME]= 0;
    
    tid = open(serialName, O_RDWR | O_NONBLOCK);
    cfsetospeed(serialIO, B115200);
    cfsetispeed(serialIO, B115200);
    tcsetattr(tid, TCSANOW, serialIO);
}

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
    imshow(window1, frame);
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

void sendMessage(int msgID)
{
    mavlink_message_t msg;
    switch (msgID) {
    case MAVLINK_MSG_ID_HEARTBEAT:
        mavlink_msg_heartbeat_pack(255, 0, &msg, 0,MAV_AUTOPILOT_ARDUPILOTMEGA, 0,0,0);
        mavlink_msg_to_send_buffer(buf, &msg); 
        write(Serial, &buf, sizeof(buf));
        break;
    case MAVLINK_MSG_ID_REQUEST_DATA_STREAM:
        mavlink_msg_request_data_stream_pack(255,0,&msg, 1, 1, MAV_DATA_STREAM_RAW_SENSORS, 100, 1);
        mavlink_msg_to_send_buffer(buf, &msg);
        write(Serial, &buf, sizeof(buf));
        
        //mavlink_msg_request_data_stream_pack(255,0,&msg, 1, 1, MAV_DATA_STREAM_POSITION, 5, 1);
        //mavlink_msg_to_send_buffer(buf, &msg);
        //write(Serial, &buf, sizeof(buf));
        
        mavlink_msg_request_data_stream_pack(255,0,&msg, 1, 1, MAV_DATA_STREAM_EXTENDED_STATUS, 5, 1);
        mavlink_msg_to_send_buffer(buf, &msg);
        write(Serial, &buf, sizeof(buf));
        
        mavlink_msg_request_data_stream_pack(255,0,&msg, 1, 1, MAV_DATA_STREAM_EXTRA1, 5, 1);
        mavlink_msg_to_send_buffer(buf, &msg);
        write(Serial, &buf, sizeof(buf));
        
        mavlink_msg_request_data_stream_pack(255,0,&msg, 1, 1, MAV_DATA_STREAM_EXTRA2, 20, 1);
        mavlink_msg_to_send_buffer(buf, &msg);
        write(Serial, &buf, sizeof(buf));
        
        mavlink_msg_request_data_stream_pack(255,0,&msg, 1, 1, MAV_DATA_STREAM_EXTRA3, 20, 1);
        mavlink_msg_to_send_buffer(buf, &msg);
        write(Serial, &buf, sizeof(buf));
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
    if(read(Serial, &c, 1)>0) {
        if (mavlink_parse_char(0, c, &msg, &status)){
            switch (msg.msgid) {
            case MAVLINK_MSG_ID_RAW_IMU:
                mavlink_raw_imu_t raw_imu;
                mavlink_msg_raw_imu_decode(&msg, &raw_imu);
	            fprintf(logIMU, "%12d,%10d,%10d,%10d,%10d,%10d,%10d,\n",microSecond(),raw_imu.xacc,raw_imu.yacc,raw_imu.zacc,raw_imu.xgyro,raw_imu.ygyro,raw_imu.zgyro);
		        //printf("%12d,%10d,%10d,%10d,%10d,%10d,%10d,\n",microSecond(),raw_imu.xacc,raw_imu.yacc,raw_imu.zacc,raw_imu.xgyro,raw_imu.ygyro,raw_imu.zgyro);
		        break;		
		        
	        case MAVLINK_MSG_ID_GPS_RAW_INT:
	            mavlink_gps_raw_int_t gps;
	            mavlink_msg_gps_raw_int_decode(&msg, &gps);
	            fprintf(logGPS, "%12d,%12d,%12d,%12d,%12d,%12d,%12d,%12d,%12d,%12d,\n",microSecond(), gps.lat, gps.lon, gps.alt, 
	                    gps.eph, gps.epv, gps.vel, gps.cog, gps.fix_type, gps.satellites_visible);
	            //printf("%12d,%12d,%12d,%12d,%12d,%12d,%12d,%12d,%12d,%12d,\n",microSecond(), gps.lat, gps.lon, gps.alt, 
	            //        gps.eph, gps.epv, gps.vel, gps.cog, gps.fix_type, gps.satellites_visible);
	            break;
	           
	        case MAVLINK_MSG_ID_SENSOR_OFFSETS:
	            mavlink_sensor_offsets_t sensor_offsets;
	            mavlink_msg_sensor_offsets_decode(&msg, &sensor_offsets);
	            fprintf(logSO, "%12d,%12f,%12f,%12f,%12f,%12f,%12f,\n",microSecond(),sensor_offsets.accel_cal_x, sensor_offsets.accel_cal_y, sensor_offsets.accel_cal_z, sensor_offsets.gyro_cal_x, sensor_offsets.gyro_cal_y, sensor_offsets.gyro_cal_z);
	            break;
	            
	        case MAVLINK_MSG_ID_ATTITUDE:
	            mavlink_attitude_t attitude;
	            mavlink_msg_attitude_decode(&msg, &attitude);
	            fprintf(logATT, "%12d,%12f,%12f,%12f,%12f,%12f,%12f,\n",microSecond(),attitude.roll, attitude.pitch, attitude.yaw, attitude.rollspeed, attitude.pitchspeed, attitude.yawspeed);
	            break;
	        
	        case MAVLINK_MSG_ID_AHRS:
	            mavlink_ahrs_t ahrs;
	            mavlink_msg_ahrs_decode(&msg, &ahrs);
	            fprintf(logDRIFT, "%12d,%12f,%12f,%12f,\n",microSecond(), ahrs.omegaIx, ahrs.omegaIy, ahrs.omegaIz);
	            break;
	            
	        case MAVLINK_MSG_ID_VFR_HUD:
	            mavlink_vfr_hud_t vfr;
	            mavlink_msg_vfr_hud_decode(&msg, &vfr);
	            fprintf(logALT, "%12d,%12f,\n",microSecond(), vfr.alt);
	            break;
	            
            case MAVLINK_MSG_ID_HEARTBEAT:
                mavlink_heartbeat_t heartbeat;
                mavlink_msg_heartbeat_decode(&msg, &heartbeat);
                if (heartbeat.system_status == MAV_STATE_ACTIVE){
                    active = true;
                    lastActiveTime = microSecond();
                    printf("APM is active \n");
                } else if (heartbeat.system_status == MAV_STATE_CALIBRATING) {
                    printf("APM is initialising \n");
                    active = false;
                }
                break;
            
            default:
	            break;
	        }   
        }
    }
}

void *runThread1(void*)
{
	pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
    while(alive)
    {
        captureVideo();
#if DISPLAY == ENABLED
        char c = waitKey(33);
        if (c==27)
        {
            pthread_mutex_lock( &mutex1 );
            alive=false;
            pthread_mutex_unlock( &mutex1 );
        }
#endif
    }
}


void *runThread2(void*)
{
    while(alive)
    {
        handleMessage();
        if (microSecond() - lastActiveTime > 1500000) active = false;
    }
}

void *runThread3(void*)
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
    
    // Serial through termios
    memset(&serialConfig, 0, sizeof(serialConfig));
    setSerialPort(&serialConfig, Serial);
    
    // Video Display
#if DISPLAY == ENABLED
    namedWindow(window1,CV_WINDOW_AUTOSIZE);
#endif
    
    // Check port status
    if (Serial != -1) {
        cout << "Serial port "<< serialName <<" is openned now" <<endl;
    }
    else {
        cout << "Serial port "<< serialName <<" is NOT available" <<endl;
        //return -1;
    }
    
    // Check capture device status
    videoSrc0.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    videoSrc0.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
    if (!videoSrc0.isOpened()) {
	    cout << "capture device failed to open!" << endl;
        return -1;
    }

    // create dir and files
    mainPath += numTime;
    cout<<"saving files to "<<mainPath<<endl;
    string imuPath, gpsPath, framePath, imgPath, offsetPath, attPath, driftPath, altPath;
    imgPath   = mainPath+"/img";
    imuPath   = mainPath+"/imu";
    gpsPath   = mainPath+"/location";
    offsetPath= mainPath+"/sensor_offsets";
    attPath   = mainPath+"/attitude";
    driftPath = mainPath+"/drift";
    altPath   = mainPath+"/alt";
    framePath = mainPath+"/framedata";
    string dir_cmd1 = "mkdir "+ mainPath;
    string dir_cmd2 = "mkdir "+ imgPath;
    // creating paths
    system(dir_cmd1.c_str());
    system(dir_cmd2.c_str());
    // openning files
    logIMU   = fopen(imuPath.c_str(), "w");
    logGPS   = fopen(gpsPath.c_str(), "w");
    logFrame = fopen(framePath.c_str(),"w");
    logSO    = fopen(offsetPath.c_str(),"w");
    logATT   = fopen(attPath.c_str(),"w");
    logDRIFT = fopen(driftPath.c_str(),"w");
    logALT   = fopen(altPath.c_str(),"w");
    fprintf(logIMU,  "     time(us)   xacc(mg)   yacc(mg)   zacc(mg)      xgyro      ygyro      zgyro\n");
    fprintf(logGPS,  "     time(us)  lat(deg*e7)  lon(deg*e7)      alt(mm)      eph(cm)      epv(cm)      vel(cm/s)  cog(deg*e2)    fix_type    satellites_visible\n");
    fprintf(logSO,   "     time(us)     cal_xacc     cal_yacc     cal_zacc    cal_xgyro    cal_ygyro    cal_zgyro\n");
    fprintf(logATT,  "     time(us)    roll(rad)   pitch(rad)     yaw(rad)      rollspd     pitchspd       yawspd (rad/s)\n");
    fprintf(logDRIFT,"     time(us) x_drift(r/s) y_drift(r/s) z_drift(r/s)\n");
    fprintf(logALT,  "     time(us)  altitude(m)\n");

    // Mainloop
    pthread_t thread1, thread2, thread3;

	pthread_create( &thread1, NULL, runThread1, NULL);
	pthread_create( &thread2, NULL, runThread2, NULL);
	pthread_create( &thread3, NULL, runThread3, NULL);
    pthread_join( thread1, NULL );
    pthread_join( thread2, NULL );
    pthread_join( thread3, NULL );

    return 0;
}
