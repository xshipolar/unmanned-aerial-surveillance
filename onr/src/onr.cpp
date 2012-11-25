/*
 * Video Sync code for onr research
 *
 * Created on : 07-01-2012
 *     Author : Xichen Shi
 *       Rev  : 10-22-2012
 */

//#include <boost/asio.hpp>
//#include <boost/bind.hpp>
//#include <boost/date_time/posix_time/posix_time.hpp>
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

#include "../include/mavlink/mavlink_types.h"
#include "../include/mavlink/ardupilotmega/mavlink.h"

using namespace std;
using namespace cv;

/** Variables **/
const char *serialName = "/dev/ttyS0";
const char *window1 = "Video";
uint8_t buf[MAVLINK_MAX_PACKET_LEN];

/** Functions **/
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

void captureVideo(VideoCapture vidsrc, const char *windowName)
{
    Mat frame;
    vidsrc>>frame;
    imshow(windowName, frame);
}

void logData(FILE *fid, int port)
{
    static unsigned char c;
    mavlink_message_t msg;
    mavlink_status_t status;
    if(read(port, &c, 1)>0) {
        if (mavlink_parse_char(0, c, &msg, &status)){
            switch (msg.msgid) {
            case MAVLINK_MSG_ID_RAW_IMU:
                mavlink_raw_imu_t raw_imu;
                mavlink_msg_raw_imu_decode(&msg, &raw_imu);
	            fprintf(fid, "%12d,%10d,%10d,%10d,%10d,%10d,%10d\n",raw_imu.time_usec,raw_imu.xacc,raw_imu.yacc,raw_imu.zacc,raw_imu.xgyro,raw_imu.ygyro,raw_imu.zgyro);
		        printf("%12d,%10d,%10d,%10d,%10d,%10d,%10d\n",raw_imu.time_usec,raw_imu.xacc,raw_imu.yacc,raw_imu.zacc,raw_imu.xgyro,raw_imu.ygyro,raw_imu.zgyro);
		        break;		
		        
	        case MAVLINK_MSG_ID_ATTITUDE:
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

int main()
{   
    // Serial through termios
    termios serialConfig;
    memset(&serialConfig, 0, sizeof(serialConfig));
    int Serial;
    
    setSerialPort(&serialConfig, Serial);
    
    // Video Capture
    VideoCapture capture(0);
    cvNamedWindow(window1,CV_WINDOW_AUTOSIZE);
    
    // Check port status
    if (Serial != -1) {
        cout << "Serial port "<< serialName <<" is openned now" <<endl;
    }
    else {
        cout << "Serial port "<< serialName <<" is NOT available" <<endl;
        return -1;
    }
    
    // Check capture device status
    if (!capture.isOpened()) {
	    cout << "capture device failed to open!" << endl;
        return 1;
    }

    // serial_port reading and writing
    int count = 0;
    FILE *log;
    log = fopen("testdata", "w");
    fprintf(log, "     time(us)   xacc(mg)   yacc(mg)   zacc(mg)      xgyro      ygyro      zgyro\n");
    printf("     time(us)   xacc(mg)   yacc(mg)   zacc(mg)      xgyro      ygyro      zgyro\n");
    
    // Mainloop
    for(;;)
    {           
        // Capture image
        //captureVideo(capture, window1);
        /*if(cvWaitKey(10) == 'q')
        {
            fclose(log);
            return 0;
        }*/
        
        // Comm
        logData(log, Serial);
        
	    /** increase main loop count **/
	    count++;
    }
    return 0;
}
