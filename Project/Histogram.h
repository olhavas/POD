//
// Created by olga on 07/05/19.
//

#ifndef PROJECT_HISTOGRAM_H
#define PROJECT_HISTOGRAM_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"
#include <vector>

class Histogram {
private:
    uint pixels = 0;
    uint channels = 0;


public:
    cv::Mat & image;
    Histogram(cv::Mat &image);
    std::vector<std::vector<uint>> data;
    void update();
    void show();
    uint getPixels() const;
    uint getChannels() const;
    virtual ~Histogram();


};


#endif //PROJECT_HISTOGRAM_H
