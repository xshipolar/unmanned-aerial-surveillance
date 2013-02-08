/*
 * comsys.hpp
 *
 *  Created on: Jul 31, 2012
 *      Author: Xichen Shi
 */

#ifndef COMSYS_HPP_
#define COMSYS_HPP_

#include <iostream>
#include <inttypes.h>
#include <ctime>
#include <cstdio>
#include <mavlink/ardupilotmega/mavlink.h>

using namespace std;

class COM_SYS
{
public:
    void COM_SYS(int Serial);
    
    /* variables */
    
    /* methods */
    bool init();
    void handleMessage();
    void sendMessage();
    void sendStream();
    
    void streamTrigger();

private:
    /* variables */
    int _serialport;
};

#endif /* COMSYS_HPP_ */
