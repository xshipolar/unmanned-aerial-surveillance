/*
 * UART test code using asio under boost c++ library
 *
 * Created on : 07-01-2012
 *     Author : Xichen Shi
 *       Rev  : 07-30-2012
 */

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>
#include <inttypes.h>
#include <ctime>
#include <cstdio>

#include "../include/mavlink/mavlink_types.h"
#include "../include/mavlink/common/mavlink.h"

using namespace std;
using namespace boost;
using namespace boost::asio;

/** Variables **/
const int bdRate = 115200;
string serialName = "/dev/ttyS0";
serial_port_base::parity parityOption = serial_port_base::parity( serial_port_base::parity::even );
mavlink_heartbeat_t mavlink_heartbeat;
mavlink_heartbeat_t mavlink_heartbeat_1;
mavlink_message_t msg;
mavlink_system_t mavlink_system;
mavlink_status_t status;
uint8_t buf[MAVLINK_MAX_PACKET_LEN];

/** Functions **/

/**
 * @brief Call back function for async_read to determine if accomplished
 * @param error -- if error has occured for asynchronous operation
 * @param bytes_transferred -- bytes read by function
 * @param timeout -- class ptr to a deadline_timer class
 * @return NULL
 */
void readCallback(const boost::system::error_code& error,size_t bytes_transferred, deadline_timer* timeout)
{
    if(error || bytes_transferred == 0)
    {
        cout << " no bytes transferred " << endl;
        return;
    }
    timeout->cancel();
    //cout << "Receive data complete: " << bytes_transferred << " bytes" <<endl;
    return;
}


/**
 * @brief Call back function for deadline_timer to determine if expired
 * @param error -- if error has occured for asynchronous operation
 * @param serial -- class ptr to a serial_port class
 * @return NULL
 */
void waitCallback(const boost::system::error_code& error, serial_port* serial)
{
    if(error)
    {
        return;
    }
    serial->cancel();
    //cout << "Receive channel timeout" << endl;
    return;
}

/**
 * @brief serial function for reading with timeout
 * @param c -- char that read value will be stored
 * @param serial -- serial class for the port
 * @param timeout -- timerclass for countdown
 * @param io -- io service class for low level function
 * @param ms --
 * @return NULL
 */
void readCharAsync(char& c, serial_port* serial, deadline_timer* timeout, io_service* io, int ms)
{
    //deadline_timer timeout( ioserviceTest);
    async_read( *serial, mutable_buffers_1(&c,1) , boost::bind( readCallback, placeholders::error, placeholders::bytes_transferred, timeout ) );
    timeout->expires_from_now(posix_time::milliseconds(ms));
    timeout->async_wait( boost::bind( waitCallback, placeholders::error, serial ) );
    io->run();
    return;
}

int main()
{   
    /** Method to Construct Serial port from boost/asio directly **/
    // Construct io_service and serial_port
    io_service ioserviceTest;
    serial_port serialTest( ioserviceTest, serialName);
    deadline_timer timeout( ioserviceTest );

    // serial_port setting
    if (serialTest.is_open())
    {
        serialTest.set_option( serial_port_base::baud_rate(bdRate) );
        cout << "set baud rate: "<< bdRate <<endl;
        serialTest.set_option( serial_port_base::flow_control( serial_port_base::flow_control::none ) );
        cout << "set flow control: none" <<endl;
        serialTest.set_option( parityOption );
        cout << "set parity: even" <<endl;
        serialTest.set_option( serial_port_base::stop_bits( serial_port_base::stop_bits::one ) );
        cout << "set stop bits: 1" <<endl;
        serialTest.set_option( serial_port_base::character_size( serial_port_base::character_size(8)) );
        cout << "set character size: 8" <<endl;
    }

    /** Method to Construct Serial port from Timeoutserial.hpp **
	*************************************************************
    TimeoutSerial serialTest("/dev/ttyS0",bdRate,parityOption); 
    serialTest.setTimeout(posix_time::seconds(5));
	**************************************************************/
    
    // check status of the serial port
    if (serialTest.is_open())
    {
        cout << "Serial port "<< serialName <<" is openned now" <<endl;
    }
    else
    {
        cout << "Serial port "<< serialName <<" is NOT available" <<endl;
        return -1;
    }

    // Mavlink System
    mavlink_system.sysid = 1;
    mavlink_system.compid = MAV_COMP_ID_SYSTEM_CONTROL;
    mavlink_system.type = MAV_TYPE_FIXED_WING;

    // Mavlink Heartbeat
    mavlink_heartbeat.custom_mode = 0;
    mavlink_heartbeat.type = MAV_TYPE_FIXED_WING;
    mavlink_heartbeat.autopilot = MAV_AUTOPILOT_GENERIC;
    mavlink_heartbeat.base_mode = MAV_MODE_FLAG_TEST_ENABLED;
    mavlink_heartbeat.system_status = MAV_STATE_STANDBY;
    mavlink_heartbeat.mavlink_version = 0;

    // serial_port reading and writing
    int count = 0;
    int sendTimer = 0;
    int receiveTimer = 0;
    bool parsed;
    char readChar;
    for(;;)
    {   
        // restart the io service test if it is stopped
        if( ioserviceTest.stopped() )
	    {
	        ioserviceTest.reset();
	    }

        /*** Debugging for timer ***/
        //printf(" %d ; %d ; %d \n",  clock(), sendTimer , CLOCKS_PER_SEC);
        /** Heartbeat pack and send (try running at 1 HZ **/
        if( parsed || count == 0 )
        {
        	parsed = false;
            sendTimer = clock();
            cout << " Sending heartbeat ..... ";
            mavlink_msg_heartbeat_encode(mavlink_system.sysid, mavlink_system.compid, &msg, &mavlink_heartbeat);
            uint16_t len = mavlink_msg_to_send_buffer(buf, &msg); 
            write( serialTest, mutable_buffers_1(&buf, sizeof(buf) ) );
            cout << "..... sent " << len << " bytes" <<endl;
        }

        readCharAsync(readChar, &serialTest, &timeout, &ioserviceTest, 100);
        if (mavlink_parse_char(0, readChar, &msg, &status))
        {
        	parsed = true;
            printf("Received message with ID %d, sequence: %d from component %d of system %d \n", msg.msgid, msg.seq, msg.compid, msg.sysid);
            mavlink_msg_heartbeat_decode(&msg, &mavlink_heartbeat_1);
            printf("mavlink heartbeat decoded");
        }
	    /** increase main loop count **/
	    count++;
    }
    serialTest.close();
    return 0;
}
