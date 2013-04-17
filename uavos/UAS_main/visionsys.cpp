/*
 * visionsys.cpp
 *
 *  Created on: Mar 05, 2013
 *      Author: Xichen Shi
 */
#include <visionsys.hpp>

using tld::Config;
using tld::Gui;
using tld::Settings;

static Main *opentld_main;
static Config opentld_config;
static ImAcq *imAcq;
static Gui *gui;
static unsigned long last_time_visionsys;

/**
 * @brief initialise vision system
 */
void initVisionsys(int argc, char **argv){
    last_time_visionsys = getMilliSeconds();
    opentld_main = new Main();
    imAcq = imAcqAlloc();
    gui = new Gui();
    
    opentld_config.init(argc, argv);

    opentld_main->gui = gui;
    opentld_main->imAcq = imAcq;
    opentld_config.configure(opentld_main);

    srand(opentld_main->seed);

    imAcqInit(imAcq);

    if(opentld_main->showOutput){
        gui->init();
    }
}

/**
 * @brief thread function for vision system
 */
void* runVisionsys(void*){
    // Main loop
    opentld_main->doWork();
    delete opentld_main;
}

/***************************************** TEMP **********************************************/
/**
 * @brief initialise vision system
 */

centroid find_centroid();