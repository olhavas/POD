//
// Created by olga on 29/06/19.
//

#include "FFT.h"


FFT::FFT(cv::Mat &image) : image(image)
{
    out.resize(image.channels(),std::vector<std::vector<std::complex<double>>>(image.rows, std::vector<std::complex<double>>(image.cols, std::complex<double>{})));

}
