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

using namespace std;

class UAS_comm
{
public:
    UAS_comm(UAS_serial* serial_apm, UAS_serial* serial_gcs);
    ~UAS_comm();

    void init();

    void update_apm_rx();
    void update_gcs_rx();

    void send_bypass_msg(uint8_t chan, mavlink_message_t* msg);

    void parse_apm_msg(mavlink_message_t* msg);
    void parse_gcs_msg(mavlink_message_t* msg);

    bool is_apm_open();
    bool is_gcs_open();

private:
    void* _comm_apm;
    void* _comm_gcs;
    uint8_t _chan_apm;
    uint8_t _chan_gcs;
    uint8_t _rx_buffer_apm[MAVLINK_MAX_PACKET_LEN];
    uint8_t _rx_buffer_gcs[MAVLINK_MAX_PACKET_LEN];
    bool _apm_initialised;
    bool _gcs_initialised;
};

#endif /* UAS_COMM_HPP_ */
