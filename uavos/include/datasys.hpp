/*
 * sensor.hpp
 *
 *  Created on: Feb 21, 2013
 *      Author: Xichen Shi
 */

#ifndef DATASYS_HPP
#define DATASYS_HPP

#include <inttypes.h>

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

extern attitude_t        g_attitude;
extern global_position_t g_global_position;

void updateGlobalPosition(int32_t lat, int32_t lon, int32_t alt, int32_t relative_alt, int16_t vx, int16_t vy, int16_t vz, uint16_t hdg);

void updateAttitude(float roll, float pitch, float yaw, float rollspeed, float pitchspeed, float yawspeed);

#endif /* DATASYS_HPP */