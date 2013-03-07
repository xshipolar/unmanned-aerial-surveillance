/*
 * UAS_tld.h
 *
 *  Created on: Mar 03, 2013
 *      Author: Xichen Shi
 */

#ifndef UAS_TLD_H_
#define UAS_TLD_H_

#include <opencv2/opencv.hpp>
#include <tld_utils.h>
#include <iostream>
#include <sstream>
#include <TLD.h>
#include <stdio.h>
#include <time.h>

using namespace cv;
using namespace std;

class UAS_tld
{
public:
    UAS_tld();
    //~UAS_tld();

    void init();
    void readyTracking(const Rect& input_box);
    void updateTracking();

    bool isInitialised();

    VideoCapture _capture_device;
    FileStorage  _parameter_file;
    BoundingBox  _target_box;
    vector<Point2f> _current_features;
    vector<Point2f> _previous_features;
    bool _track_status;
    Mat  _current_color_frame;

private:
    TLD          _tld_vision;

    bool _capture_device_opened;
    bool _parameter_file_opened;
    bool _tld_initialised;
    
    Mat  _current_gray_frame;
    Mat  _previous_gray_frame;
};

#endif /* UAS_TLD_H_ */