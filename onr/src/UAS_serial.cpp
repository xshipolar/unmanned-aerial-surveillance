/*
 *
 * Author: Xichen Shi
 * Derived from codes by Teunis van Beelen
 * Date created: Feb 8, 2013
 *
 */
 
#include "UAS_serial.hpp"
#include <string>
#include <cstdio>
#include <cstring>
/**
 * @brief -- default constructor of UAS_serial class
 */
UAS_serial::UAS_serial(){
    // Initialise status data to false
    _port_opened = 0;
    _serial_id = -1;
}

/**
 * @brief -- constructor of UAS_serial class with serial name defined
 */
UAS_serial::UAS_serial(const char* serial_name){
    _device_name = serial_name;
    // Initialise status data to false
    _port_opened = 0;
    _serial_id = -1;
}

/**
 * @brief -- constructor of UAS_serial class with serial name defined
 */
UAS_serial::UAS_serial(const std::string serial_name){
    _device_name = serial_name.c_str();
    // Initialise status data to false
    _port_opened = 0;
    _serial_id = -1;
}

/**
 * @brief initializer for port, provided that device name is defined
 * @param baud_rate -- bits-per-sec for the I/O
 * @return -- 1 for success 0 for failure
 */
bool UAS_serial::beginPort(uint32_t baudrate){
    int baudr;
    _baudrate = baudrate;
    
    mapBaudRate(_baudrate, &baudr); // map baudrate to termios B-- style

    _serial_id = open(_device_name, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if(_serial_id == -1){
      perror("unable to open comport ");
      _error = -1;
      _port_opened = 0;
      return _port_opened;
    }

    memset(&_port_settings, 0, sizeof(_port_settings));  // setting memory for port
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
      _port_opened = 0;
      return _port_opened;
    }
    
    flushIO();
    _port_opened = 1;
    return _port_opened;
}

/**
 * @brief initializer for port, provided that device name is defined
 * @param serial_name -- name of the serial device to be opened
 * @param baud_rate -- bits-per-sec for the I/O
 * @return -- 1 for success 0 for failure
 */
bool UAS_serial::beginPort(const char* serial_name, uint32_t baudrate){
    int baudr;
    _baudrate = baudrate;
    _device_name = serial_name;
    
    mapBaudRate(_baudrate, &baudr); // map baudrate to termios B-- style

    _serial_id = open(_device_name, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if(_serial_id == -1){
      perror("unable to open comport ");
      _error = -1;
      _port_opened = 0;
      return _port_opened;
    }

    memset(&_port_settings, 0, sizeof(_port_settings));  // setting memory for port
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
      _port_opened = 0;
      return _port_opened;
    }
    
    flushIO();
    _port_opened = 1;
    return _port_opened;
}

/**
 * @brief initializer for port, provided that device name is defined
 * @param serial_name -- name of the serial device to be opened
 * @param baud_rate -- bits-per-sec for the I/O
 * @return -- 1 for success 0 for failure
 */
bool UAS_serial::beginPort(const std::string serial_name, uint32_t baudrate){
    int baudr;
    _baudrate = baudrate;
    _device_name = serial_name.c_str();
    
    mapBaudRate(_baudrate, &baudr); // map baudrate to termios B-- style

    _serial_id = open(_device_name, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if(_serial_id == -1){
      perror("unable to open comport ");
      _error = -1;
      _port_opened = 0;
      return _port_opened;
    }

    memset(&_port_settings, 0, sizeof(_port_settings));  // setting memory for port
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
      _port_opened = 0;
      return _port_opened;
    }
    
    flushIO();
    _port_opened = 1;
    return _port_opened;
}

/**
 * @brief flush the IO buffer
 */
void UAS_serial::flushIO(){
  tcflush(_serial_id, TCIOFLUSH);
}

/**
 * @brief get available bytes
 * @return -- avaiable bytes on input buffer
 */
int UAS_serial::available(){
  int available_bytes;
  ioctl(_serial_id, FIONREAD, &available_bytes);
  return available_bytes;
}

/**
 * @brief close the serial port
 */
void UAS_serial::closePort(){
  _port_opened = 0;
  close(_serial_id);
}

/**
 * @brief get the name of the port
 * @return -- string of the name of the port
 */
const char* UAS_serial::getDeviceName(){
  return _device_name;
}

/**
 * @brief get the open status of the port
 * @return -- _port_opened
 */
bool UAS_serial::isOpened(){
  return _port_opened;
}

/**
 * @brief map 32bit baudrate setting to termios baudrate setting
 * @param baudrate -- raw 32 bit number for baudrate
 * @param baudrate_termios -- termios specific number for baudrate
 * @return error code -- 0 for success, 1 for invalid 
 */
bool UAS_serial::mapBaudRate(uint32_t baudrate, int* baudrate_termios){
  switch(baudrate){
      case      50 : *baudrate_termios = B50;
                     break;
      case      75 : *baudrate_termios = B75;
                     break;
      case     110 : *baudrate_termios = B110;
                     break;
      case     134 : *baudrate_termios = B134;
                     break;
      case     150 : *baudrate_termios = B150;
                     break;
      case     200 : *baudrate_termios = B200;
                     break;
      case     300 : *baudrate_termios = B300;
                     break;
      case     600 : *baudrate_termios = B600;
                     break;
      case    1200 : *baudrate_termios = B1200;
                     break;
      case    1800 : *baudrate_termios = B1800;
                     break;
      case    2400 : *baudrate_termios = B2400;
                     break;
      case    4800 : *baudrate_termios = B4800;
                     break;
      case    9600 : *baudrate_termios = B9600;
                     break;
      case   19200 : *baudrate_termios = B19200;
                     break;
      case   38400 : *baudrate_termios = B38400;
                     break;
      case   57600 : *baudrate_termios = B57600;
                     break;
      case  115200 : *baudrate_termios = B115200;
                     break;
      case  230400 : *baudrate_termios = B230400;
                     break;
      case  460800 : *baudrate_termios = B460800;
                     break;
      case  500000 : *baudrate_termios = B500000;
                     break;
      case  576000 : *baudrate_termios = B576000;
                     break;
      case  921600 : *baudrate_termios = B921600;
                     break;
      case 1000000 : *baudrate_termios = B1000000;
                     break;
      default      : printf("invalid baudrate\n");
                     return 1; // error code 1
                     break;
    }
    return 0; // no error
}