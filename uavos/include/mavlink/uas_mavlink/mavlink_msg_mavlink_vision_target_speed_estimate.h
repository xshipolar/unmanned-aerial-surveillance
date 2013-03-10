// MESSAGE MAVLINK_VISION_TARGET_SPEED_ESTIMATE PACKING

#define MAVLINK_MSG_ID_MAVLINK_VISION_TARGET_SPEED_ESTIMATE 202

typedef struct __mavlink_mavlink_vision_target_speed_estimate_t
{
 uint64_t t_usec; ///< Timestamp (milliseconds)
 float vx; ///< in meter/sec
 float vy; ///< in meter/sec
 float vz; ///< in meter/sec
 uint8_t frame; ///< coordinate frame used, see UAS_FRAME
} mavlink_mavlink_vision_target_speed_estimate_t;

#define MAVLINK_MSG_ID_MAVLINK_VISION_TARGET_SPEED_ESTIMATE_LEN 21
#define MAVLINK_MSG_ID_202_LEN 21



#define MAVLINK_MESSAGE_INFO_MAVLINK_VISION_TARGET_SPEED_ESTIMATE { \
	"MAVLINK_VISION_TARGET_SPEED_ESTIMATE", \
	5, \
	{  { "t_usec", NULL, MAVLINK_TYPE_UINT64_T, 0, 0, offsetof(mavlink_mavlink_vision_target_speed_estimate_t, t_usec) }, \
         { "vx", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_mavlink_vision_target_speed_estimate_t, vx) }, \
         { "vy", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_mavlink_vision_target_speed_estimate_t, vy) }, \
         { "vz", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_mavlink_vision_target_speed_estimate_t, vz) }, \
         { "frame", NULL, MAVLINK_TYPE_UINT8_T, 0, 20, offsetof(mavlink_mavlink_vision_target_speed_estimate_t, frame) }, \
         } \
}


/**
 * @brief Pack a mavlink_vision_target_speed_estimate message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param t_usec Timestamp (milliseconds)
 * @param vx in meter/sec
 * @param vy in meter/sec
 * @param vz in meter/sec
 * @param frame coordinate frame used, see UAS_FRAME
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mavlink_vision_target_speed_estimate_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
						       uint64_t t_usec, float vx, float vy, float vz, uint8_t frame)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[21];
	_mav_put_uint64_t(buf, 0, t_usec);
	_mav_put_float(buf, 8, vx);
	_mav_put_float(buf, 12, vy);
	_mav_put_float(buf, 16, vz);
	_mav_put_uint8_t(buf, 20, frame);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, 21);
#else
	mavlink_mavlink_vision_target_speed_estimate_t packet;
	packet.t_usec = t_usec;
	packet.vx = vx;
	packet.vy = vy;
	packet.vz = vz;
	packet.frame = frame;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, 21);
#endif

	msg->msgid = MAVLINK_MSG_ID_MAVLINK_VISION_TARGET_SPEED_ESTIMATE;
	return mavlink_finalize_message(msg, system_id, component_id, 21, 25);
}

/**
 * @brief Pack a mavlink_vision_target_speed_estimate message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message was sent over
 * @param msg The MAVLink message to compress the data into
 * @param t_usec Timestamp (milliseconds)
 * @param vx in meter/sec
 * @param vy in meter/sec
 * @param vz in meter/sec
 * @param frame coordinate frame used, see UAS_FRAME
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_mavlink_vision_target_speed_estimate_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
							   mavlink_message_t* msg,
						           uint64_t t_usec,float vx,float vy,float vz,uint8_t frame)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[21];
	_mav_put_uint64_t(buf, 0, t_usec);
	_mav_put_float(buf, 8, vx);
	_mav_put_float(buf, 12, vy);
	_mav_put_float(buf, 16, vz);
	_mav_put_uint8_t(buf, 20, frame);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, 21);
#else
	mavlink_mavlink_vision_target_speed_estimate_t packet;
	packet.t_usec = t_usec;
	packet.vx = vx;
	packet.vy = vy;
	packet.vz = vz;
	packet.frame = frame;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, 21);
#endif

	msg->msgid = MAVLINK_MSG_ID_MAVLINK_VISION_TARGET_SPEED_ESTIMATE;
	return mavlink_finalize_message_chan(msg, system_id, component_id, chan, 21, 25);
}

/**
 * @brief Encode a mavlink_vision_target_speed_estimate struct into a message
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param mavlink_vision_target_speed_estimate C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_mavlink_vision_target_speed_estimate_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_mavlink_vision_target_speed_estimate_t* mavlink_vision_target_speed_estimate)
{
	return mavlink_msg_mavlink_vision_target_speed_estimate_pack(system_id, component_id, msg, mavlink_vision_target_speed_estimate->t_usec, mavlink_vision_target_speed_estimate->vx, mavlink_vision_target_speed_estimate->vy, mavlink_vision_target_speed_estimate->vz, mavlink_vision_target_speed_estimate->frame);
}

/**
 * @brief Send a mavlink_vision_target_speed_estimate message
 * @param chan MAVLink channel to send the message
 *
 * @param t_usec Timestamp (milliseconds)
 * @param vx in meter/sec
 * @param vy in meter/sec
 * @param vz in meter/sec
 * @param frame coordinate frame used, see UAS_FRAME
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_mavlink_vision_target_speed_estimate_send(mavlink_channel_t chan, uint64_t t_usec, float vx, float vy, float vz, uint8_t frame)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[21];
	_mav_put_uint64_t(buf, 0, t_usec);
	_mav_put_float(buf, 8, vx);
	_mav_put_float(buf, 12, vy);
	_mav_put_float(buf, 16, vz);
	_mav_put_uint8_t(buf, 20, frame);

	_mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAVLINK_VISION_TARGET_SPEED_ESTIMATE, buf, 21, 25);
#else
	mavlink_mavlink_vision_target_speed_estimate_t packet;
	packet.t_usec = t_usec;
	packet.vx = vx;
	packet.vy = vy;
	packet.vz = vz;
	packet.frame = frame;

	_mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_MAVLINK_VISION_TARGET_SPEED_ESTIMATE, (const char *)&packet, 21, 25);
#endif
}

#endif

// MESSAGE MAVLINK_VISION_TARGET_SPEED_ESTIMATE UNPACKING


/**
 * @brief Get field t_usec from mavlink_vision_target_speed_estimate message
 *
 * @return Timestamp (milliseconds)
 */
