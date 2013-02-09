/*
 *
 * Author: Xichen Shi
 * Derived from codes by Teunis van Beelen
 * Date created: Feb 8, 2013
 *
 */
 
#ifndef UAS_serial
#define UAS_serial

#include <termios.h>
#include <stdio>
#include <inttypes.h>
#include <fcntl.h>

class UAS_serial {
public:
    UAS_serial(const char* serial_name, int baudrate);
    int fetch(unsigned char* c);
    int send(uint8_t* buf);
    void closePort();
    
private:
    uint8_t  _serial_id;
    uint32_t _baudrate;
    uint8_t  _error;
    termios  _port_settings;
};

#endif
