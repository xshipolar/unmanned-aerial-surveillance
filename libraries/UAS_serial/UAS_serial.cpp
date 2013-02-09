/*
 *
 * Author: Xichen Shi
 * Derived from codes by Teunis van Beelen
 * Date created: Feb 8, 2013
 *
 */
 
#include "UAS_serial.hpp"

/**
 * @brief constructor of UAS_serial class
 */
UAS_serial::UAS_serial(const char* serial_name){
    _device_name = serial_name;
}

/**
 * @brief initializer for port
 * @param baud_rate -- bits-per-sec for the I/O
 */
int UAS_serial::beginPort(uint32_t baudrate){
    int baudr;
    _baudrate = baudrate;
    
    switch(baudrate){
      case      50 : baudr = B50;
                     break;
      case      75 : baudr = B75;
                     break;
      case     110 : baudr = B110;
                     break;
      case     134 : baudr = B134;
                     break;
      case     150 : baudr = B150;
                     break;
      case     200 : baudr = B200;
                     break;
      case     300 : baudr = B300;
                     break;
      case     600 : baudr = B600;
                     break;
      case    1200 : baudr = B1200;
                     break;
      case    1800 : baudr = B1800;
                     break;
      case    2400 : baudr = B2400;
                     break;
      case    4800 : baudr = B4800;
                     break;
      case    9600 : baudr = B9600;
                     break;
      case   19200 : baudr = B19200;
                     break;
      case   38400 : baudr = B38400;
                     break;
      case   57600 : baudr = B57600;
                     break;
      case  115200 : baudr = B115200;
                     break;
      case  230400 : baudr = B230400;
                     break;
      case  460800 : baudr = B460800;
                     break;
      case  500000 : baudr = B500000;
                     break;
      case  576000 : baudr = B576000;
                     break;
      case  921600 : baudr = B921600;
                     break;
      case 1000000 : baudr = B1000000;
                     break;
      default      : printf("invalid baudrate\n");
                     return(1);
                     break;
    } 

    _serial_id = open(_device_name, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if(_serial_id == -1){
      perror("unable to open comport ");
      _error = -1;
      return(1);
    }

    memset(&_port_settings, 0, sizeof(_port_settings));  /* clear the new struct */

    _port_settings.c_cflag = baudr | CS8 | CLOCAL | CREAD;
    _port_settings.c_iflag = 0;
    _port_settings.c_oflag = 0;
    _port_settings.c_lflag = 0;
    _port_settings.c_cc[VMIN] = 0;      /* block untill n bytes are received */
    _port_settings.c_cc[VTIME] = 0;     /* block untill a timer expires (n * 100 mSec.) */
    _error = tcsetattr(_serial_id, TCSANOW, &_port_settings);
    
    if(_error==-1){
      close(_serial_id);
      perror("unable to adjust portsettings ");
      return(1);
    }
    
    return(0);
}

/**
 * @brief fetch the next coming byte
 * @param c -- the char to be read into
 * @return length of the packet (bytes)
 */
int UAS_serial::fetch(unsigned char* c){
    int len;
    len = read(_serial_id, c, 1);
    return len;
}

/**
 * @brief send out a buffer of bytes
 * @param buf --array to be sent 
 * @return length of the packet (bytes)
 */
int UAS_serial::send(uint8_t* buf){
    int len;
    len = write(_serial_id, buf, sizeof(buf));
    return len;
}

/**
 * @brief close the serial port
 */
void UAS_serial::closePort()
{
  close(_serial_id);
}