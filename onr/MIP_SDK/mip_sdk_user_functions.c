/////////////////////////////////////////////////////////////////////////////
//
//! @file    mip_sdk_user_functions.c 
//! @author  Nathan Miller
//! @version 1.0
//
//! @description Target-Specific Functions Required by the MIP SDK
//
// External dependencies:
//
//  mip.h
// 
//! @copyright 2011 Microstrain. 
//
//!@section LICENSE
//!
//! THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING 
//! CUSTOMERS WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER 
//! FOR THEM TO SAVE TIME. AS A RESULT, MICROSTRAIN SHALL NOT BE HELD LIABLE 
//! FOR ANY DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY 
//! CLAIMS ARISING FROM THE CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY 
//! CUSTOMERS OF THE CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH 
//! THEIR PRODUCTS.
//
/////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//
//Include Files
//
////////////////////////////////////////////////////////////////////////////////


#include "mip_sdk_user_functions.h"
//#include <windows.h>


/////////////////////////////////////////////////////////////////////////////
//
//! @fn
//! u16 mip_sdk_port_open(void *port_handle, int port_num, int baudrate)
//
//! @section DESCRIPTION
//! Target-Specific communications port open. 
//
//! @section DETAILS
//!
//! @param [out] void *port_handle - target-specific port handle pointer (user needs to allocate memory for this)
//! @param [in] int port_num       - port number (as recognized by the operating system.)
//! @param [in] int baudrate       - baudrate of the com port.
//
//! @retval MIP_USER_FUNCTION_ERROR  When there is a problem opening the port.\n
//! @retval MIP_USER_FUNCTION_OK     The open was successful.\n
//
//! @section NOTES
//! 
//! The user should copy the \c mip_sdk_user_functions.c file to their project directory and\n
//! edit it as needed to support their target operating system.
//!
//
/////////////////////////////////////////////////////////////////////////////
static int Serial1;
static struct termios Serial1_setting;

