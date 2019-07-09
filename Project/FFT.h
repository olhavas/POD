//
// Created by olga on 29/06/19.
//

#ifndef POD_FFT_H
#define POD_FFT_H


#include <vector>
#include <memory>
#include <cmath>
#include <complex>
#include "Complex.h"

class FFT {
public:
    //FFT(cv::Mat & image);
    FFT(const cv::Mat &image, cv::Mat &oimg);

    cv::Mat oimg;


private:
    //cv::Mat & image;
    std::shared_ptr<cv::Mat> image;
    std::shared_ptr<cv::Mat> c_img;

    //std::vector<std::unique_ptr<double>> sin();
    //std::vector<std::unique_ptr<double>> cos();
    std::vector<std::vector<std::vector<Complex>>> out;
    void copyToComplex(std::shared_ptr<cv::Mat> image);
    std::vector<std::vector<std::vector<Complex>>> reverseVector(const std::vector<std::vector<std::vector<Complex>>> &input);
    void fastFourier1D(std::vector<std::vector<Complex>> input, const bool & inverse);
    cv::Mat fastFourierTransform();



};


#endif //POD_FFT_H
