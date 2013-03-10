/*
 * navsys.cpp
 *
 *  Created on: Mar 06, 2013
 *      Author: Shubham Gupta
 */

#include <navsys.hpp>

void* runNavsys(void*)
{
    cout <<"Gimbal angles>>\n";
    cout<<"\tTilt\tPan\n";
    //tracking_started=true;
    while(true)
    {
        if(tracking_started==true){
            centroid c=find_centroid();
            gimbal_state_t g=gimbal_calculate(c.x, c.y, 320, 240, g_current_gimbal_state.tilt_angle, g_current_gimbal_state.pan_angle);
            setGimbalState(g.pan_angle, g.tilt_angle, 0.0);
        }
    }
}