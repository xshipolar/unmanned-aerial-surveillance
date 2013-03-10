/*
 * UAS_tld.cpp
 *
 *  Created on: Mar 03, 2013
 *      Author: Xichen Shi
 */

#include <UAS_tld.h>

/**
 * @brief constructor for tld class
 */
UAS_tld::UAS_tld() {
}

/**
 * @brief initialization for tld vision class
 */
void UAS_tld::init(){
    // open the first video capture device
    _capture_device.open(0);
    // open the yml file containing the setting paramters and read setttings
    _parameter_file.open("../parameters.yml", FileStorage::READ);

    // check for successes
    if (!_capture_device.isOpened()) {
        _capture_device_opened = false;
        _tld_initialised = false;
        printf("Default(0) video device failed to open\n");
            if (!_parameter_file.isOpened()) {
                _parameter_file_opened = false;
                _tld_initialised = false;
                printf("File for tld settings failed to open\n");
                return;
            }
        return;
    } else {
        // Set Capture Device Resolution
        _capture_device.set(CV_CAP_PROP_FRAME_WIDTH,340);
        _capture_device.set(CV_CAP_PROP_FRAME_HEIGHT,240);

        // Set TLD with stored paramters
        _tld_vision.read(_parameter_file.getFirstTopLevelNode());
        _tld_initialised = true;
        printf("TLD vision is initialised\n");
    }
}

/**
 * @brief Get the frame ready to start tld vision tracking
 * @param input_box -- the box containing input information of the selection box
 */
void UAS_tld::readyTracking(const Rect& input_box){
    // Read one frame for starting tracking
    _capture_device >> _current_color_frame;
    cvtColor(_current_color_frame, _previous_gray_frame, CV_RGB2GRAY);

    // Initialize the first frame of vision tracking
    _tld_vision.init(_previous_gray_frame, input_box);
}


/**
 * @brief process the frame and try to track target
 */
void UAS_tld::updateTracking(){
    if (_capture_device.read(_current_color_frame)){
        // Convert to gray scale
        cvtColor(_current_color_frame, _current_gray_frame, CV_RGB2GRAY);

        // Process Frame
        _tld_vision.processFrame(_previous_gray_frame, _current_gray_frame, 
                                 _previous_features, _current_features, 
                                 _target_box, _track_status, true); // TODO: Track and Learn is always on for now

        // Clean up the features and swap frames
        swap(_previous_gray_frame,_current_gray_frame);
        _previous_features.clear();
        _current_features.clear();
    }
}

/**
 * @brief Check if tld vision class is properly initialised
 * @return -- true or false
 */
bool UAS_tld::isInitialised(){
    return _tld_initialised;
}