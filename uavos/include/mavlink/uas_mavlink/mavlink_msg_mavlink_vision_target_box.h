// MESSAGE MAVLINK_VISION_TARGET_BOX PACKING

#define MAVLINK_MSG_ID_MAVLINK_VISION_TARGET_BOX 205

typedef struct __mavlink_mavlink_vision_target_box_t
{
 uint64_t t_usec; ///< Timestamp (milliseconds)
 uint8_t topLeftX; ///< topLeftX
 uint8_t topLeftY; ///< topLeftY
 uint8_t bottomRightX; ///< bottomRightX
 uint8_t botoomRightY; ///< botoomRightY
} mavlink_mavlink_vision_target_box_t;

#define MAVLINK_MSG_ID_MAVLINK_VISION_TARGET_BOX_LEN 12
#define MAVLINK_MSG_ID_205_LEN 12



#define MAVLINK_MESSAGE_INFO_MAVLINK_VISION_TARGET_BOX { \
	"MAVLINK_VISION_TARGET_BOX", \
	5, \
	{  { "t_usec", NULL, MAVLINK_TYPE_UINT64_T, 0, 0, offsetof(mavlink_mavlink_vision_target_box_t, t_usec) }, \
         { "topLeftX", NULL, MAVLINK_TYPE_UINT8_T, 0, 8, offsetof(mavlink_mavlink_vision_target_box_t, topLeftX) }, \
         { "topLeftY", NULL, MAVLINK_TYPE_UINT8_T, 0, 9, offsetof(mavlink_mavlink_vision_target_box_t, topLeftY) }, \
         { "bottomRightX", NULL, MAVLINK_TYPE_UINT8_T, 0, 10, offsetof(mavlink_mavlink_vision_target_box_t, bottomRightX) }, \
         { "botoomRightY", NULL, MAVLINK_TYPE_UINT8_T, 0, 11, offsetof(mavlink_mavlink_vision_target_box_t, botoomRightY) }, \
         } \
}


/**
 * @brief Pack a mavlink_vision_target_box message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param t_usec Timestamp (milliseconds)
 * @param topLeftX topLeftX
 * @param topLeftY topLeftY
 * @param bottomRightX bottomRightX
 * @param botoomRightY botoomRightY
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mavlink_vision_target_box_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
						       uint64_t t_usec, uint8_t topLeftX, uint8_t topLeftY, uint8_t bottomRightX, uint8_t botoomRightY)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[12];
	_mav_put_uint64_t(buf, 0, t_usec);
	_mav_put_uint8_t(buf, 8, topLeftX);
	_mav_put_uint8_t(buf, 9, topLeftY);
	_mav_put_uint8_t(buf, 10, bottomRightX);
	_mav_put_uint8_t(buf, 11, botoomRightY);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, 12);
#else
	mavlink_mavlink_vision_target_box_t packet;
	packet.t_usec = t_usec;
	packet.topLeftX = topLeftX;
	packet.topLeftY = topLeftY;
	packet.bottomRightX = bottomRightX;
	packet.botoomRightY = botoomRightY;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, 12);
#endif

	msg->msgid = MAVLINK_MSG_ID_MAVLINK_VISION_TARGET_BOX;
	return mavlink_finalize_message(msg, system_id, component_id, 12, 11);
}

/**
 * @brief Pack a mavlink_vision_target_box message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message was sent over
 * @param msg The MAVLink message to compress the data into
 * @param t_usec Timestamp (milliseconds)
 * @param topLeftX topLeftX
 * @param topLeftY topLeftY
 * @param bottomRightX bottomRightX
 * @param botoomRightY botoomRightY
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mavlink_vision_target_box_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
							   mavlink_message_t* msg,
						           uint64_t t_usec,uint8_t topLeftX,uint8_t topLeftY,uint8_t bottomRightX,uint8_t botoomRightY)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[12];
	_mav_put_uint64_t(buf, 0, t_usec);
	_mav_put_uint8_t(buf, 8, topLeftX);
	_mav_put_uint8_t(buf, 9, topLeftY);
	_mav_put_uint8_t(buf, 10, bottomRightX);
	_mav_put_uint8_t(buf, 11, botoomRightY);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, 12);
#else
	mavlink_mavlink_vision_target_box_t packet;
	packet.t_usec = t_usec;
	packet.topLeftX = topLeftX;
	packet.topLeftY = topLeftY;
	packet.bottomRightX = bottomRightX;
	packet.botoomRightY = botoomRightY;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, 12);
#endif

	msg->msgid = MAVLINK_MSG_ID_MAVLINK_VISION_TARGET_BOX;
	return mavlink_finalize_message_chan(msg, system_id, component_id, chan, 12, 11);
}

/**
 * @brief Encode a mavlink_vision_target_box struct into a message
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param mavlink_vision_target_box C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mavlink_vision_target_box_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_mavlink_vision_target_box_t* mavlink_vision_target_box)
{
	return mavlink_msg_mavlink_vision_target_box_pack(system_id, component_id, msg, mavlink_vision_target_box->t_usec, mavlink_vision_target_box->topLeftX, mavlink_vision_target_box->topLeftY, mavlink_vision_target_box->bottomRightX, mavlink_vision_target_box->botoomRightY);
}

/**
 * @brief Send a mavlink_vision_target_box message
 * @param chan MAVLink channel to send the message
 *
 * @param t_usec Timestamp (milliseconds)
 * @param topLeftX topLeftX
 * @param topLeftY topLeftY
 * @param bottomRightX bottomRightX
 * @param botoomRightY botoomRightY
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_mavlink_vision_target_box_send(mavlink_channel_t chan, uint64_t t_usec, uint8_t topLeftX, uint8_t topLeftY, uint8_t bottomRightX, uint8_t botoomRightY)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[12];
	_mav_put_uint64_t(buf, 0, t_usec);
	_mav_put_uint8_t(buf, 8, topLeftX);
	_mav_put_uint8_t(buf, 9, topLeftY);
	_mav_put_uint8_t(buf, 10, bottomRightX);
	_mav_put_uint8_t(buf, 11, botoomRightY);

	_mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAVLINK_VISION_TARGET_BOX, buf, 12, 11);
#else
	mavlink_mavlink_vision_target_box_t packet;
	packet.t_usec = t_usec;
	packet.topLeftX = topLeftX;
	packet.topLeftY = topLeftY;
	packet.bottomRightX = bottomRightX;
	packet.botoomRightY = botoomRightY;

	_mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAVLINK_VISION_TARGET_BOX, (const char *)&packet, 12, 11);
#endif
}

#endif

// MESSAGE MAVLINK_VISION_TARGET_BOX UNPACKING


/**
 * @brief Get field t_usec from mavlink_vision_target_box message
 *
 * @return Timestamp (milliseconds)
 */
