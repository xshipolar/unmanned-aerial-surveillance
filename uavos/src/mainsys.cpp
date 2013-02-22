/*
 * mainsys.cpp
 *
 *  Created on: Feb 20, 2013
 *      Author: Xichen Shi
 */

#include <mainsys.hpp>

int main() {
    initialize();
    
    pthread_t thread1;
    pthread_create( &thread1, NULL, runComsys, NULL);
    pthread_join( thread1, NULL );

    return 0;
}

void initialize() {
    program_start_time = getMicroSecond();
    initComsys();
}