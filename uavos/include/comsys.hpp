/*
 * comsys.hpp
 *
 *  Created on: Jul 31, 2012
 *      Author: Xichen Shi
 */

#ifndef COMSYS_HPP_
#define COMSYS_HPP_

#include <UAS_comm.hpp>
#include <UAS_serial.hpp>
#include <mavlink/mavlink_types.h>
#include <datasys.hpp>
#include <utility.hpp>
#include <unistd.h>

extern UAS_comm comModule;

void initComsys();

// Control functions
bool setGimbalState(double input_pan_angle, double input_tilt_angle, double input_roll_angle);
void getGimbalState();

void *runApmLink(void*);

void *sendApmMessage(void*);

void *runGcsLink(void*);

void *sendGcsMessage(void*);

void *runComsys(void*);

#endif /* COMSYS_HPP_ */
