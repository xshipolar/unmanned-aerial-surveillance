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
    _apm_initialised = !_comm_apm->beginPort(115200); // open ports with baudrate specified
    _gcs_initialised = !_comm_gcs->beginPort(57600); 

    _chan_apm = MAVLINK_COMM_0; // assign mavlink_channels to each link
    _chan_gcs = MAVLINK_COMM_1;
}

/**
 * @brief -- check the incoming byte from APM
 * @return -- NULL
 */
void UAS_comm::update_apm_rx(){
    mavlink_message_t msg;
    mavlink_status_t status;
    unsigned char c;

    while(_comm_gcs->fetch(&c)>0){
        if (mavlink_parse_char(_chan_apm, c, &msg, &status)){
            parse_apm_msg(&msg);
        }
    }
}

/**
 * @brief -- check the incoming byte from GCS
 * @return -- NULL
 */
void UAS_comm::update_gcs_rx(){
    mavlink_message_t msg;
    mavlink_status_t status;
    unsigned char c;

    while(_comm_gcs->fetch(&c)>0){
        if (mavlink_parse_char(_chan_gcs, c, &msg, &status)){
            parse_gcs_msg(&msg);
        }
    }
}

/**
 * @brief -- pass-through non-needed message to specified channel 
 * @param chan -- channel to send (apm or gcs)
 * @param msg -- the msg to be sent
 * @return -- NULL
 */
