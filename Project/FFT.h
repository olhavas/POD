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



inline void swap(Complex *v1, Complex *v2);

class FFT {
public:
    //FFT(cv::Mat & image);
    FFT(const cv::Mat &image, cv::Mat &oimg);

    cv::Mat oimg;
    cv::Mat fastFourierTransform();


private:
    //cv::Mat & image;
    std::shared_ptr<cv::Mat> or_image;
    std::shared_ptr<cv::Mat> c_img;
    uint cols;
    uint rows;
    uint channels;
    uint NO_OF_BITS;

    //std::vector<std::unique_ptr<double>> sin();
    //std::vector<std::unique_ptr<double>> cos();


    //[row][col][channel]
    std::vector<std::vector<std::vector<Complex>>> out;
    void copyToComplex(std::shared_ptr<cv::Mat> image);
    std::vector<std::vector<std::vector<Complex>>> reverseVector(const std::vector<std::vector<std::vector<Complex>>> &input);
    void fastFourier1D(std::vector<std::vector<Complex>> input, const bool & inverse);
    uint reverseBits(uint num);

    void resultToImg(const std::vector<std::vector<std::vector<Complex>>> &input);




};


#endif //POD_FFT_H
