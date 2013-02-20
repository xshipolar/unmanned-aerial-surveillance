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

#include <UAS_serial/UAS_serial.hpp>

using namespace std;

class UAS_comm
{
public:
    UAS_comm();
    ~UAS_comm();

    void parse_APM_msg();
    void parse_GCS_msg();

private:

};

#endif /* COMSYS_HPP_ */
