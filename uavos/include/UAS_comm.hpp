/*
 * UAS_comm.hpp
 *
 *  Created on: Jul 31, 2012
 *      Author: Xichen Shi
 */

#ifndef UAS_COMM_HPP_
#define UAS_COMM_HPP_

#include <iostream>
#include <inttypes.h>
#include <ctime>
#include <cstdio>
#include <mavlink/ardupilotmega/mavlink.h>

#include <UAS_serial/UAS_serial.hpp>
#include <sensor.hpp>

using namespace std;

class UAS_comm
{
public:
    UAS_comm(UAS_serial* serial_apm, UAS_serial* serial_gcs);
    //~UAS_comm();

    void init();

    void updateApm();
    void updateGcs();

    void bypassMessage(uint8_t chan, mavlink_message_t* msg);

    void parseApmMessage(mavlink_message_t* msg);
    void parseGcsMessage(mavlink_message_t* msg);

    void updateSensorData();

    // Get info function sets
    bool isApmOpen();
    bool isGcsOpen();

private:
    // pointer to communication 
    UAS_serial* _comm_apm; 
    UAS_serial* _comm_gcs;

    // mavlink communication channel 
    uint8_t _chan_apm; 
    uint8_t _chan_gcs;


    uint8_t _rx_buffer_apm[MAVLINK_MAX_PACKET_LEN];
    uint8_t _rx_buffer_gcs[MAVLINK_MAX_PACKET_LEN];

    // states of communication links
    int  _apm_packet_drops;
    int  _gcs_packet_drops;
    bool _apm_initialised;
    bool _gcs_initialised;
};

#endif /* UAS_COMM_HPP_ */
