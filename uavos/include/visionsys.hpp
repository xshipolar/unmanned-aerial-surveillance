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

struct centroid
{
    int x;
    int y;
 };

extern bool tracking_started;

void initVisionsys();

void* runVisionsys(void*);

centroid find_centroid();

void mouseHandler(int event, int x, int y, int flags, void *param);

#endif /* VISIONSYS_HPP_ */