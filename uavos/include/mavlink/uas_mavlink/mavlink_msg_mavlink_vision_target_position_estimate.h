// MESSAGE MAVLINK_VISION_TARGET_POSITION_ESTIMATE PACKING

#define MAVLINK_MSG_ID_MAVLINK_VISION_TARGET_POSITION_ESTIMATE 201

typedef struct __mavlink_mavlink_vision_target_position_estimate_t
{
 uint64_t t_usec; ///< Timestamp (milliseconds)
 float x; ///< x position: global: latitude
 float y; ///< y position: global: longitude
 float z; ///< z position: global: altitude
 float roll; ///< roll angle in rad
 float pitch; ///< pitch angle in rad
 float yaw; ///< yaw angle in rad
 uint8_t frame; ///< coordinate frame used, see UAS_FRAME
} mavlink_mavlink_vision_target_position_estimate_t;

#define MAVLINK_MSG_ID_MAVLINK_VISION_TARGET_POSITION_ESTIMATE_LEN 33
#define MAVLINK_MSG_ID_201_LEN 33



#define MAVLINK_MESSAGE_INFO_MAVLINK_VISION_TARGET_POSITION_ESTIMATE { \
	"MAVLINK_VISION_TARGET_POSITION_ESTIMATE", \
	8, \
	{  { "t_usec", NULL, MAVLINK_TYPE_UINT64_T, 0, 0, offsetof(mavlink_mavlink_vision_target_position_estimate_t, t_usec) }, \
         { "x", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_mavlink_vision_target_position_estimate_t, x) }, \
         { "y", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_mavlink_vision_target_position_estimate_t, y) }, \
         { "z", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_mavlink_vision_target_position_estimate_t, z) }, \
         { "roll", NULL, MAVLINK_TYPE_FLOAT, 0, 20, offsetof(mavlink_mavlink_vision_target_position_estimate_t, roll) }, \
         { "pitch", NULL, MAVLINK_TYPE_FLOAT, 0, 24, offsetof(mavlink_mavlink_vision_target_position_estimate_t, pitch) }, \
         { "yaw", NULL, MAVLINK_TYPE_FLOAT, 0, 28, offsetof(mavlink_mavlink_vision_target_position_estimate_t, yaw) }, \
         { "frame", NULL, MAVLINK_TYPE_UINT8_T, 0, 32, offsetof(mavlink_mavlink_vision_target_position_estimate_t, frame) }, \
         } \
}


/**
 * @brief Pack a mavlink_vision_target_position_estimate message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param t_usec Timestamp (milliseconds)
 * @param x x position: global: latitude
 * @param y y position: global: longitude
 * @param z z position: global: altitude
 * @param roll roll angle in rad
 * @param pitch pitch angle in rad
 * @param yaw yaw angle in rad
 * @param frame coordinate frame used, see UAS_FRAME
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mavlink_vision_target_position_estimate_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
						       uint64_t t_usec, float x, float y, float z, float roll, float pitch, float yaw, uint8_t frame)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[33];
	_mav_put_uint64_t(buf, 0, t_usec);
	_mav_put_float(buf, 8, x);
	_mav_put_float(buf, 12, y);
	_mav_put_float(buf, 16, z);
	_mav_put_float(buf, 20, roll);
	_mav_put_float(buf, 24, pitch);
	_mav_put_float(buf, 28, yaw);
	_mav_put_uint8_t(buf, 32, frame);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, 33);
#else
	mavlink_mavlink_vision_target_position_estimate_t packet;
	packet.t_usec = t_usec;
	packet.x = x;
	packet.y = y;
	packet.z = z;
	packet.roll = roll;
	packet.pitch = pitch;
	packet.yaw = yaw;
	packet.frame = frame;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, 33);
#endif

	msg->msgid = MAVLINK_MSG_ID_MAVLINK_VISION_TARGET_POSITION_ESTIMATE;
	return mavlink_finalize_message(msg, system_id, component_id, 33, 120);
}

/**
 * @brief Pack a mavlink_vision_target_position_estimate message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message was sent over
 * @param msg The MAVLink message to compress the data into
 * @param t_usec Timestamp (milliseconds)
 * @param x x position: global: latitude
 * @param y y position: global: longitude
 * @param z z position: global: altitude
 * @param roll roll angle in rad
 * @param pitch pitch angle in rad
 * @param yaw yaw angle in rad
 * @param frame coordinate frame used, see UAS_FRAME
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mavlink_vision_target_position_estimate_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
							   mavlink_message_t* msg,
						           uint64_t t_usec,float x,float y,float z,float roll,float pitch,float yaw,uint8_t frame)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[33];
	_mav_put_uint64_t(buf, 0, t_usec);
	_mav_put_float(buf, 8, x);
	_mav_put_float(buf, 12, y);
	_mav_put_float(buf, 16, z);
	_mav_put_float(buf, 20, roll);
	_mav_put_float(buf, 24, pitch);
	_mav_put_float(buf, 28, yaw);
	_mav_put_uint8_t(buf, 32, frame);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, 33);
#else
	mavlink_mavlink_vision_target_position_estimate_t packet;
	packet.t_usec = t_usec;
	packet.x = x;
	packet.y = y;
	packet.z = z;
	packet.roll = roll;
	packet.pitch = pitch;
	packet.yaw = yaw;
	packet.frame = frame;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, 33);
#endif

	msg->msgid = MAVLINK_MSG_ID_MAVLINK_VISION_TARGET_POSITION_ESTIMATE;
	return mavlink_finalize_message_chan(msg, system_id, component_id, chan, 33, 120);
}

/**
 * @brief Encode a mavlink_vision_target_position_estimate struct into a message
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param mavlink_vision_target_position_estimate C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mavlink_vision_target_position_estimate_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_mavlink_vision_target_position_estimate_t* mavlink_vision_target_position_estimate)
{
	return mavlink_msg_mavlink_vision_target_position_estimate_pack(system_id, component_id, msg, mavlink_vision_target_position_estimate->t_usec, mavlink_vision_target_position_estimate->x, mavlink_vision_target_position_estimate->y, mavlink_vision_target_position_estimate->z, mavlink_vision_target_position_estimate->roll, mavlink_vision_target_position_estimate->pitch, mavlink_vision_target_position_estimate->yaw, mavlink_vision_target_position_estimate->frame);
}

/**
 * @brief Send a mavlink_vision_target_position_estimate message
 * @param chan MAVLink channel to send the message
 *
 * @param t_usec Timestamp (milliseconds)
 * @param x x position: global: latitude
 * @param y y position: global: longitude
 * @param z z position: global: altitude
 * @param roll roll angle in rad
 * @param pitch pitch angle in rad
 * @param yaw yaw angle in rad
 * @param frame coordinate frame used, see UAS_FRAME
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_mavlink_vision_target_position_estimate_send(mavlink_channel_t chan, uint64_t t_usec, float x, float y, float z, float roll, float pitch, float yaw, uint8_t frame)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[33];
	_mav_put_uint64_t(buf, 0, t_usec);
	_mav_put_float(buf, 8, x);
	_mav_put_float(buf, 12, y);
	_mav_put_float(buf, 16, z);
	_mav_put_float(buf, 20, roll);
	_mav_put_float(buf, 24, pitch);
	_mav_put_float(buf, 28, yaw);
	_mav_put_uint8_t(buf, 32, frame);

	_mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAVLINK_VISION_TARGET_POSITION_ESTIMATE, buf, 33, 120);
#else
	mavlink_mavlink_vision_target_position_estimate_t packet;
	packet.t_usec = t_usec;
	packet.x = x;
	packet.y = y;
	packet.z = z;
	packet.roll = roll;
	packet.pitch = pitch;
	packet.yaw = yaw;
	packet.frame = frame;

	_mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAVLINK_VISION_TARGET_POSITION_ESTIMATE, (const char *)&packet, 33, 120);
#endif
}

#endif

// MESSAGE MAVLINK_VISION_TARGET_POSITION_ESTIMATE UNPACKING


/**
 * @brief Get field t_usec from mavlink_vision_target_position_estimate message
 *
 * @return Timestamp (milliseconds)
 */
