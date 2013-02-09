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
    int beginPort(uint32_t baudrate);
    int fetch(unsigned char* c);
    int send(uint8_t* buf);
    void closePort();
    
private:
	const char* _device_name;
    uint8_t     _serial_id;
    uint32_t    _baudrate;
    uint8_t     _error;
    termios     _port_settings;
};

#endif
