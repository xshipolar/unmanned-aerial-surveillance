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
///////////////////////// System Condition //////////////////////////
/////////////////////////////////////////////////////////////////////
#define ENABLED              1
#define DISABLED            -1
#define DISPLAY             ENABLED

/////////////////////////////////////////////////////////////////////
/////////////////////////// Data Log ////////////////////////////////
/////////////////////////////////////////////////////////////////////
FILE *logIMU;
FILE *logGPS;
FILE *logFrame;
string mainPath = "../test_data/";

/////////////////////////////////////////////////////////////////////
///////////////////////// System Status /////////////////////////////
/////////////////////////////////////////////////////////////////////
bool alive = true;
bool active = false;
uint8_t buf[MAVLINK_MAX_PACKET_LEN];
static unsigned long startTime = 0;
static unsigned long lastActiveTime = 0;

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
		        
	        case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
	            mavlink_global_position_int_t gps;
	            mavlink_msg_global_position_int_decode(&msg, &gps);
	            fprintf(logGPS, "%12d,%12d,%12d,%10d,%10d,%12d,%12d,%12d,%12d,\n",microSecond(), gps.lat, gps.lon, gps.lat, gps.relative_alt, gps.vx, gps.vy, gps.vz, gps.hdg);
	            //printf("%12d,%12d,%12d,%10d,%10d,%12d,%12d,%12d,%12d,\n",microSecond(, gps.lat, gps.lon, gps.lat, gps.relative_alt, gps.vx, gps.vy, gps.vz, gps.hdg);
	            break;
	           
            case MAVLINK_MSG_ID_HEARTBEAT:
                mavlink_heartbeat_t heartbeat;
                mavlink_msg_heartbeat_decode(&msg, &heartbeat);
                active = true;
                lastActiveTime = microSecond();
                printf("mavlink heartbeat received: system is active \n");
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
        return -1;
    }
    
    // Check capture device status
    if (!videoSrc0.isOpened()) {
	    cout << "capture device failed to open!" << endl;
        return -1;
    }

    // create dir and files
    mainPath += numTime;
    cout<<"saving files to "<<mainPath<<endl;
    string imuPath, gpsPath, framePath, imgPath;
    imgPath   = mainPath+"/img";
    imuPath   = mainPath+"/IMUdata";
    gpsPath   = mainPath+"/GPSdata";
    framePath = mainPath+"/Framedata";
    string dir_cmd1 = "mkdir "+ mainPath;
    string dir_cmd2 = "mkdir "+ imgPath;
    // creating paths
    system(dir_cmd1.c_str());
    system(dir_cmd2.c_str());
    // openning files
    logIMU   = fopen(imuPath.c_str(), "w");
    logGPS   = fopen(gpsPath.c_str(), "w");
    logFrame = fopen(framePath.c_str(),"w");
    fprintf(logIMU, "     time(us)   xacc(mg)   yacc(mg)   zacc(mg)      xgyro      ygyro      zgyro\n");
    fprintf(logGPS, "     time(us)  lat(deg*e7)  lon(deg*e7)    alt(mm)    agl(mm)   vx(m/s*e2)   vy(m/s*e2)   vz(m/s*e2)  hdg(deg*e2)\n");
 
    // Mainloop
    pthread_t thread1, thread2;

	pthread_create( &thread1, NULL, runThread1, NULL);
	pthread_create( &thread2, NULL, runThread2, NULL);
    pthread_join( thread1, NULL );
    pthread_join( thread2, NULL );

    return 0;
}
