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

UAS_serial Serial_apm("/dev/ttyACM0");
UAS_serial Serial_gcs("/dev/ttyS0");
UAS_comm comModule(&Serial_apm, &Serial_gcs);

/**
 * @brief -- initialize communication system
 */
void initComsys() {
    comModule.init(); // initialize communication subsystem
}

/**
 * @brief --
 */
void *runApmLink(void*){
    while(true){
        comModule.updateApm();
    }
}

/**
 * @brief --
 */
void *runGcsLink(void*){
    while(true){
        comModule.updateGcs();
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