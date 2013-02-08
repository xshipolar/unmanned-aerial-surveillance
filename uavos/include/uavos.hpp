/*
 * uavos.hpp
 *
 *  Created on: Jul 31, 2012
 *      Author: X
 */

#ifndef UAVOS_HPP_
#define UAVOS_HPP_

#include <cstring>
#include "mavlink/common/mavlink.h"

/** Global variables **/
// Autopilot-state related containers
mavlink_heartbeat_t autopilot_heartbeat;
mavlink_system_t autopilot_system;

// Pico-state related containers
mavlink_heartbeat_t pico_heartbeat;
mavlink_system_t pico_system;

// Vision related containers
mavlink_vision_position_estimate_t pico_target_position_estimate;

// Comsys related
string uart0Name = "/dev/ttyS0";
serial_port_base::parity       parityOption = serial_port_base::parity( serial_port_base::parity::even );
serial_port_base::baud_rate      rateOption = serial_port_base::baud_rate(115200);
serial_port_base::stop_bits       bitOption = serial_port_base::stop_bits( serial_port_base::stop_bits::one );
serial_port_base::flow_control   flowOption = serial_port_base::flow_control( serial_port_base::flow_control::none );
serial_port_base::character_size sizeOption = serial_port_base::character_size( serial_port_base::character_size(8));

// Function Prototypes


#endif /* UAVOS_HPP_ */
