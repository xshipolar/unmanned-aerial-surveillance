/*
 * sensor.hpp
 *
 *  Created on: Feb 21, 2013
 *      Author: Xichen Shi
 */

#ifndef DATASYS_HPP
#define DATASYS_HPP

#include <inttypes.h>

// sensor related structs 
struct global_position_t {
    int32_t lat; ///< Latitude, expressed as * 1E7
    int32_t lon; ///< Longitude, expressed as * 1E7
    int32_t alt; ///< Altitude in meters, expressed as * 1000 (millimeters), above MSL
    int32_t relative_alt; ///< Altitude above ground in meters, expressed as * 1000 (millimeters)
    int16_t vx; ///< Ground X Speed (Latitude), expressed as m/s * 100
    int16_t vy; ///< Ground Y Speed (Longitude), expressed as m/s * 100
    int16_t vz; ///< Ground Z Speed (Altitude), expressed as m/s * 100
    uint16_t hdg; ///< Compass heading in degrees * 100, 0.0..359.99 degrees. If unknown, set to: 65535
};

struct attitude_t {
    float roll; ///< Roll angle (rad)
    float pitch; ///< Pitch angle (rad)
    float yaw; ///< Yaw angle (rad)
    float rollspeed; ///< Roll angular speed (rad/s)
    float pitchspeed; ///< Pitch angular speed (rad/s)
    float yawspeed; ///< Yaw angular speed (rad/s)
};

// system status related structs and enums
enum UAS_navigation_mode_t {
    NAV_MODE_IDLE,
    NAV_MODE_SEMIAUTO,
    NAV_MODE_AUTO,
    NAV_MODE_END // Always end this
};

enum UAS_system_state_t {
    SYSTEM_STATE_INITIALIZING,
    SYSTEM_STATE_ACTIVE,
    SYSTEM_STATE_END // Always end this
};

struct system_status_t {
    uint8_t navigation_mode;   // mode of the uav operating system
    uint8_t system_state;  // stage of the current system during operation, e.g initialization, mainloop, etc.
};


struct gimbal
{
    /* data: Euler angles for Gimbal obtained from */
    double tilt;            //Tilt angle in 100degrees
    double pan;             //Pan angle in 100degrees
};



// Prototypes and globals
extern attitude_t           g_attitude;
extern global_position_t    g_global_position;
extern system_status_t      g_system_status;
extern gimbal               g_gimbal_input;     //Input in 100*degrees
extern gimbal               g_gimbal_output;

void updateGlobalPosition(int32_t lat, int32_t lon, int32_t alt, int32_t relative_alt, int16_t vx, int16_t vy, int16_t vz, uint16_t hdg);

void updateAttitude(float roll, float pitch, float yaw, float rollspeed, float pitchspeed, float yawspeed);

#endif /* DATASYS_HPP */