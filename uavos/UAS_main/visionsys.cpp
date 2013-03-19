/*
 * visionsys.cpp
 *
 *  Created on: Mar 05, 2013
 *      Author: Xichen Shi
 */
#include <visionsys.hpp>

static UAS_tld visionModule;
static unsigned long last_time_visionsys;
// Temp vars
static bool start_track;
Rect box;
Mat frame;
bool gotBB = false;
bool drawing_box = false;
int lot = 0;
bool tracking_started = false;

/**
 * @brief initialise vision system
 */
void initVisionsys(){
    last_time_visionsys = getMilliSeconds();
    //g_centroid.x=0;
    //g_centroid.y=0;
    visionModule.init();
}


/**
 * @brief 
 */
void* runVisionsys(void*){
    // Exit thread when module not properly initialised
    // TODO: be able to try restart a module
    if(visionModule.isInitialised()==false){
        exit(0);    
    }
    
    // Main loop
    while(true){
        if (getMilliSeconds() - last_time_visionsys > 1000 && g_system_status.navigation_mode >= NAV_MODE_SEMIAUTO){
            start_track = true;
            cvNamedWindow("TLD",CV_WINDOW_AUTOSIZE);
            cvSetMouseCallback( "TLD", mouseHandler, NULL );

            GETBOUNDINGBOX:
            while(!gotBB)
            {
                visionModule.capture_device >> frame;
                drawBox(frame,box);
                imshow("TLD", frame);
                if ((char)waitKey(10) == 'q') {
                    destroyAllWindows();
                    return 0;
                }
            }
            if (min(box.width,box.height)<(int)visionModule.parameter_file.getFirstTopLevelNode()["min_win"]){
                cout << "Bounding box too small, try again." << endl;
                gotBB = false;
                goto GETBOUNDINGBOX;
            }
            //Remove callback
            cvSetMouseCallback( "TLD", NULL, NULL );

            visionModule.readyTracking(box);
        }
        time_t t1=time(0);
        // Loop to update TLD tracking
        while(start_track && g_system_status.navigation_mode >= NAV_MODE_SEMIAUTO){
            tracking_started=true;
            visionModule.updateTracking();
            if (visionModule._track_status) {
                drawBox(visionModule._current_color_frame,visionModule.target_box);
            }
            time_t t2=time(0);
            lot++;
            if(difftime(t2,t1)>1)
            {
                double fr=(double)lot/(double)difftime(t2,t1);
                //cout << "Frame Rate : "<< fr <<"\n";
                lot=0;
                t1=t2;
            }
            imshow("TLD",visionModule._current_color_frame);
            if ((char)waitKey(10) == 'q') {
                g_system_status.navigation_mode = NAV_MODE_IDLE; // change to IDLE mode
                start_track = false;
                destroyAllWindows();
                cout << "Changed System State to IDLE" <<endl;
                break;
            }
        }
    }
}

/***************************************** TEMP **********************************************/
/**
 * @brief initialise vision system
 */

centroid find_centroid()
{
    centroid c;
    c.x=visionModule.target_box.x+(visionModule.target_box.width)/2;
    c.y=visionModule.target_box.y+(visionModule.target_box.height)/2;
    tracking_started=true;
    return c;
}

void mouseHandler(int event, int x, int y, int flags, void *param){
    switch( event ){
    case CV_EVENT_MOUSEMOVE:
    if (drawing_box){
        box.width = x-box.x;
        box.height = y-box.y;
    }
    break;
    case CV_EVENT_LBUTTONDOWN:
    drawing_box = true;
    box = Rect( x, y, 0, 0 );
    break;
    case CV_EVENT_LBUTTONUP:
    drawing_box = false;
    if( box.width < 0 ){
        box.x += box.width;
        box.width *= -1;
    }
    if( box.height < 0 ){
        box.y += box.height;
        box.height *= -1;
    }
    gotBB = true;
    break;
  }
}