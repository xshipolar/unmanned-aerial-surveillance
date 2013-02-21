/*
 * comsys.hpp
 *
 *  Created on: Jul 31, 2012
 *      Author: Xichen Shi
 */

#ifndef COMSYS_HPP_
#define COMSYS_HPP_

#include <UAS_comm.hpp>

extern UAS_serial Serial_apm;
extern UAS_serial Serial_gcs;
extern UAS_comm comModule;

void *run_apm_link(void*);

void *run_gcs_link(void*);

void comsys();

#endif /* COMSYS_HPP_ */
