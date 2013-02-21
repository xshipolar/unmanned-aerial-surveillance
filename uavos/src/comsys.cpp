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

/**
 * @brief --
 */
void *run_apm_link(void*){
    while(true){
        comModule.update_apm_rx();
    }
}

/**
 * @brief --
 */
void *run_gcs_link(void*){
    while(true){
        comModule.update_gcs_rx();
    }
}

/**
 * @brief --
 */
void comsys(){
    pthread_t thread1, thread2;
    pthread_create( &thread1, NULL, run_apm_link, NULL);
    pthread_create( &thread2, NULL, run_gcs_link, NULL);
    pthread_join( thread1, NULL );
    pthread_join( thread2, NULL );
}