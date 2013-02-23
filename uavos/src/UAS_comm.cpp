/*
 * File name   : UAS_comm.cpp
 *
 * Created on  : Jul 31, 2012
 *
 * Author      : Xichen Shi
 */

#include <UAS_comm.hpp>

/**
 * @brief -- Constructor for comm system
 * @param -- two serial classes for comm with APM and GCS
 */
UAS_comm::UAS_comm(UAS_serial* serial_apm, UAS_serial* serial_gcs) {
    _comm_apm = serial_apm; // void pointer to each class
    _comm_gcs = serial_gcs;
}

/**
 * @brief -- initialize comm system by opening two serial ports
 */
void UAS_comm::init(){
     // open ports with baudrate specified
    _apm_initialised = _comm_apm->beginPort(115200);
    _gcs_initialised = _comm_gcs->beginPort(57600); 

    printf("_apm_initialised = %d\n",_apm_initialised);
    printf("_gcs_initialised = %d\n",_gcs_initialised);
   
    // if (_apm_initialised) {
    //     printf("Port to APM is initialised at %s\n",_comm_apm->getDeviceName() );
    // } else {
    //     printf("Port to APM failed to initialised");
    // }
    // if (_gcs_initialised) {
    //     printf("Port to GCS is initialised at %s\n",_comm_gcs->getDeviceName() );
    // } else {
    //     printf("Port to GCS failed to initialised");
    // }


    // assign mavlink_channels to each link
    _chan_apm = MAVLINK_COMM_0; 
    _chan_gcs = MAVLINK_COMM_1;
}

/**
 * @brief -- check the incoming byte from APM
 * @return -- NULL
 */
void UAS_comm::updateApm(){
    mavlink_message_t msg;
    mavlink_status_t status;
    unsigned char c;

    while(_comm_gcs->fetch(&c)>0){
        if (mavlink_parse_char(_chan_apm, c, &msg, &status)){
            parseApmMessage(&msg);
        }
    }

    _apm_packet_drops += status.packet_rx_drop_count;
}

/**
 * @brief -- check the incoming byte from GCS
 * @return -- NULL
 */
void UAS_comm::updateGcs(){
    mavlink_message_t msg;
    mavlink_status_t status;
    unsigned char c;

    while(_comm_gcs->fetch(&c)>0){
        if (mavlink_parse_char(_chan_gcs, c, &msg, &status)){
            parseGcsMessage(&msg);
        }
    }

    _gcs_packet_drops += status.packet_rx_drop_count;
}

/**
 * @brief -- pass-through non-needed message to specified channel 
 * @param chan -- channel to send (apm or gcs)
 * @param msg -- the msg to be sent
 * @return -- NULL
 */
void UAS_comm::bypassMessage(uint8_t chan, mavlink_message_t* msg){
    uint8_t buf[MAVLINK_MAX_PACKET_LEN]; 
    mavlink_msg_to_send_buffer(buf, msg);
    
    if (chan == _chan_gcs){
        _comm_gcs->send(&buf,sizeof(buf)); // send bypass msg to gcs
    }
    if (chan == _chan_apm){
        _comm_apm->send(&buf,sizeof(buf)); // send bypass msg to apm
    }
}

/**
 * @brief -- parse mavlink_msg after the char is processed
 * @param msg -- the msg to be parsed
 * @return -- NULL
 */
void UAS_comm::parseApmMessage(mavlink_message_t* pMsg){
    switch (pMsg->msgid){
    case MAVLINK_MSG_ID_HEARTBEAT:
        mavlink_heartbeat_t heartbeat;
        mavlink_msg_heartbeat_decode(pMsg, &heartbeat);
        break;

    case MAVLINK_MSG_ID_ATTITUDE:
        mavlink_attitude_t attitude;
        mavlink_msg_attitude_decode(pMsg, &attitude);
        updateAttitude(attitude.roll,
                       attitude.pitch,
                       attitude.yaw, 
                       attitude.rollspeed,
                       attitude.pitchspeed, 
                       attitude.yawspeed);
        break;

    case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
        mavlink_global_position_int_t global_position;
        mavlink_msg_global_position_int_decode(pMsg, &global_position);
        updateGlobalPosition(global_position.lat, 
                             global_position.lon, 
                             global_position.alt, 
                             global_position.relative_alt, 
                             global_position.vx, 
                             global_position.vy, 
                             global_position.vz,  
                             global_position.hdg);
        break;

    default:
        if (isGcsOpen()) { 
            bypassMessage(_chan_gcs, pMsg); // pass the message to 
        }
        break;
    } // end switch
}

/**
 * @brief -- parse mavlink_msg after the char is processed
 * @param msg -- the msg to be parsed
 * @return -- NULL
 */
void UAS_comm::parseGcsMessage(mavlink_message_t* pMsg){
    switch (pMsg->msgid) {
    case MAVLINK_MSG_ID_HEARTBEAT:
        mavlink_heartbeat_t heartbeat;
        mavlink_msg_heartbeat_decode(pMsg, &heartbeat);
        break;

    default:
        if (isApmOpen()) {
            bypassMessage(_chan_apm, pMsg); // pass the msg to apm
        }
        break;
    } // end switch
} 

/**
 * @brief -- if apm port is opened
 */
bool UAS_comm::isApmOpen(){
    return _apm_initialised;
}

/**
 * @brief -- if gcs port is opened
 */
bool UAS_comm::isGcsOpen(){
    return _gcs_initialised;
}