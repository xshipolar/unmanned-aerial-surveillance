/*
 * utility.cpp
 *
 *  Created on: Feb 20, 2013
 *      Author: Xichen Shi
 */

/**
 * @brief -- get current time in microSecond
 * @return -- usec after program start
 */
unsigned long microSecond()
{
    timeval currentTime;
    gettimeofday(&currentTime, NULL);
    unsigned long usec = currentTime.tv_sec * 1000000 + currentTime.tv_usec - startTime;
    return usec;
}