/*
 * UAS_nav.hpp
 *
 *  Created on: Mar 06, 2013
 *      Author: Shubham Gupta
 */

#ifndef UAS_NAV_HPP_
#define UAS_NAV_HPP_

#include <iostream>
#include <armadillo>
#include <cmath>
#include <datasys.hpp>

void init();

gimbal_state_t gimbal_calculate(double u, double v, double uc, double vc, double pitch, double yaw);

#endif /* UAS_NAV_HPP_ */