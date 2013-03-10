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
    // Opening serial ports with settings specified
    Serial_apm.beginPort("/dev/ttyACM0", 115200);
    Serial_gcs.beginPort("/dev/ttyS0", 57600);

    mavlink_system.sysid = 255; // Set system id to 255 to match gcs

    comModule.init(&Serial_apm, &Serial_gcs); // initialize communication subsystem
}

/**
 * @brief -- send the message that control gimbal mount 
 */
bool setGimbalState(double input_pan_angle, double input_tilt_angle, double input_roll_angle) {
    if(comModule.isApmOpen()) {
        mavlink_msg_mount_control_send(MAVLINK_COMM_0,
                                       1, 0, // Target APM sysid and compid
                                       (int32_t) input_tilt_angle*100, 
                                       (int32_t) input_roll_angle*100, 
                                       (int32_t) input_pan_angle*100, 
                                       0);
        updateCurrentGimbalState(input_pan_angle, input_tilt_angle, input_roll_angle);
        return 1;
    } else {
        return 0;
    }
}

/**
 * @brief -- send the message to get the state of gimbal mount
 */
void getGimbalState();

/**
 * @brief -- thread function for running APM link
 */
void *runApmLink(void*){
    while(true){
        if (comModule.isApmOpen()) {
            comModule.updateApm();
        }
    }
}

/**
 * @brief -- thread function for running GCS link
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