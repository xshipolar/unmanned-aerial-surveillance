#define MAVLINK_USE_CONVENIENCE_FUNCTIONS

#include <mavlink/mavlink_types.h>

mavlink_system_t mavlink_system;

static inline void comm_send_ch(mavlink_channel_t chan, uint8_t ch);
