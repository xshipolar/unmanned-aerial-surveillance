/*
 * utility.cpp
 *
 *  Created on: Feb 20, 2013
 *      Author: Xichen Shi
 */

#include <utility.hpp>

unsigned long program_start_time_us; // reference time in u_sec for system
unsigned long program_start_time_ms; // reference time in millisec for system

/**
 * @brief -- get current time in microSecond
 * @return -- usec after program start
 */
unsigned long getMicroSeconds()
{
    timeval currentTime;
    gettimeofday(&currentTime, NULL);
    unsigned long usec = currentTime.tv_sec * 1000000 + currentTime.tv_usec - program_start_time_us;
    return usec;
}

/**
 * @brief -- get current time in milliSecond
 * @return -- usec after program start
 */
unsigned long getMilliSeconds()
{
    timeval currentTime;
    gettimeofday(&currentTime, NULL);
    unsigned long usec = currentTime.tv_sec * 1000 + currentTime.tv_usec/1000 - program_start_time_ms;
    return usec;
}