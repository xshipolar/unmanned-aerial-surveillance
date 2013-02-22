/*
 * utility.cpp
 *
 *  Created on: Feb 20, 2013
 *      Author: Xichen Shi
 */

#include <utility.hpp>

unsigned long program_start_time; // reference time in u_sec for system

/**
 * @brief -- get current time in microSecond
 * @return -- usec after program start
 */
unsigned long getMicroSecond()
{
    timeval currentTime;
    gettimeofday(&currentTime, NULL);
    unsigned long usec = currentTime.tv_sec * 1000000 + currentTime.tv_usec - program_start_time;;
    return usec;
}