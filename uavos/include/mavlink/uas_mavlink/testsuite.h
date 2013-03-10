/** @file
 *	@brief MAVLink comm protocol testsuite generated from uas_mavlink.xml
 *	@see http://qgroundcontrol.org/mavlink/
 */
#ifndef UAS_MAVLINK_TESTSUITE_H
#define UAS_MAVLINK_TESTSUITE_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MAVLINK_TEST_ALL
#define MAVLINK_TEST_ALL
static void mavlink_test_ardupilotmega(uint8_t, uint8_t, mavlink_message_t *last_msg);
static void mavlink_test_common(uint8_t, uint8_t, mavlink_message_t *last_msg);
static void mavlink_test_uas_mavlink(uint8_t, uint8_t, mavlink_message_t *last_msg);

static void mavlink_test_all(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
	mavlink_test_ardupilotmega(system_id, component_id, last_msg);
	mavlink_test_common(system_id, component_id, last_msg);
	mavlink_test_uas_mavlink(system_id, component_id, last_msg);
}
#endif

#include "../ardupilotmega/testsuite.h"
#include "../common/testsuite.h"


static void mavlink_test_mavlink_vision_target_position_estimate(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
	mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
	mavlink_mavlink_vision_target_position_estimate_t packet_in = {
		93372036854775807ULL,
	73.0,
	101.0,
	129.0,
	157.0,
	185.0,
	213.0,
	101,
	};
	mavlink_mavlink_vision_target_position_estimate_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        	packet1.t_usec = packet_in.t_usec;
        	packet1.x = packet_in.x;
        	packet1.y = packet_in.y;
        	packet1.z = packet_in.z;
        	packet1.roll = packet_in.roll;
        	packet1.pitch = packet_in.pitch;
        	packet1.yaw = packet_in.yaw;
        	packet1.frame = packet_in.frame;
        
        

        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_mavlink_vision_target_position_estimate_encode(system_id, component_id, &msg, &packet1);
	mavlink_msg_mavlink_vision_target_position_estimate_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_mavlink_vision_target_position_estimate_pack(system_id, component_id, &msg , packet1.t_usec , packet1.x , packet1.y , packet1.z , packet1.roll , packet1.pitch , packet1.yaw , packet1.frame );
	mavlink_msg_mavlink_vision_target_position_estimate_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_mavlink_vision_target_position_estimate_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.t_usec , packet1.x , packet1.y , packet1.z , packet1.roll , packet1.pitch , packet1.yaw , packet1.frame );
	mavlink_msg_mavlink_vision_target_position_estimate_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
        	comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
	mavlink_msg_mavlink_vision_target_position_estimate_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_mavlink_vision_target_position_estimate_send(MAVLINK_COMM_1 , packet1.t_usec , packet1.x , packet1.y , packet1.z , packet1.roll , packet1.pitch , packet1.yaw , packet1.frame );
	mavlink_msg_mavlink_vision_target_position_estimate_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_mavlink_vision_target_speed_estimate(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
	mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
	mavlink_mavlink_vision_target_speed_estimate_t packet_in = {
		93372036854775807ULL,
	73.0,
	101.0,
	129.0,
	65,
	};
	mavlink_mavlink_vision_target_speed_estimate_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        	packet1.t_usec = packet_in.t_usec;
        	packet1.vx = packet_in.vx;
        	packet1.vy = packet_in.vy;
        	packet1.vz = packet_in.vz;
        	packet1.frame = packet_in.frame;
        
        

        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_mavlink_vision_target_speed_estimate_encode(system_id, component_id, &msg, &packet1);
	mavlink_msg_mavlink_vision_target_speed_estimate_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_mavlink_vision_target_speed_estimate_pack(system_id, component_id, &msg , packet1.t_usec , packet1.vx , packet1.vy , packet1.vz , packet1.frame );
	mavlink_msg_mavlink_vision_target_speed_estimate_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_mavlink_vision_target_speed_estimate_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.t_usec , packet1.vx , packet1.vy , packet1.vz , packet1.frame );
	mavlink_msg_mavlink_vision_target_speed_estimate_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
        	comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
	mavlink_msg_mavlink_vision_target_speed_estimate_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_mavlink_vision_target_speed_estimate_send(MAVLINK_COMM_1 , packet1.t_usec , packet1.vx , packet1.vy , packet1.vz , packet1.frame );
	mavlink_msg_mavlink_vision_target_speed_estimate_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_mavlink_computer_mode(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
	mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
	mavlink_mavlink_computer_mode_t packet_in = {
		93372036854775807ULL,
	29,
	};
	mavlink_mavlink_computer_mode_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        	packet1.t_usec = packet_in.t_usec;
        	packet1.mode = packet_in.mode;
        
        

        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_mavlink_computer_mode_encode(system_id, component_id, &msg, &packet1);
	mavlink_msg_mavlink_computer_mode_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_mavlink_computer_mode_pack(system_id, component_id, &msg , packet1.t_usec , packet1.mode );
	mavlink_msg_mavlink_computer_mode_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_mavlink_computer_mode_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.t_usec , packet1.mode );
	mavlink_msg_mavlink_computer_mode_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
        	comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
	mavlink_msg_mavlink_computer_mode_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_mavlink_computer_mode_send(MAVLINK_COMM_1 , packet1.t_usec , packet1.mode );
	mavlink_msg_mavlink_computer_mode_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_mavlink_set_computer_mode(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
	mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
	mavlink_mavlink_set_computer_mode_t packet_in = {
		93372036854775807ULL,
	29,
	};
	mavlink_mavlink_set_computer_mode_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        	packet1.t_usec = packet_in.t_usec;
        	packet1.mode = packet_in.mode;
        
        

        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_mavlink_set_computer_mode_encode(system_id, component_id, &msg, &packet1);
	mavlink_msg_mavlink_set_computer_mode_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_mavlink_set_computer_mode_pack(system_id, component_id, &msg , packet1.t_usec , packet1.mode );
	mavlink_msg_mavlink_set_computer_mode_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_mavlink_set_computer_mode_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.t_usec , packet1.mode );
	mavlink_msg_mavlink_set_computer_mode_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
        	comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
	mavlink_msg_mavlink_set_computer_mode_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_mavlink_set_computer_mode_send(MAVLINK_COMM_1 , packet1.t_usec , packet1.mode );
	mavlink_msg_mavlink_set_computer_mode_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_mavlink_vision_target_box(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
	mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
	mavlink_mavlink_vision_target_box_t packet_in = {
		93372036854775807ULL,
	29,
	96,
	163,
	230,
	};
	mavlink_mavlink_vision_target_box_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        	packet1.t_usec = packet_in.t_usec;
        	packet1.topLeftX = packet_in.topLeftX;
        	packet1.topLeftY = packet_in.topLeftY;
        	packet1.bottomRightX = packet_in.bottomRightX;
        	packet1.botoomRightY = packet_in.botoomRightY;
        
        

        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_mavlink_vision_target_box_encode(system_id, component_id, &msg, &packet1);
	mavlink_msg_mavlink_vision_target_box_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_mavlink_vision_target_box_pack(system_id, component_id, &msg , packet1.t_usec , packet1.topLeftX , packet1.topLeftY , packet1.bottomRightX , packet1.botoomRightY );
	mavlink_msg_mavlink_vision_target_box_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_mavlink_vision_target_box_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.t_usec , packet1.topLeftX , packet1.topLeftY , packet1.bottomRightX , packet1.botoomRightY );
	mavlink_msg_mavlink_vision_target_box_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
        	comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
	mavlink_msg_mavlink_vision_target_box_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_mavlink_vision_target_box_send(MAVLINK_COMM_1 , packet1.t_usec , packet1.topLeftX , packet1.topLeftY , packet1.bottomRightX , packet1.botoomRightY );
	mavlink_msg_mavlink_vision_target_box_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_mavlink_vision_set_target_box(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
	mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
	mavlink_mavlink_vision_set_target_box_t packet_in = {
		93372036854775807ULL,
	29,
	96,
	163,
	230,
	};
	mavlink_mavlink_vision_set_target_box_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        	packet1.t_usec = packet_in.t_usec;
        	packet1.topLeftX = packet_in.topLeftX;
        	packet1.topLeftY = packet_in.topLeftY;
        	packet1.bottomRightX = packet_in.bottomRightX;
        	packet1.botoomRightY = packet_in.botoomRightY;
        
        

        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_mavlink_vision_set_target_box_encode(system_id, component_id, &msg, &packet1);
	mavlink_msg_mavlink_vision_set_target_box_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_mavlink_vision_set_target_box_pack(system_id, component_id, &msg , packet1.t_usec , packet1.topLeftX , packet1.topLeftY , packet1.bottomRightX , packet1.botoomRightY );
	mavlink_msg_mavlink_vision_set_target_box_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_mavlink_vision_set_target_box_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.t_usec , packet1.topLeftX , packet1.topLeftY , packet1.bottomRightX , packet1.botoomRightY );
	mavlink_msg_mavlink_vision_set_target_box_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
        	comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
	mavlink_msg_mavlink_vision_set_target_box_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
	mavlink_msg_mavlink_vision_set_target_box_send(MAVLINK_COMM_1 , packet1.t_usec , packet1.topLeftX , packet1.topLeftY , packet1.bottomRightX , packet1.botoomRightY );
	mavlink_msg_mavlink_vision_set_target_box_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
}

static void mavlink_test_uas_mavlink(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
	mavlink_test_mavlink_vision_target_position_estimate(system_id, component_id, last_msg);
	mavlink_test_mavlink_vision_target_speed_estimate(system_id, component_id, last_msg);
	mavlink_test_mavlink_computer_mode(system_id, component_id, last_msg);
	mavlink_test_mavlink_set_computer_mode(system_id, component_id, last_msg);
	mavlink_test_mavlink_vision_target_box(system_id, component_id, last_msg);
	mavlink_test_mavlink_vision_set_target_box(system_id, component_id, last_msg);
}

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // UAS_MAVLINK_TESTSUITE_H
