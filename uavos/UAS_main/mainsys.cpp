/*
 * mainsys.cpp
 *
 *  Created on: Feb 20, 2013
 *      Author: Xichen Shi
 */

#include <mainsys.hpp>

int main() {
    
    initialize();
    
    pthread_t thread1, thread2,thread3,thread4;
    pthread_create( &thread1, NULL, runComsys, NULL);
    // pthread_create( &thread2, NULL, runVisionsys, NULL);
    // pthread_create( &thread3, NULL, runNavsys, NULL);
    pthread_create( &thread4, NULL, runUisys, NULL);
    pthread_join( thread1, NULL );
    // pthread_join( thread2, NULL );
    // pthread_join( thread3, NULL);
    pthread_join( thread4, NULL);

    return 0;
}

/**
 * @brief Initialization function for each subsystem;
 */
void initialize() {
    // Reference Time
    program_start_time_us = getMicroSeconds();           // get the reference time in u_sec (microseconds) when system
    program_start_time_ms = program_start_time_us/1000;  // get the reference time in m_sec (milliseconds) when system
    initComsys();
    initVisionsys();
    initUisys();
    g_system_status.navigation_mode = NAV_MODE_SEMIAUTO;
}