static inline uint64_t mavlink_msg_mavlink_vision_target_speed_estimate_get_t_usec(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint64_t(msg,  0);
}

/**
 * @brief Get field vx from mavlink_vision_target_speed_estimate message
 *
 * @return in meter/sec
 */
static inline float mavlink_msg_mavlink_vision_target_speed_estimate_get_vx(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field vy from mavlink_vision_target_speed_estimate message
 *
 * @return in meter/sec
 */
static inline float mavlink_msg_mavlink_vision_target_speed_estimate_get_vy(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Get field vz from mavlink_vision_target_speed_estimate message
 *
 * @return in meter/sec
 */
static inline float mavlink_msg_mavlink_vision_target_speed_estimate_get_vz(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  16);
}

/**
 * @brief Get field frame from mavlink_vision_target_speed_estimate message
 *
 * @return coordinate frame used, see UAS_FRAME
 */
static inline uint8_t mavlink_msg_mavlink_vision_target_speed_estimate_get_frame(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  20);
}

/**
 * @brief Decode a mavlink_vision_target_speed_estimate message into a struct
 *
 * @param msg The message to decode
 * @param mavlink_vision_target_speed_estimate C-struct to decode the message contents into
 */
static inline void mavlink_msg_mavlink_vision_target_speed_estimate_decode(const mavlink_message_t* msg, mavlink_mavlink_vision_target_speed_estimate_t* mavlink_vision_target_speed_estimate)
{
#if MAVLINK_NEED_BYTE_SWAP
	mavlink_vision_target_speed_estimate->t_usec = mavlink_msg_mavlink_vision_target_speed_estimate_get_t_usec(msg);
	mavlink_vision_target_speed_estimate->vx = mavlink_msg_mavlink_vision_target_speed_estimate_get_vx(msg);
	mavlink_vision_target_speed_estimate->vy = mavlink_msg_mavlink_vision_target_speed_estimate_get_vy(msg);
	mavlink_vision_target_speed_estimate->vz = mavlink_msg_mavlink_vision_target_speed_estimate_get_vz(msg);
	mavlink_vision_target_speed_estimate->frame = mavlink_msg_mavlink_vision_target_speed_estimate_get_frame(msg);
#else
	memcpy(mavlink_vision_target_speed_estimate, _MAV_PAYLOAD(msg), 21);
#endif
}
