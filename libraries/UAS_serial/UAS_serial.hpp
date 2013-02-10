/*
 *
 * Author: Xichen Shi
 * Derived from codes by Teunis van Beelen
 * Date created: Feb 8, 2013
 *
 */
 
#ifndef UAS_SERIAL_HPP_
#define UAS_SERIAL_HPP_

#include <termios.h>
#include <cstdio>
#include <cstring>
#include <inttypes.h>
#include <fcntl.h>
#include <unistd.h>

class UAS_serial {
public:
	UAS_serial(const char* serial_name);
    bool beginPort(uint32_t baudrate);
/**
 * @brief fetch the next coming byte
 * @param c -- the char to be read into
 * @return length of the packet (bytes)
 */
    template <class serial_buffer>
    int fetch(serial_buffer* rx_buffer){
        int len;
        len = read(_serial_id, rx_buffer, sizeof(rx_buffer));
        return len;
    }

/**
 * @brief send out a buffer of bytes
 * @param buf --array to be sent 
 * @return length of the packet (bytes)
 */
    template <class serial_buffer>
    int send(serial_buffer* tx_buffer){
        int len;
        len = write(_serial_id, tx_buffer, sizeof(tx_buffer));
        return len;
    }

    void closePort();
    
private:
    bool mapBaudRate(uint32_t baudrate, int* baudrate_termios);

	const char* _device_name;
    uint8_t     _serial_id;
    uint32_t    _baudrate;
    uint8_t     _error;
    termios     _port_settings;
};

#endif
