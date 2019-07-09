//
// Created by olga on 29/06/19.
//

#include "FFT.h"

/*
FFT::FFT(cv::Mat &image) : image(image)
{
    out.resize(image.channels(),std::vector<std::vector<Complex>>(image.rows, std::vector<Complex>(image.cols, Complex{})));

}*/
FFT::FFT(const cv::Mat &image, cv::Mat &oimg) : oimg(oimg)
{
    this->image = std::make_shared<cv::Mat>(image);
    this->c_img = std::make_shared<cv::Mat>()
    out.resize(image.rows,std::vector<std::vector<Complex>>(image.cols, std::vector<Complex>(image.channels(), Complex{})));


}


void FFT::copyToComplex(std::shared_ptr<cv::Mat> image)
{

    for(int i = 0; i < image->rows; i++)
    {
        for(int j =0; j<image->cols; j++)
        {
            for(int c = 0; c < image->channels(); c++)
            {
                out[i][j][c] = image->at<cv::Vec3b>(i,j)[c];
                //std::cout<< out[c][i][j]<<" ";
            }

        }
        //std::cout<<"\n";
    }
    //::cout<<"\n";

}

void FFT::fastFourier1D(std::vector<std::vector<Complex>> input,const bool & inverse) {

}

std::vector<std::vector<std::vector<Complex>>>
FFT::reverseVector(const std::vector<std::vector<std::vector<Complex>>> &input) {
    std::vector<std::vector<std::vector<Complex>>> temp(input);
    for(int i = 0; i < image->rows; i++)
    {
        for(int j =0; j<image->cols; j++)
        {
            for(int c = 0; c < image->channels(); c++)
            {
                temp[i][j][c] = input[j][i][c];
            }
        }
    }
    return temp;
}

cv::Mat FFT::fastFourierTransform() {
    copyToComplex(image);
    cv::Mat oimg(image);
    for(int i = 0; i < image->rows; i++)
        fastFourier1D(out[i], false);
    out = reverseVector(out);
    for(int i = 0; i < image->rows; i++)
        fastFourier1D(out[i], false);



}

void FFT::fastFourierInverse() {

}



