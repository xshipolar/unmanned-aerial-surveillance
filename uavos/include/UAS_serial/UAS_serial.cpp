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
bool UAS_serial::beginPort(uint32_t baudrate){
    int baudr;
    _baudrate = baudrate;
    
    mapBaudRate(_baudrate, &baudr); // map baudrate to termios B-- style

    _serial_id = open(_device_name, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if(_serial_id == -1){
      perror("unable to open comport ");
      _error = -1;
      return 1;
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
      return 1;
    }
    
    return 0;
}

/**
 * @brief close the serial port
 */
void UAS_serial::closePort(){
  close(_serial_id);
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