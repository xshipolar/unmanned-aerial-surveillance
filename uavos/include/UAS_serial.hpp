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
#include <sys/ioctl.h>
#include <unistd.h>
#include <string>

class UAS_serial {
public:
    // Constructors
    UAS_serial();
    UAS_serial(const char* serial_name);
    UAS_serial(const std::string serial_name);

    // Initialiser
    bool beginPort(uint32_t baudrate);
    bool beginPort(const char* serial_name, uint32_t baudrate);
    bool beginPort(const std::string serial_name, uint32_t baudrate);

    void flushIO();
    int getIncomingBytes();
    int getOutgoingBytes();
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
    bool isOpened();
    
private:
    bool mapBaudRate(uint32_t baudrate, int* baudrate_termios);

    const char* _device_name;
    int8_t      _serial_id;
    uint32_t    _baudrate;
    int8_t      _error;
    termios     _port_settings;

    bool _port_opened; // status of the port
};

#endif
