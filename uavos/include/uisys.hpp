/*
 * uisys.hpp
 *
 *  Created on: Mar 06, 2013
 *      Author: Shubham Gupta
 */

#ifndef UISYS_HPP_
#define UISYS_HPP_

#include <UAS_ui.hpp>
#include <opencv2/opencv.hpp>
#include <stdio.h>


void initUisys();

void* runUisys(void*);

bool read_flag=false;

bool set_state_flag=false;

bool check_state_flag=false;

bool exit_flag=false;

#endif /* UISYS_HPP_ */