/*
 * visionsys.hpp
 *
 *  Created on: Mar 05, 2013
 *      Author: Xichen Shi
 */

#ifndef VISIONSYS_HPP_
#define VISIONSYS_HPP_

#include <datasys.hpp>
#include <cstdio>
#include <utility.hpp>

// New opentld class
#include "Main.h"
#include "Config.h"
#include "ImAcq.h"
#include "Gui.h"

struct centroid
{
    int x;
    int y;
 };

void initVisionsys(int argc, char **argv);

void* runVisionsys(void*);

centroid find_centroid();

#endif /* VISIONSYS_HPP_ */