int mapBaudRate(uint32_t baudrate, int* baudrate_termios){
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

u16 mip_sdk_port_open(void **port_handle, const char *port_name, int baudrate) {
    // Get device name
    // char port_name[15];
    // sprintf(port_name,"/dev/ttyACM%d",port_name);

    // Begin port with device name and baudrate 
    int baudr;
    mapBaudRate(baudrate, &baudr); // map baudrate to termios B-- style

    Serial1 = open(port_name, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if(Serial1 == -1){
      perror("unable to open comport ");
      return MIP_USER_FUNCTION_ERROR;
    }

    memset(&Serial1_setting, 0, sizeof(Serial1_setting));  // setting memory for port
    Serial1_setting.c_cflag = baudr | CS8 | CLOCAL | CREAD;
    Serial1_setting.c_iflag = 0;
    Serial1_setting.c_oflag = 0;
    Serial1_setting.c_lflag = 0;
    Serial1_setting.c_cc[VMIN] = 0;      /* block untill n bytes are received */
    Serial1_setting.c_cc[VTIME] = 0;     /* block untill a timer expires (n * 100 mSec.) */
    int error = tcsetattr(Serial1, TCSANOW, &Serial1_setting);
    
    if(error==-1){
      close(Serial1);
      perror("unable to adjust portsettings ");
      return MIP_USER_FUNCTION_ERROR;
    }

    *port_handle = &Serial1;
    return MIP_USER_FUNCTION_OK;
}


/////////////////////////////////////////////////////////////////////////////
//
//! @fn
//! u16 mip_sdk_port_close(void *port_handle)
//
//! @section DESCRIPTION
//! Target-Specific port close function 
//
//! @section DETAILS
//!
//! @param [in] void *port_handle - target-specific port handle pointer (user needs to allocate memory for this)
//
//! @retval MIP_USER_FUNCTION_ERROR  When there is a problem closing the port.\n
//! @retval MIP_USER_FUNCTION_OK     The close was successful.\n
//
//! @section NOTES
//! 
//! The user should copy the \c mip_sdk_user_functions.c file to their project directory and\n
//! edit it as needed to support their target operating system.
//!
//
/////////////////////////////////////////////////////////////////////////////

u16 mip_sdk_port_close(void *port_handle) {
    int error = close( *((int*)port_handle) ) ;
    if(error == 0) {
        return MIP_USER_FUNCTION_OK; 
    } else {
        perror("unable to close port");
        return MIP_USER_FUNCTION_ERROR;
    }
}


/////////////////////////////////////////////////////////////////////////////
//
//! @fn
//! u16 mip_sdk_port_write(void *port_handle, u8 *buffer, u32 num_bytes, u32 *bytes_written, u32 timeout_ms)
//
//! @section DESCRIPTION
//! Target-Specific Port Write Function. 
//
//! @section DETAILS
//!
//! @param [in]  void *port_handle  - target-specific port handle pointer (user needs to allocate memory for this)
//! @param [in]  u8 *buffer         - buffer containing num_bytes of data
//! @param [in]  u32 num_bytes      - the number of bytes to write to the port
//! @param [out] u32 *bytes_written - the number of bytes actually written to the port
//! @param [in]  u32 timeout_ms     - the write timeout
//
//! @retval MIP_USER_FUNCTION_ERROR  When there is a problem communicating with the port.\n
//! @retval MIP_USER_FUNCTION_OK     The write was successful.\n
//
//! @section NOTES
//! 
//! The user should copy the \c mip_sdk_user_functions.c file to their project directory and\n
//! edit it as needed to support their target operating system.
//!
//
/////////////////////////////////////////////////////////////////////////////

u16 mip_sdk_port_write(void *port_handle, u8 *buffer, u32 num_bytes, u32 *bytes_written, u32 timeout_ms) {
    *bytes_written = 0;

    *bytes_written = write( *((int*)port_handle), buffer, num_bytes);
    if (*bytes_written > 0){
        return MIP_USER_FUNCTION_OK;
    } else {
        return MIP_USER_FUNCTION_ERROR;
    }
}


/////////////////////////////////////////////////////////////////////////////
//
//! @fn
//! u16 mip_sdk_port_read(void *port_handle, u8 *buffer, u32 num_bytes, u32 *bytes_read, u32 timeout_ms)
//
//! @section DESCRIPTION
//! Target-Specific Port Write Function. 
//
//! @section DETAILS
//!
//! @param [in]  void *port_handle  - target-specific port handle pointer (user needs to allocate memory for this)
//! @param [in]  u8 *buffer         - buffer containing num_bytes of data
//! @param [in]  u32 num_bytes      - the number of bytes to write to the port
//! @param [out] u32 *bytes_read    - the number of bytes actually read from the device
//! @param [in]  u32 timeout_ms     - the read timeout
//
//! @retval MIP_USER_FUNCTION_ERROR  When there is a problem communicating with the port.\n
//! @retval MIP_USER_FUNCTION_OK     The read was successful.\n
//
//! @section NOTES
//! 
//! The user should copy the \c mip_sdk_user_functions.c file to their project directory and\n
//! edit it as needed to support their target operating system.
//!
//
/////////////////////////////////////////////////////////////////////////////

u16 mip_sdk_port_read(void *port_handle, u8 *buffer, u32 num_bytes, u32 *bytes_read, u32 timeout_ms) {
    *bytes_read = 0;

    *bytes_read = read( *((int*) port_handle), buffer, num_bytes);
    if (*bytes_read > 0) {
        return MIP_USER_FUNCTION_OK;
    } else {
        return MIP_USER_FUNCTION_ERROR;
    }
}


/////////////////////////////////////////////////////////////////////////////
//
//! @fn
//! u32 mip_sdk_port_read_count(void *port_handle)
//
//! @section DESCRIPTION
//! Target-Specific Function to Get the Number of Bytes Waiting on the Port.
//
//! @section DETAILS
//!
//! @param [in]  void *port_handle  - target-specific port handle pointer (user needs to allocate memory for this)
//
//! @returns  Number of bytes waiting on the port,\n
//!           0, if there is an error.
//
//! @section NOTES
//! 
//! The user should copy the \c mip_sdk_user_functions.c file to their project directory and\n
//! edit it as needed to support their target operating system.
//!
//
/////////////////////////////////////////////////////////////////////////////

u32 mip_sdk_port_read_count(void *port_handle) {
    u32 available = 0;
    ioctl(*((int*)port_handle), TIOCINQ, &available);
    return available;
}


/////////////////////////////////////////////////////////////////////////////
//
//! @fn
//! u32 mip_sdk_get_time_ms()
//
//! @section DESCRIPTION
//! Target-Specific Call to get the current time in milliseconds.
//
//! @section DETAILS
//!
//! @param [in]  void *port_handle  - target-specific port handle pointer (user needs to allocate memory for this)
//
//! @returns  Current time in milliseconds.
//
//! @section NOTES
//! 
//!   1) This value should no roll-over in short periods of time (e.g. minutes)\n
//!   2) Most systems have a millisecond counter that rolls-over every 32 bits\n
//!      (e.g. 49.71 days roll-over period, with 1 millisecond LSB)\n
//!   3) An absolute reference is not required since this function is\n
//!      used for relative time-outs.\n
//!   4) The user should copy the \c mip_sdk_user_functions.c file to their project directory and\n
//!      edit it as needed to support their target operating system.
//!
//
/////////////////////////////////////////////////////////////////////////////

u32 mip_sdk_get_time_ms(){
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    unsigned long usec = currentTime.tv_sec * 1000 + currentTime.tv_usec/1000;
    return usec;
}