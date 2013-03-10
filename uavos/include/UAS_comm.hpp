/*
 * UAS_comm.hpp
 *
 *  Created on: Feb 26, 2013
 *      Author: Xichen Shi
 */

#ifndef UAS_COMM_HPP_
#define UAS_COMM_HPP_

#include <iostream>
#include <inttypes.h>
#include <ctime>
#include <cstdio>
#include <mavlink/mavlink_types.h>
#include <UAS_serial.hpp>
#include <datasys.hpp>

using namespace std;

// Globals for use in other systems
extern UAS_serial Serial_apm;
extern UAS_serial Serial_gcs;
extern mavlink_system_t mavlink_system;

class UAS_comm
{
public:
    // Constructors
    UAS_comm();
    UAS_comm(UAS_serial* serial_apm, UAS_serial* serial_gcs);
    //~UAS_comm();

    // Initializers
    void init(UAS_serial* serial_apm, UAS_serial* serial_gcs);
    void init(uint32_t baudrate_apm, uint32_t baudrate_gcs);
    void init(const char* serial_name_apm, const char* serial_name_gcs, uint32_t baudrate_apm, uint32_t baudrate_gcs);

    // Updates functions to be called periodically
    void updateApm();
    void updateGcs();
    void parseApmMessage(mavlink_message_t* msg);
    void parseGcsMessage(mavlink_message_t* msg);
    void bypassMessage(uint8_t chan, mavlink_message_t* msg);

    // Get info function sets
    bool isApmOpen();
    bool isGcsOpen();

    // mavlink communication channel 
    uint8_t chan_apm; 
    uint8_t chan_gcs;

private:
    // pointer to communication 
    UAS_serial* _comm_apm; 
    UAS_serial* _comm_gcs;

    uint8_t _rx_buffer_apm[MAVLINK_MAX_PACKET_LEN];
    uint8_t _rx_buffer_gcs[MAVLINK_MAX_PACKET_LEN];

    // states of communication links
    int  _apm_packet_drops;
    int  _gcs_packet_drops;
    bool _apm_initialised;
    bool _gcs_initialised;
};

// For mavlink conveinience functions
#define MAVLINK_USE_CONVENIENCE_FUNCTIONS

/**
 * @brief -- function used for convenience in mavlink
 * @param chan -- channel of the message
 * @param ch   -- character to write to port
 */
static inline void comm_send_ch(mavlink_channel_t chan, uint8_t ch) {
    switch(chan){
    case MAVLINK_COMM_0:
        Serial_apm.send(&ch,1);
        break;
    case MAVLINK_COMM_1:
        Serial_gcs.send(&ch,1);
        break;
    default:
        break;
    }
}

#include <mavlink/uas_mavlink/mavlink.h>

#endif /* UAS_COMM_HPP_ */
