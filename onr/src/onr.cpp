/*
 * Video Sync code for onr research
 *
 * Created on : 07-01-2012
 *     Author : Xichen Shi
 *       Rev  : 10-22-2012
 */

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>
#include <inttypes.h>
#include <ctime>
#include <cstdio>

#include "../include/mavlink/mavlink_types.h"
#include "../include/mavlink/ardupilotmega/mavlink.h"

using namespace std;
using namespace boost;
using namespace boost::asio;

/** Variables **/
const int bdRate = 115200;
string serialName = "/dev/ttyS0";
serial_port_base::parity parityOption = serial_port_base::parity( serial_port_base::parity::none );
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
    io_service ioServ;
    serial_port comPort( ioServ, serialName);
    deadline_timer timeout( ioServ );

    // serial_port setting
    if (comPort.is_open())
    {
        comPort.set_option( serial_port_base::baud_rate(bdRate) );
        cout << "set baud rate: "<< bdRate <<endl;
        comPort.set_option( serial_port_base::flow_control( serial_port_base::flow_control::none ) );
        cout << "set flow control: none" <<endl;
        comPort.set_option( parityOption );
        cout << "set parity: none" <<endl;
        comPort.set_option( serial_port_base::stop_bits( serial_port_base::stop_bits::one ) );
        cout << "set stop bits: 1" <<endl;
        comPort.set_option( serial_port_base::character_size( serial_port_base::character_size(8)) );
        cout << "set character size: 8" <<endl;
    }
  
    // check status of the serial port
    if (comPort.is_open())
    {
        cout << "Serial port "<< serialName <<" is openned now" <<endl;
    }
    else
    {
        cout << "Serial port "<< serialName <<" is NOT available" <<endl;
        return -1;
    }

    // serial_port reading and writing
    int count = 0;
    for(;;)
    {   
        // restart the io service test if it is stopped
        if( ioServ.stopped() )
	    {
	        ioServ.reset();
	    }
        mavlink_message_t msg
        readCharAsync(char &c, &comPort, &timeout, &ioServ, 100);
        if (mavlink_parse_char(0, c, &msg, &status))
        {
            //printf("Received message with ID %d, sequence: %d from component %d of system %d \n", msg.msgid, msg.seq, msg.compid, msg.sysid);
            switch (msg.id) {
			
			case MAVLINK_MSG_ID_RAW_IMU:
			    mavlink_msg_raw_imu_decode(&msg, mavlink_raw_imu_t* raw_imu)
			    printf("%d : xacc = %d mg, yacc = %d mg, zacc = %d mg, xgyro = %d mrad/s, ygyro = %d mrad/s, zgyro = %d mrad/s",
				        raw_imu->time_usec,raw_imu->xacc,raw_imu->yacc,raw_imu->zacc,raw_imu->xgyro,raw_imu->ygyro,raw_imu->zgyro);
				break;
				
			case MAVLINK_MSG_ID_ATTITUDE:

			    break;
			default:
			    break;
			}
        }
	    /** increase main loop count **/
	    count++;
    }
    comPort.close();
    return 0;
}
