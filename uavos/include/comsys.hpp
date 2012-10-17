/*
 * comsys.hpp
 *
 *  Created on: Jul 31, 2012
 *      Author: Xichen Shi
 */

#ifndef COMSYS_HPP_
#define COMSYS_HPP_

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iostream>
#include <inttypes.h>
#include <ctime>
#include <cstdio>

#define CLOCK_FUNCTION clock()

using namespace std;
using namespace boost;
using namespace boost::asio;

//class thread_data;

/**
 * @brief set options for serialport
 * @param serialPort -- serial class to be set
 * @return state of success -- 1 for success, 0 for fail
 */
int configPort(serial_port& serialPort);

/**
 * @brief Call back function for async_read to determine if accomplished
 * @param error -- if error has occured for asynchronous operation
 * @param bytes_transferred -- bytes read by function
 * @param timeout -- class ptr to a deadline_timer class
 * @return NULL
 */
void readCallback(const boost::system::error_code& error,size_t bytes_transferred, deadline_timer* timeout);

/**
 * @brief Call back function for deadline_timer to determine if expired
 * @param error -- if error has occured for asynchronous operation
 * @param serial -- class ptr to a serial_port class
 * @return NULL
 */
void waitCallback(const boost::system::error_code& error, serial_port* serial);

/**
 * @brief serial function for reading with timeout
 * @param c -- char that read value will be stored
 * @param serial -- serial class for the port
 * @param timeout -- timerclass for countdown
 * @param io -- io service class for low level function
 * @param ms -- timeout in millisecond
 * @return NULL
 */
void readCharAsync(char& c, serial_port* serial, deadline_timer* timeout, io_service* io, int ms);

/**
 * @brief calling continuously for parsing data
 * @param thread_data->serial -- ptr to serial_port class for receiving
 * @param thread_data->ioService -- ptr to io_service class
 * @return
 */
void *parseData(void *threadArg);

/**
 * @brief calling continuously for sending data
 * @param thread_data->serial -- ptr to serial class for sending
 * @return
 */
void *packData(void *threadArg);

/**
 * @brief Communicatio0n module to be called in main
 * @param
 * @return
 */
void comModule();

#endif /* COMSYS_HPP_ */
