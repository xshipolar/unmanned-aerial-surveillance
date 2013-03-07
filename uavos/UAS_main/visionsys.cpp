/*
 * visionsys.cpp
 *
 *  Created on: Mar 05, 2013
 *      Author: Xichen Shi
 */
#include <uisys.hpp>
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
    if(visionModule.isInitialised()==true)


    while(true){


    }
}