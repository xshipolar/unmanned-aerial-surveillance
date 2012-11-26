/*
 * Video Sync code for onr research
 *
 * Created on : 07-01-2012
 *     Author : Xichen Shi
 *       Rev  : 11-25-2012
 */

#include <iostream>
#include <inttypes.h>
#include <ctime>
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
///////////////////////// System Status /////////////////////////////
/////////////////////////////////////////////////////////////////////
bool alive = true;
uint8_t buf[MAVLINK_MAX_PACKET_LEN];

/////////////////////////////////////////////////////////////////////
////////////////////////// Video Src ////////////////////////////////
/////////////////////////////////////////////////////////////////////
VideoCapture videoSrc0(0);
const char *window1 = "Video";
   
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

void captureVideo()
{
    Mat frame;
    videoSrc0>>frame;
    imshow(window1, frame);
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
	            //fprintf(fid, "%12d,%10d,%10d,%10d,%10d,%10d,%10d\n",raw_imu.time_usec,raw_imu.xacc,raw_imu.yacc,raw_imu.zacc,raw_imu.xgyro,raw_imu.ygyro,raw_imu.zgyro);
		        //printf("%12d,%10d,%10d,%10d,%10d,%10d,%10d\n",raw_imu.time_usec,raw_imu.xacc,raw_imu.yacc,raw_imu.zacc,raw_imu.xgyro,raw_imu.ygyro,raw_imu.zgyro);
		        break;		
		        
	        case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
	            mavlink_global_position_int_t gps;
	            mavlink_msg_global_position_int_decode(&msg, &gps);
	            printf("%10d,%10d,%10d,%10d,%10d,%10d,%10d,%10d,%10d\n",gps.time_boot_ms, gps.lat, gps.lon, gps.lat, gps.relative_alt, gps.vx, gps.vy, gps.vz, gps.hdg);
	            break;
	           
            case MAVLINK_MSG_ID_HEARTBEAT:
                //mavlink_heartbeat_t heartbeat;
                //mavlink_msg_heartbeat_decode(&msg, &heartbeat);
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
    while(alive)
    {
        captureVideo();
        char c = waitKey(33);
        if (c==27) alive=false;
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
    //FILE *log;
    //log = fopen("testdata", "w");
    //fprintf(log, "     time(us)   xacc(mg)   yacc(mg)   zacc(mg)      xgyro      ygyro      zgyro\n");
    printf("     time(us)   xacc(mg)   yacc(mg)   zacc(mg)      xgyro      ygyro      zgyro\n");
    
    // Mainloop
    pthread_t thread1, thread2;

	pthread_create( &thread1, NULL, runThread1, NULL);
	pthread_create( &thread2, NULL, runThread2, NULL);
    pthread_join( thread1, NULL );
    pthread_join( thread2, NULL );

    return 0;
}