static inline uint64_t mavlink_msg_mavlink_vision_target_position_estimate_get_t_usec(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint64_t(msg,  0);
}

/**
 * @brief Get field x from mavlink_vision_target_position_estimate message
 *
 * @return x position: global: latitude
 */
static inline float mavlink_msg_mavlink_vision_target_position_estimate_get_x(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field y from mavlink_vision_target_position_estimate message
 *
 * @return y position: global: longitude
 */
static inline float mavlink_msg_mavlink_vision_target_position_estimate_get_y(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Get field z from mavlink_vision_target_position_estimate message
 *
 * @return z position: global: altitude
 */
static inline float mavlink_msg_mavlink_vision_target_position_estimate_get_z(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  16);
}

/**
 * @brief Get field roll from mavlink_vision_target_position_estimate message
 *
 * @return roll angle in rad
 */
static inline float mavlink_msg_mavlink_vision_target_position_estimate_get_roll(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  20);
}

/**
 * @brief Get field pitch from mavlink_vision_target_position_estimate message
 *
 * @return pitch angle in rad
 */
static inline float mavlink_msg_mavlink_vision_target_position_estimate_get_pitch(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  24);
}

/**
 * @brief Get field yaw from mavlink_vision_target_position_estimate message
 *
 * @return yaw angle in rad
 */
static inline float mavlink_msg_mavlink_vision_target_position_estimate_get_yaw(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  28);
}

/**
 * @brief Get field frame from mavlink_vision_target_position_estimate message
 *
 * @return coordinate frame used, see UAS_FRAME
 */
static inline uint8_t mavlink_msg_mavlink_vision_target_position_estimate_get_frame(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  32);
}

/**
 * @brief Decode a mavlink_vision_target_position_estimate message into a struct
 *
 * @param msg The message to decode
 * @param mavlink_vision_target_position_estimate C-struct to decode the message contents into
 */
static inline void mavlink_msg_mavlink_vision_target_position_estimate_decode(const mavlink_message_t* msg, mavlink_mavlink_vision_target_position_estimate_t* mavlink_vision_target_position_estimate)
{
#if MAVLINK_NEED_BYTE_SWAP
	mavlink_vision_target_position_estimate->t_usec = mavlink_msg_mavlink_vision_target_position_estimate_get_t_usec(msg);
	mavlink_vision_target_position_estimate->x = mavlink_msg_mavlink_vision_target_position_estimate_get_x(msg);
	mavlink_vision_target_position_estimate->y = mavlink_msg_mavlink_vision_target_position_estimate_get_y(msg);
	mavlink_vision_target_position_estimate->z = mavlink_msg_mavlink_vision_target_position_estimate_get_z(msg);
	mavlink_vision_target_position_estimate->roll = mavlink_msg_mavlink_vision_target_position_estimate_get_roll(msg);
	mavlink_vision_target_position_estimate->pitch = mavlink_msg_mavlink_vision_target_position_estimate_get_pitch(msg);
	mavlink_vision_target_position_estimate->yaw = mavlink_msg_mavlink_vision_target_position_estimate_get_yaw(msg);
	mavlink_vision_target_position_estimate->frame = mavlink_msg_mavlink_vision_target_position_estimate_get_frame(msg);
#else
	memcpy(mavlink_vision_target_position_estimate, _MAV_PAYLOAD(msg), 33);
#endif
}
