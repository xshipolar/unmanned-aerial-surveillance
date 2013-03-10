/*
 * sensor.cpp
 *
 *  Created on: Feb 21, 2013
 *      Author: Xichen Shi
 */

#include <datasys.hpp>

// Defining Global Structs
global_position_t g_global_position;
attitude_t        g_attitude;
system_status_t   g_system_status;
gimbal_state_t    g_current_gimbal_state;

void updateGlobalPosition(int32_t lat, int32_t lon, int32_t alt, int32_t relative_alt, int16_t vx, int16_t vy, int16_t vz, uint16_t hdg){
    g_global_position.lat = lat;
    g_global_position.lon = lon;
    g_global_position.alt = alt;
    g_global_position.relative_alt = relative_alt;
    g_global_position.vx  = vx;
    g_global_position.vy  = vy;
    g_global_position.vz  = vz;
    g_global_position.hdg = hdg;
}

void updateAttitude(float roll, float pitch, float yaw, float rollspeed, float pitchspeed, float yawspeed){
    g_attitude.roll  = roll;
    g_attitude.pitch = pitch;
    g_attitude.yaw   = yaw;
    g_attitude.rollspeed  = rollspeed;
    g_attitude.pitchspeed = pitchspeed;
    g_attitude.yawspeed   = yawspeed;
}

void updateCurrentGimbalState(double input_pan_angle, double input_tilt_angle, double input_roll_angle){
    g_current_gimbal_state.pan_angle  = input_pan_angle;
    g_current_gimbal_state.tilt_angle = input_tilt_angle;
    g_current_gimbal_state.roll_angle = input_roll_angle;
}