#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/nonfree.hpp>

const char *window1 = "Window";

int main()
{
    cv::namedWindow(window1,CV_WINDOW_AUTOSIZE);
    const cv::Mat input = cv::imread("input.jpg", 0); //Load as grayscale

    cv::SIFT detector;
    std::vector<cv::KeyPoint> keypoints;
    detector.operator()(input,input,keypoints);

    // Add results to image and save.
    cv::Mat output;
    cv::drawKeypoints(input, keypoints, output);
    cv::imshow(window1, output);
    cv::imwrite("sift_result.jpg", output);
    return 1;
}
