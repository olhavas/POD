//
// Created by olga on 07/05/19.
//

#ifndef PROJECT_HISTOGRAM_H
#define PROJECT_HISTOGRAM_H
#include <opencv2/core/core.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"
#include <vector>
#include <math.h>
#include <algorithm>

class Histogram {
private:
    static uint cou;
    uint pixels = 0;
    uint channels = 0;
    int hist_w = 640;
    int hist_h = 480;


public:
    cv::Mat & image;
    cv::Mat * histogram;
    Histogram(cv::Mat &image);
    std::vector<std::vector<uint>> data;
    void update();
    void showTable();
    void show();
    void equalization(cv::Mat &out, uint gmin,uint gmax);
    uint getPixels() const;
    uint getChannels() const;
    static uint getCounter();
    virtual ~Histogram();
    void saveHistogramImage(std::string name);
    double calculateScale();


};


#endif //PROJECT_HISTOGRAM_H
