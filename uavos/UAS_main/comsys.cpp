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
    Serial_gcs.beginPort("/dev/ttyS1", 57600);

    mavlink_system.sysid = 255; // Set system id to 255 to match gcs

    comModule.init(&Serial_apm, &Serial_gcs); // initialize communication subsystem
}

/**
 * @brief -- send the message that control gimbal mount 
 * @param input_pan_angle  -- pan angle to be set to gimbal mount in degs
 * @param input_tilt_angle -- tilt angle to be set to gimbal mount in degs
 * @param input_roll_angle -- roll angle to be set to gimbal mount in degs
 * @return -- 1 if set successfully, 0 if port no available (failure)
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
    // Start loop timer
    unsigned long last_time_apm_link = getMilliSeconds();
    for(;;){
        if (comModule.isApmOpen()) {
            comModule.updateApm();
            last_time_apm_link = getMilliSeconds();
        }
    }
}

void *sendApmMessage(void*){
    for(;;){
        pthread_mutex_lock(&mutex_apm_send_lock);

        // Wait to be waken up to send data
        pthread_cond_wait(&condition_apm_send_var, &mutex_apm_send_lock);
        int len = Serial_apm.send(&apm_tx_buf, apm_tx_buf_len);

        //printf("Size written: %d / %d\n",len, apm_tx_buf_len);
        pthread_mutex_unlock(&mutex_apm_send_lock);
    }
}

/**
 * @brief -- thread function for running GCS link
 */
void *runGcsLink(void*){
    // Start loop timer
    unsigned long last_time_gcs_link = getMilliSeconds();
    for(;;){
        if (comModule.isGcsOpen()) {
            comModule.updateGcs();
            last_time_gcs_link = getMilliSeconds();
        }
    }
}

void *sendGcsMessage(void*){
    for(;;){
        pthread_mutex_lock(&mutex_gcs_send_lock);

        // Wait to be waken up to send data
        pthread_cond_wait(&condition_gcs_send_var, &mutex_gcs_send_lock);

        int len = Serial_gcs.send(&gcs_tx_buf, gcs_tx_buf_len);
        //printf("Size written: %d / %d\n",len, gcs_tx_buf_len);
        pthread_mutex_unlock(&mutex_gcs_send_lock);
    }
}

/**
 * @brief --
 */
void* runComsys(void*){
    pthread_t thread1, thread2, thread3, thread4;
    pthread_create( &thread1, NULL, runApmLink, NULL);
    pthread_create( &thread2, NULL, runGcsLink, NULL);
    pthread_create( &thread3, NULL, sendApmMessage, NULL);
    pthread_create( &thread4, NULL, sendGcsMessage, NULL);
    pthread_join( thread1, NULL );
    pthread_join( thread2, NULL ); 
    pthread_join( thread3, NULL );
    pthread_join( thread4, NULL ); 
}