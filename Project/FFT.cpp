//
// Created by olga on 29/06/19.
//

#include "FFT.h"

/*
FFT::FFT(cv::Mat &image) : image(image)
{
    out.resize(image.channels(),std::vector<std::vector<Complex>>(image.rows, std::vector<Complex>(image.cols, Complex{})));

}*/
FFT::FFT(cv::Mat &image)
{
    this->image =  std::make_unique<cv::Mat>(image);
    out.resize(image.channels(),std::vector<std::vector<Complex>>(image.rows, std::vector<Complex>(image.cols, Complex{})));
    copyToComplex();


}


void FFT::copyToComplex()
{
    for(int c = 0; c < this->image->channels(); c++)
    {
        for(int i = 0; i < image->rows; i++)
        {
            for(int j =0; j<image->cols; j++)
            {
                out[c][i][j] = image->at<cv::Vec3b>(i,j)[c];
                //std::cout<< out[c][i][j]<<" ";
            }
            //std::cout<<"\n";
        }
        //::cout<<"\n";
    }

}


