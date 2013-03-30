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
#include <ncurses.h>
#include <termios.h>
#include <term.h>
//#include <conio.h>
#include <iostream>
#include <unistd.h>


void initUisys();

void* runUisys(void*);

void init_keyboard();

void close_keyboard();

int kbhit();


#endif /* UISYS_HPP_ */