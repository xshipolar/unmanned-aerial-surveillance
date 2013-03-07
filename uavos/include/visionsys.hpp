/*
 * visionsys.hpp
 *
 *  Created on: Mar 05, 2013
 *      Author: Xichen Shi
 */

#ifndef VISIONSYS_HPP_
#define VISIONSYS_HPP_

#include <UAS_tld.h>
#include <datasys.hpp>
#include <cstdio>
#include <utility.hpp>

void initVisionsys();

void* runVisionsys(void*);

void mouseHandler(int event, int x, int y, int flags, void *param);

#endif /* VISIONSYS_HPP_ */