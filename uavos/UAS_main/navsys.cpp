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
            //cout<<"inside loop\n";
            centroid c=find_centroid();
            gimbal_state_t g=gimbal_calculate(c.x, c.y, 320, 240, g_current_gimbal_state.tilt_angle, g_current_gimbal_state.pan_angle);
            // cout<<"\t"<<g.tilt<<"\t"<<g.pan<<"\n";
            // g_gimbal_output.tilt=g.tilt;
            // g_gimbal_output.pan=g.pan;
        }
        
    }
}