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

/**
 * @brief initialise vision system
 */
void initVisionsys(){
    last_time_visionsys = getMilliSeconds();
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
        if (getMilliSeconds() - last_time_visionsys > 1000 && g_system_status.navigation_mode == NAV_MODE_SEMIAUTO){
            start_track = true;
            cvNamedWindow("TLD",CV_WINDOW_AUTOSIZE);
            cvSetMouseCallback( "TLD", mouseHandler, NULL );

            GETBOUNDINGBOX:
            while(!gotBB)
            {
                visionModule._capture_device >> frame;
                drawBox(frame,box);
                imshow("TLD", frame);
        
                if (cvWaitKey(33) == 'q') return 0;
            }
            if (min(box.width,box.height)<(int)visionModule._parameter_file.getFirstTopLevelNode()["min_win"]){
                cout << "Bounding box too small, try again." << endl;
                gotBB = false;
                goto GETBOUNDINGBOX;
            }
            //Remove callback
            cvSetMouseCallback( "TLD", NULL, NULL );

            visionModule.readyTracking(box);
        }

        // Loop to update TLD tracking
        while(start_track && g_system_status.navigation_mode == NAV_MODE_SEMIAUTO){
            visionModule.updateTracking();
            if (visionModule._track_status) {
                cout << "drawing box" << endl;
                drawPoints(visionModule._current_color_frame,visionModule._previous_features);
                drawPoints(visionModule._current_color_frame,visionModule._current_features,Scalar(0,255,0));
                drawBox(visionModule._current_color_frame,visionModule._target_box);
            }
            imshow("TLD",frame);
            if (cvWaitKey(33) == 'q') break;
        }
    }
}

/***************************************** TEMP **********************************************/
/**
 * @brief initialise vision system
 */
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