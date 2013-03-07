/*
 * mainsys.cpp
 *
 *  Created on: Feb 20, 2013
 *      Author: Xichen Shi
 */

#include <mainsys.hpp>

int main() {
    
    initialize();
    
    pthread_t thread1, thread2;
    pthread_create( &thread1, NULL, runComsys, NULL);
    pthread_create( &thread2, NULL, runVisionsys, NULL);
    pthread_join( thread1, NULL );
    pthread_join( thread2, NULL );

    return 0;
}

/**
 * @brief Initialization function for each subsystem;
 */
void initialize() {
    // Reference Time
    program_start_time = getMicroSeconds(); // get the reference time in u_sec (microseconds) when system
    initComsys();
    initVisionsys();
    g_system_status.navigation_mode = NAV_MODE_SEMIAUTO;
    //uiModule.init_ui();
    //uiModule.load_ui();
}