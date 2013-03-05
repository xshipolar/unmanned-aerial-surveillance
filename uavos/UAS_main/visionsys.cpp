/*
 * visionsys.cpp
 *
 *  Created on: Mar 05, 2013
 *      Author: Xichen Shi
 */

#include <visionsys.hpp>

static UAS_tld visionModule;

/**
 * @brief initialise vision system
 */
void initVisionsys(){
    visionModule.init();
}

/**
 * @brief 
 */
void* runVisionsys(void*){
    while(true){

    }
}