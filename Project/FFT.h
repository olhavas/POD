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
    FFT(cv::Mat & image);
    //cv::Mat * out;


private:
    cv::Mat & image;

    //std::vector<std::unique_ptr<double>> sin();
    //std::vector<std::unique_ptr<double>> cos();
    std::vector<std::vector<std::vector<std::complex<double>>>> out;


};


#endif //POD_FFT_H
