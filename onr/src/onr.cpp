/*
 * Video Sync code for onr research
 *
 * Created on : 07-01-2012
 *     Author : Xichen Shi
 *       Rev  : 11-25-2012
 */

#include <iostream>
#include <inttypes.h>
#include <sys/time.h>
#include <cstdio>
#include <cstring>
#include <opencv2/opencv.hpp>
#include <cv.h>

#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#include <mavlink/mavlink_types.h>
#include <mavlink/ardupilotmega/mavlink.h>

using namespace std;
using namespace cv;
/////////////////////////////////////////////////////////////////////
/////////////////////////// Data Log ////////////////////////////////
/////////////////////////////////////////////////////////////////////
FILE *logIMU;
FILE *logGPS;
FILE *logFrame;

/////////////////////////////////////////////////////////////////////
///////////////////////// System Status /////////////////////////////
/////////////////////////////////////////////////////////////////////
bool alive = true;
uint8_t buf[MAVLINK_MAX_PACKET_LEN];
static unsigned long startTime = 0;

/////////////////////////////////////////////////////////////////////
////////////////////////// Video Src ////////////////////////////////
/////////////////////////////////////////////////////////////////////
VideoCapture videoSrc0(0);
const char *window1 = "Video";
int frameCount = 0;
   
/////////////////////////////////////////////////////////////////////
//////////////////////// Serial Port ////////////////////////////////
/////////////////////////////////////////////////////////////////////
const char *serialName = "/dev/ttyS0";
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
    imshow(window1, frame);
    
    frameCount++;
    char imgName[14], imgPath[30];
    sprintf(imgName, "%010d.jpg", frameCount);
    sprintf(imgPath, "../test_data/%s",imgName);
    fprintf(logFrame,"%10d,%010d.jpg\n" ,microSecond(),frameCount);
    imwrite(imgPath, frame);
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
		        printf("%12d,%10d,%10d,%10d,%10d,%10d,%10d,\n",microSecond(),raw_imu.xacc,raw_imu.yacc,raw_imu.zacc,raw_imu.xgyro,raw_imu.ygyro,raw_imu.zgyro);
		        break;		
		        
	        case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
	            mavlink_global_position_int_t gps;
	            mavlink_msg_global_position_int_decode(&msg, &gps);
	            fprintf(logGPS, "%12d,%12d,%12d,%10d,%10d,%12d,%12d,%12d,%12d,\n",microSecond(), gps.lat, gps.lon, gps.lat, gps.relative_alt, gps.vx, gps.vy, gps.vz, gps.hdg);
	            //printf("%12d,%12d,%12d,%10d,%10d,%12d,%12d,%12d,%12d,\n",microSecond(, gps.lat, gps.lon, gps.lat, gps.relative_alt, gps.vx, gps.vy, gps.vz, gps.hdg);
	            break;
	           
            case MAVLINK_MSG_ID_HEARTBEAT:
                mavlink_heartbeat_t heartbeat;
                mavlink_msg_heartbeat_decode(&msg, &heartbeat);
                //printf("mavlink heartbeat received \n");
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
        char c = waitKey(33);
        if (c==27)
        {
            pthread_mutex_lock( &mutex1 );
            alive=false;
            pthread_mutex_unlock( &mutex1 );
        }
    }
}


void *runThread2(void*)
{
    while(alive)
    {
        handleMessage();
    }
}
      
int main()
{   
    // start system timer
    startTime = microSecond();
    
    // Serial through termios
    memset(&serialConfig, 0, sizeof(serialConfig));
    setSerialPort(&serialConfig, Serial);
    
    // Video Capture
    namedWindow(window1,CV_WINDOW_AUTOSIZE);
    
    // Check port status
    if (Serial != -1) {
        cout << "Serial port "<< serialName <<" is openned now" <<endl;
    }
    else {
        cout << "Serial port "<< serialName <<" is NOT available" <<endl;
        return -1;
    }
    
    // Check capture device status
    if (!videoSrc0.isOpened()) {
	    cout << "capture device failed to open!" << endl;
        return -1;
    }

    // serial_port reading and writing
    int count = 0;
    logIMU = fopen("../test_data/IMUdata", "w");
    logGPS = fopen("../test_data/GPSdata", "w");
    logFrame = fopen("../test_data/Framedata","w");
    fprintf(logIMU, "     time(us)   xacc(mg)   yacc(mg)   zacc(mg)      xgyro      ygyro      zgyro\n");
    fprintf(logGPS, "     time(us)  lat(deg*e7)  lon(deg*e7)    alt(mm)    agl(mm)   vx(m/s*e2)   vy(m/s*e2)   vz(m/s*e2)  hdg(deg*e2)\n");
    //printf("     time(us)   xacc(mg)   yacc(mg)   zacc(mg)      xgyro      ygyro      zgyro\n");   
    // Mainloop
    pthread_t thread1, thread2;

	pthread_create( &thread1, NULL, runThread1, NULL);
	pthread_create( &thread2, NULL, runThread2, NULL);
    pthread_join( thread1, NULL );
    pthread_join( thread2, NULL );

    return 0;
}
