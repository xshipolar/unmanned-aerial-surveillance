/*
 * File name   : comsys.cpp
 *
 * Description : Communication subsystem
 *
 * Created on  : Jul 31, 2012
 *
 * Author      : Xichen Shi
 */

#include <comsys.hpp>
#include <uavos.hpp>
#include <mavlink/common/mavlink.h>
#include <pthread.h>

/**
 * @brief Class for handling thread inputs
 * @param serial -- class to be used in comsys
 * @param ioService -- class associated with serial
 */
struct thread_data
{
	public:
		boost::asio::io_service ioService;
		boost::asio::serial_port serial;

    thread_data(string port, unsigned int baud_rate) : ioService(), serial(ioService, port)
	{
    	if(configPort(serial))
    	{
    		serial.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
    	}
	}
};

/**
 * @brief set options for serialport
 * @param serialPort -- serial class to be set
 * @return state of success -- 1 for success, 0 for fail
 */
int configPort(serial_port& serialPort)
{
	int configState;
    // serial_port setting
    if (serialPort.is_open())
    {
        serialPort.set_option( rateOption );
        //cout << "set baud rate: "<< bdRate <<endl;
        serialPort.set_option( flowOption );
        //cout << "set flow control: none" <<endl;
        serialPort.set_option( parityOption );
        //cout << "set parity: even" <<endl;
        serialPort.set_option( bitOption );
        //cout << "set stop bits: 1" <<endl;
        serialPort.set_option( sizeOption );
        //cout << "set character size: 8" <<endl;
        configState = 1;
    } else {
    	configState = 0;
    }
	return configState;
}

/**
 * @brief Call back function for async_read to determine if accomplished
 * @param error -- if error has occured for asynchronous operation
 * @param bytes_transferred -- bytes read by function
 * @param timeout -- class ptr to a deadline_timer class
 * @return NULL
 */
void readCallback(const boost::system::error_code& error,
		size_t bytes_transferred, deadline_timer* timeout)
{
    if(error || bytes_transferred == 0)
    {
        return;
    }
    timeout->cancel();
    return;
}

/**
 * @brief Call back function for deadline_timer to determine if expired
 * @param error -- if error has occured for asynchronous operation
 * @param serial -- class ptr to a serial_port class
 * @return NULL
 */
void waitCallback(const boost::system::error_code& error,
		serial_port* serial)
{
    if(error)
    {
        return;
    }
    serial->cancel();
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
void readCharAsync(char& c, serial_port* serial, deadline_timer* timeout,
		io_service* io, int ms)
{
    //deadline_timer timeout( ioserviceTest);
    async_read( *serial, mutable_buffers_1(&c,1) , boost::bind( readCallback, placeholders::error, placeholders::bytes_transferred, timeout ) );
    timeout->expires_from_now(posix_time::milliseconds(ms));
    timeout->async_wait( boost::bind( waitCallback, placeholders::error, serial ) );
    io->run();
    return;
}

/**
 * @brief calling continuously for parsing data
 * @param serial -- ptr to serial_port class for receiving
 * @param ioService -- ptr to io_service class
 * @return
 */
void *parseData( void *threadArg )
{
	/** Handling inputs from class **/
	thread_data* data = (thread_data*) threadArg;
	serial_port serial = data->serial;
    io_service ioService = data->ioService;

    /** Temporary containers for message handling **/
	char readChar;
	mavlink_message_t msg;
	mavlink_status_t status;

	/** Timer for timeout action **/
	deadline_timer receiveTimer(ioService);

	/** Loop for receiving data **/
	for(;;)
	{
	    readCharAsync(readChar, &serial, &receiveTimer, &ioService, 100);
	    if (mavlink_parse_char(MAVLINK_COMM_0, readChar, &msg, &status))
	    {
	    	printf("Received message with ID %d,"
	    			" sequence: %d from component %d of system %d \n",
	    			msg.msgid, msg.seq, msg.compid, msg.sysid);
	    	switch(msg.msgid)
	    	{
	    	case MAVLINK_MSG_ID_HEARTBEAT:
	    		mavlink_msg_heartbeat_decode(&msg, &autopilot_heartbeat);
	    		break;
	    	default:
	    		break;
	    	}
	    }
	}
	return 0;
}

/**
 * @brief calling continuously for sending data
 * @param serial -- ptr to serial class for sending
 * @return
 */
void *packData( void *threadArg )
{
	/** Handling inputs from class **/
	thread_data* data = (thread_data*) threadArg;
    serial_port serial = data->serial;

	/** Temporary containers for message handling **/
	mavlink_message_t msg;
	uint8_t buf[MAVLINK_MAX_PACKET_LEN];
	uint16_t len; //message length sent

	/** Timer for loop frequency handling **/
    int heartbeatTimer;
    int targetinfoTimer;

    /** loop for Tx data **/
	for(;;)
	{
		/* Send Heartbeat */
		if( CLOCK_FUNCTION - heartbeatTimer > 1000)
		{
			heartbeatTimer = CLOCK_FUNCTION;
		    mavlink_msg_heartbeat_encode(pico_system.sysid, pico_system.compid, &msg, &pico_heartbeat);
		    len = mavlink_msg_to_send_buffer( buf, &msg );
		    write( serial, mutable_buffers_1( &buf, sizeof(buf) ) );
		    printf("heartbeat sent");
	    }

		/* Send Target Info */
		if( CLOCK_FUNCTION - targetinfoTimer > 20)
		{
            targetinfoTimer = CLOCK_FUNCTION;
			mavlink_msg_vision_position_estimate_encode(pico_system.sysid, pico_system.compid, &msg, &pico_target_position_estimate);
			len = mavlink_msg_to_send_buffer( buf, &msg );
			write( serial, mutable_buffers_1( &buf, sizeof(buf) ) );
			printf("targetinfo sent");
		}
	}
	return 0;
}

void comModule()
{
	//io_service io0;
	//serial_port uart0(io0, uart0Name);
	//configPort(uart0);

	// thread inputs
	thread_data threadInput(uart0Name, 115200);
	//threadInput.serial = uart0;
	//threadInput.ioService = io0;

	// thread creation
	pthread_t threadRx, threadTx;
	int  iretRx, iretTx;

	// thread exectution
	iretRx = pthread_create( &threadRx, NULL, parseData, (void*) &threadInput);
	iretTx = pthread_create( &threadTx, NULL, packData, (void*) &threadInput);

	// thread sync
	pthread_join( threadRx, NULL);
	pthread_join( threadTx, NULL);
}
