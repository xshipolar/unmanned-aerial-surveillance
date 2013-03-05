/*
 * FerNNClassifier.h
 *
 *  Created on: Jun 14, 2011
 *      Author: alantrrs
 *  Modified on: Feb 26, 2013
 *      Author: Xichen Shi
 */

#ifndef FERNNCLASSIFIER_H_
#define FERNNCLASSIFIER_H_

#include <opencv2/opencv.hpp>
#include <stdio.h>

class FerNNClassifier{
public:
    //Parameters
    float thr_nn_valid;

    void read(const cv::FileNode& file);
    void prepare(const std::vector<cv::Size>& scales);
    void getFeatures(const cv::Mat& image,const int& scale_idx,std::vector<int>& fern);
    void update(const std::vector<int>& fern, int C, int N);
    float measure_forest(std::vector<int> fern);
    void trainF(const std::vector<std::pair<std::vector<int>,int> >& ferns,int resample);
    void trainNN(const std::vector<cv::Mat>& nn_examples);
    void NNConf(const cv::Mat& example,std::vector<int>& isin,float& rsconf,float& csconf);
    void evaluateTh(const std::vector<std::pair<std::vector<int>,int> >& nXT,const std::vector<cv::Mat>& nExT);
    void show();
    //Ferns Members
    int getNumStructs(){return _nstructs;}
    float getFernTh(){return _thr_fern;}
    float getNNTh(){return _thr_nn;}

    struct Feature {
            uchar x1, y1, x2, y2;
            Feature() : x1(0), y1(0), x2(0), y2(0) {}
            Feature(int _x1, int _y1, int _x2, int _y2)
            : x1((uchar)_x1), y1((uchar)_y1), x2((uchar)_x2), y2((uchar)_y2)
            {}
            bool operator ()(const cv::Mat& patch) const
            { return patch.at<uchar>(y1,x1) > patch.at<uchar>(y2, x2); }
    };
    std::vector<std::vector<Feature> > features;  //Ferns features (one std::vector for each scale)
    std::vector< std::vector<int> > nCounter;     //negative counter
    std::vector< std::vector<int> > pCounter;     //positive counter
    std::vector< std::vector<float> > posteriors; //Ferns posteriors
    float thrN;  //Negative threshold
    float thrP;  //Positive thershold

    //NN Members
    std::vector<cv::Mat> pEx; //NN positive examples
    std::vector<cv::Mat> nEx; //NN negative examples

private:
    float _thr_fern;
    float _valid;
    float _ncc_thesame;
    float _thr_nn;

    int _struct_size;
    int _nstructs;
    int _acum;
};

#endif /* FERNNCLASSIFIER_H_ */
