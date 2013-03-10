/*
 * File name   : comsys.cpp
 *
 * Description : Communication subsystem
 *
 * Created on  : Jul 31, 2012
 *
 * Author      : Xichen Shi
 */

#include <comsys.hpp>

UAS_comm comModule;

/**
 * @brief -- initialize communication system
 */
void initComsys() {
    Serial_apm.beginPort("/dev/ttyACM0", 115200);
    Serial_gcs.beginPort("/dev/ttyS0", 57600);

    mavlink_system.sysid = 255;
    comModule.init(&Serial_apm, &Serial_gcs); // initialize communication subsystem
}

/**
 * @brief --
 */
void *runApmLink(void*){
    while(true){
        if (comModule.isApmOpen()) {
            comModule.updateApm();
        }
    }
}

/**
 * @brief --
 */
void *runGcsLink(void*){
    while(true){
        if (comModule.isGcsOpen()) {
            comModule.updateGcs();
        }
    }
}

/**
 * @brief --
 */
void* runComsys(void*){
    pthread_t thread1, thread2;
    pthread_create( &thread1, NULL, runApmLink, NULL);
    pthread_create( &thread2, NULL, runGcsLink, NULL);
    pthread_join( thread1, NULL );
    pthread_join( thread2, NULL ); 
}