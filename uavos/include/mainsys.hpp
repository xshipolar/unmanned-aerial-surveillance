/*
 * mainsys.hpp
 *
 *  Created on: Feb 20, 2013
 *      Author: Xichen Shi
 */

#ifndef MAINSYS_HPP_
#define MAINSYS_HPP_

#include <UAS_serial.hpp>
#include <UAS_comm.hpp>
#include <utility.hpp>
#include <datasys.hpp>
#include <comsys.hpp>
#include <visionsys.hpp>
#include <uisys.hpp>

void initialize();

extern UAS_UI uiModule;

#endif /* MAINSYS_HPP_ */