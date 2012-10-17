/*
 * uavos.cpp
 *
 *  Created on: Jul 30, 2012
 *      Author: Xichen Shi
 */
#include <comsys.hpp>
#include <uavos.hpp>

/** Global variables **/
// Autopilot-state related containers
mavlink_heartbeat_t autopilot_heartbeat;
mavlink_system_t autopilot_system;

// Pico-state related containers
mavlink_heartbeat_t pico_heartbeat;
mavlink_system_t pico_system;

// Vision related containers
mavlink_vision_position_estimate_t pico_target_position_estimate;

/** Main Loop **/
int main()
{
	comModule();
}