void send_bypass_msg(uint8_t chan, mavlink_message_t* msg){
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
void UAS_comm::parse_apm_msg(mavlink_message_t* msg){
}

/**
 * @brief -- parse mavlink_msg after the char is processed
 * @param msg -- the msg to be parsed
 * @return -- NULL
 */
void UAS_comm::parse_gcs_msg(mavlink_message_t* msg){
    switch (msg->msgid) {

    case MAVLINK_MSG_ID_REQUEST_DATA_STREAM:
    {
        // decode
        mavlink_request_data_stream_t packet;
        mavlink_msg_request_data_stream_decode(msg, &packet);
        break;
    }

    case MAVLINK_MSG_ID_COMMAND_LONG:
    {
        // decode
        mavlink_command_long_t packet;
        mavlink_msg_command_long_decode(msg, &packet);
        break;
    }


    case MAVLINK_MSG_ID_SET_MODE:
    {
        // decode
        mavlink_set_mode_t packet;
        mavlink_msg_set_mode_decode(msg, &packet);
        break;

    }

    case MAVLINK_MSG_ID_MISSION_REQUEST_LIST:
    {
        // decode
        mavlink_mission_request_list_t packet;
        mavlink_msg_mission_request_list_decode(msg, &packet);
        break;
    }


    // XXX read a WP from EEPROM and send it to the GCS
    case MAVLINK_MSG_ID_MISSION_REQUEST:
    {
        // decode
        mavlink_mission_request_t packet;
        mavlink_msg_mission_request_decode(msg, &packet);
        break;
    }


    case MAVLINK_MSG_ID_MISSION_ACK:
    {
        // decode
        mavlink_mission_ack_t packet;
        mavlink_msg_mission_ack_decode(msg, &packet);
        break;
    }

    case MAVLINK_MSG_ID_PARAM_REQUEST_LIST:
    {
        // decode
        mavlink_param_request_list_t packet;
        mavlink_msg_param_request_list_decode(msg, &packet);
        break;
    }

    case MAVLINK_MSG_ID_PARAM_REQUEST_READ:
    {
        // decode
        mavlink_param_request_read_t packet;
        mavlink_msg_param_request_read_decode(msg, &packet);
        break;
    }

    case MAVLINK_MSG_ID_MISSION_CLEAR_ALL:
    {
        // decode
        mavlink_mission_clear_all_t packet;
        mavlink_msg_mission_clear_all_decode(msg, &packet);
        break;
    }

    case MAVLINK_MSG_ID_MISSION_SET_CURRENT:
    {
        // decode
        mavlink_mission_set_current_t packet;
        mavlink_msg_mission_set_current_decode(msg, &packet);
        break;
    }

    case MAVLINK_MSG_ID_MISSION_COUNT:
    {
        // decode
        mavlink_mission_count_t packet;
        mavlink_msg_mission_count_decode(msg, &packet);
        break;
    }

    case MAVLINK_MSG_ID_MISSION_WRITE_PARTIAL_LIST:
    {
        // decode
        mavlink_mission_write_partial_list_t packet;
        mavlink_msg_mission_write_partial_list_decode(msg, &packet);
        break;
    }

    case MAVLINK_MSG_ID_SET_MAG_OFFSETS:
    {
        mavlink_set_mag_offsets_t packet;
        mavlink_msg_set_mag_offsets_decode(msg, &packet);
        break;
    }

    // XXX receive a WP from GCS and store in EEPROM
    case MAVLINK_MSG_ID_MISSION_ITEM:
    {
        // decode
        mavlink_mission_item_t packet;
        uint8_t result = MAV_MISSION_ACCEPTED;

        mavlink_msg_mission_item_decode(msg, &packet);
        break;
    }

    // receive a fence point from GCS and store in EEPROM
    case MAVLINK_MSG_ID_FENCE_POINT: {
        mavlink_fence_point_t packet;
        mavlink_msg_fence_point_decode(msg, &packet);
        break;
    }

    // send a fence point to GCS
    case MAVLINK_MSG_ID_FENCE_FETCH_POINT: {
        mavlink_fence_fetch_point_t packet;
        mavlink_msg_fence_fetch_point_decode(msg, &packet);
        break;
    }

    case MAVLINK_MSG_ID_PARAM_SET:
    {
        // decode
        mavlink_param_set_t packet;
        mavlink_msg_param_set_decode(msg, &packet);
        break;

    } // end case

    case MAVLINK_MSG_ID_RC_CHANNELS_OVERRIDE:
    {
        if(msg->sysid != g.sysid_my_gcs) break; // Only accept control from our gcs
        mavlink_rc_channels_override_t packet;
        mavlink_msg_rc_channels_override_decode(msg, &packet);
        break;
    }

    case MAVLINK_MSG_ID_HEARTBEAT:
    {
        // We keep track of the last time we received a heartbeat from
        // our GCS for failsafe purposes
        if (msg->sysid != g.sysid_my_gcs) break;
        last_heartbeat_ms = millis();
        pmTest1++;
        break;
    }

    case MAVLINK_MSG_ID_DIGICAM_CONFIGURE:
    {
        camera.configure_msg(msg);
        break;
    }

    case MAVLINK_MSG_ID_DIGICAM_CONTROL:
    {
        camera.control_msg(msg);
        break;
    }

    case MAVLINK_MSG_ID_MOUNT_CONFIGURE:
    {
        camera_mount.configure_msg(msg);
        break;
    }

    case MAVLINK_MSG_ID_MOUNT_CONTROL:
    {
        camera_mount.control_msg(msg);
        break;
    }

    case MAVLINK_MSG_ID_MOUNT_STATUS:
    {
        camera_mount.status_msg(msg);
        break;
    }

    case MAVLINK_MSG_ID_RADIO:
    {
        mavlink_radio_t packet;
        mavlink_msg_radio_decode(msg, &packet);
        break;
    }

    default:
        // forward unknown messages to the other link if there is one
        if ((chan == MAVLINK_COMM_1 && gcs0.initialised) ||
            (chan == MAVLINK_COMM_0 && gcs3.initialised)) {
            mavlink_channel_t out_chan = (mavlink_channel_t)(((uint8_t)chan)^1);
            // only forward if it would fit in our transmit buffer
            if (comm_get_txspace(out_chan) > ((uint16_t)msg->len) + MAVLINK_NUM_NON_PAYLOAD_BYTES) {
                _mavlink_resend_uart(out_chan, msg);
            }
        }
        break;

    } // end switch
} 

/**
 * @brief -- if apm port is opened
 */
bool UAS_comm::is_apm_open(){
    return _apm_initialised;
}

/**
 * @brief -- if gcs port is opened
 */
bool UAS_comm::is_gcs_open(){
    return _gcs_initialised;
}