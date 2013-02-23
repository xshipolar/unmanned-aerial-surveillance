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
    int fetch(serial_buffer* c){
        int len;
        len = read(_serial_id, c, 1);
        return len;
    }

/**
 * @brief send out a buffer of bytes
 * @param tx_buffer --array to be sent 
 * @return length of the packet (bytes)
 */
    template <class serial_buffer>
    int send(serial_buffer* tx_buffer, size_t count){
        int len;
        len = write(_serial_id, tx_buffer, count);
        return len;
    }

    void closePort();

    // Get info functions
    const char* getDeviceName();
    
private:
    bool mapBaudRate(uint32_t baudrate, int* baudrate_termios);

    const char* _device_name;
    int8_t      _serial_id;
    uint32_t    _baudrate;
    int8_t     _error;
    termios     _port_settings;
};

#endif
