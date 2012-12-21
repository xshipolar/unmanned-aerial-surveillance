/*
 * File name   : comsys.cpp
 *
 * Description : Communication subsystem
 *
 * Created on  : Jul 31, 2012
 *
 * Author      : Xichen Shi
 */

#include <comsys.hpp>

COMSYS::COMSYS(int Serial) {
    _serialport = Serial;
}

COMSYS::handleMessage() {
    static unsigned char c;
    mavlink_message_t msg;
    mavlink_status_t status;
    if(read(Serial, &c, 1)>0) {
        if (mavlink_parse_char(0, c, &msg, &status)){
            switch (msg.msgid) {
            case MAVLINK_MSG_ID_RAW_IMU:
                mavlink_raw_imu_t raw_imu;
                mavlink_msg_raw_imu_decode(&msg, &raw_imu);
		        break;		
		        
	        case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
	            mavlink_global_position_int_t gps;
	            mavlink_msg_global_position_int_decode(&msg, &gps);
	            break;
	           
            case MAVLINK_MSG_ID_HEARTBEAT:
                mavlink_heartbeat_t heartbeat;
                mavlink_msg_heartbeat_decode(&msg, &heartbeat);
                if (heartbeat.system_status == MAV_STATE_ACTIVE){
                    active = true;
                    lastActiveTime = microSecond();
                    printf("APM is active \n");
                } else if (heartbeat.system_status == MAV_STATE_CALIBRATING) {
                    printf("APM is initialising \n");
                    active = false;
                }
                break;
            
            default:
	            break;
	        }   
        }
    }
}