static inline uint64_t mavlink_msg_mavlink_vision_target_box_get_t_usec(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint64_t(msg,  0);
}

/**
 * @brief Get field topLeftX from mavlink_vision_target_box message
 *
 * @return topLeftX
 */
static inline uint8_t mavlink_msg_mavlink_vision_target_box_get_topLeftX(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  8);
}

/**
 * @brief Get field topLeftY from mavlink_vision_target_box message
 *
 * @return topLeftY
 */
static inline uint8_t mavlink_msg_mavlink_vision_target_box_get_topLeftY(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  9);
}

/**
 * @brief Get field bottomRightX from mavlink_vision_target_box message
 *
 * @return bottomRightX
 */
static inline uint8_t mavlink_msg_mavlink_vision_target_box_get_bottomRightX(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  10);
}

/**
 * @brief Get field botoomRightY from mavlink_vision_target_box message
 *
 * @return botoomRightY
 */
static inline uint8_t mavlink_msg_mavlink_vision_target_box_get_botoomRightY(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  11);
}

/**
 * @brief Decode a mavlink_vision_target_box message into a struct
 *
 * @param msg The message to decode
 * @param mavlink_vision_target_box C-struct to decode the message contents into
 */
static inline void mavlink_msg_mavlink_vision_target_box_decode(const mavlink_message_t* msg, mavlink_mavlink_vision_target_box_t* mavlink_vision_target_box)
{
#if MAVLINK_NEED_BYTE_SWAP
	mavlink_vision_target_box->t_usec = mavlink_msg_mavlink_vision_target_box_get_t_usec(msg);
	mavlink_vision_target_box->topLeftX = mavlink_msg_mavlink_vision_target_box_get_topLeftX(msg);
	mavlink_vision_target_box->topLeftY = mavlink_msg_mavlink_vision_target_box_get_topLeftY(msg);
	mavlink_vision_target_box->bottomRightX = mavlink_msg_mavlink_vision_target_box_get_bottomRightX(msg);
	mavlink_vision_target_box->botoomRightY = mavlink_msg_mavlink_vision_target_box_get_botoomRightY(msg);
#else
	memcpy(mavlink_vision_target_box, _MAV_PAYLOAD(msg), 12);
#endif
}
