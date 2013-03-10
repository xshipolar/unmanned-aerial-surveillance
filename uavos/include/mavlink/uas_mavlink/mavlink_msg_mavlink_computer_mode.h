// MESSAGE MAVLINK_COMPUTER_MODE PACKING

#define MAVLINK_MSG_ID_MAVLINK_COMPUTER_MODE 203

typedef struct __mavlink_mavlink_computer_mode_t
{
 uint64_t t_usec; ///< Timestamp (milliseconds)
 uint8_t mode; ///< mode enums
} mavlink_mavlink_computer_mode_t;

#define MAVLINK_MSG_ID_MAVLINK_COMPUTER_MODE_LEN 9
#define MAVLINK_MSG_ID_203_LEN 9



#define MAVLINK_MESSAGE_INFO_MAVLINK_COMPUTER_MODE { \
	"MAVLINK_COMPUTER_MODE", \
	2, \
	{  { "t_usec", NULL, MAVLINK_TYPE_UINT64_T, 0, 0, offsetof(mavlink_mavlink_computer_mode_t, t_usec) }, \
         { "mode", NULL, MAVLINK_TYPE_UINT8_T, 0, 8, offsetof(mavlink_mavlink_computer_mode_t, mode) }, \
         } \
}


/**
 * @brief Pack a mavlink_computer_mode message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param t_usec Timestamp (milliseconds)
 * @param mode mode enums
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mavlink_computer_mode_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
						       uint64_t t_usec, uint8_t mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[9];
	_mav_put_uint64_t(buf, 0, t_usec);
	_mav_put_uint8_t(buf, 8, mode);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, 9);
#else
	mavlink_mavlink_computer_mode_t packet;
	packet.t_usec = t_usec;
	packet.mode = mode;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, 9);
#endif

	msg->msgid = MAVLINK_MSG_ID_MAVLINK_COMPUTER_MODE;
	return mavlink_finalize_message(msg, system_id, component_id, 9, 15);
}

/**
 * @brief Pack a mavlink_computer_mode message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message was sent over
 * @param msg The MAVLink message to compress the data into
 * @param t_usec Timestamp (milliseconds)
 * @param mode mode enums
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mavlink_computer_mode_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
							   mavlink_message_t* msg,
						           uint64_t t_usec,uint8_t mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[9];
	_mav_put_uint64_t(buf, 0, t_usec);
	_mav_put_uint8_t(buf, 8, mode);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, 9);
#else
	mavlink_mavlink_computer_mode_t packet;
	packet.t_usec = t_usec;
	packet.mode = mode;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, 9);
#endif

	msg->msgid = MAVLINK_MSG_ID_MAVLINK_COMPUTER_MODE;
	return mavlink_finalize_message_chan(msg, system_id, component_id, chan, 9, 15);
}

/**
 * @brief Encode a mavlink_computer_mode struct into a message
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param mavlink_computer_mode C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mavlink_computer_mode_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_mavlink_computer_mode_t* mavlink_computer_mode)
{
	return mavlink_msg_mavlink_computer_mode_pack(system_id, component_id, msg, mavlink_computer_mode->t_usec, mavlink_computer_mode->mode);
}

/**
 * @brief Send a mavlink_computer_mode message
 * @param chan MAVLink channel to send the message
 *
 * @param t_usec Timestamp (milliseconds)
 * @param mode mode enums
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_mavlink_computer_mode_send(mavlink_channel_t chan, uint64_t t_usec, uint8_t mode)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[9];
	_mav_put_uint64_t(buf, 0, t_usec);
	_mav_put_uint8_t(buf, 8, mode);

	_mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAVLINK_COMPUTER_MODE, buf, 9, 15);
#else
	mavlink_mavlink_computer_mode_t packet;
	packet.t_usec = t_usec;
	packet.mode = mode;

	_mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAVLINK_COMPUTER_MODE, (const char *)&packet, 9, 15);
#endif
}

#endif

// MESSAGE MAVLINK_COMPUTER_MODE UNPACKING


/**
 * @brief Get field t_usec from mavlink_computer_mode message
 *
 * @return Timestamp (milliseconds)
 */
static inline uint64_t mavlink_msg_mavlink_computer_mode_get_t_usec(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint64_t(msg,  0);
}

/**
 * @brief Get field mode from mavlink_computer_mode message
 *
 * @return mode enums
 */
static inline uint8_t mavlink_msg_mavlink_computer_mode_get_mode(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  8);
}

/**
 * @brief Decode a mavlink_computer_mode message into a struct
 *
 * @param msg The message to decode
 * @param mavlink_computer_mode C-struct to decode the message contents into
 */
static inline void mavlink_msg_mavlink_computer_mode_decode(const mavlink_message_t* msg, mavlink_mavlink_computer_mode_t* mavlink_computer_mode)
{
#if MAVLINK_NEED_BYTE_SWAP
	mavlink_computer_mode->t_usec = mavlink_msg_mavlink_computer_mode_get_t_usec(msg);
	mavlink_computer_mode->mode = mavlink_msg_mavlink_computer_mode_get_mode(msg);
#else
	memcpy(mavlink_computer_mode, _MAV_PAYLOAD(msg), 9);
#endif
}
