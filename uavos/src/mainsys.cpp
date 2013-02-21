/*
 * mainsys.cpp
 *
 *  Created on: Feb 20, 2013
 *      Author: Xichen Shi
 */

#include <mainsys.hpp>

UAS_serial Serial_apm("/dev/ttyACM0");
UAS_serial Serial_gcs("/dev/ttyS0");
UAS_comm comModule(&Serial_apm, &Serial_gcs);

int main(){
    initialize();
    
    pthread_t thread1;
    pthread_create( &thread1, NULL, runCommModule, NULL);
    pthread_join( thread1, NULL );

    return 0;
}

/**
 * @brief -- initialize subsystems
 */
void initialize(){
    comModule.init();
}

void *runCommModule(void*){

}