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
    this->or_image = std::make_shared<cv::Mat>(image);
    this->c_img = std::make_shared<cv::Mat>(image);
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

void FFT::fastFourier1D(std::vector<std::vector<Complex>> input,const bool & inverse)
{
    double angle = 0.0;

    if(input.size() <= 1)
        return;

    std::vector<std::vector<Complex>> even;
    std::vector<std::vector<Complex>> odd;

    uint channels = input[0].size();

    for(uint i = 0; i < input.size(); i += 2)
    {
        std::vector<Complex> tempeven;
        std::vector<Complex> tempodd;
        for(uint ch = 0; ch < channels; ch++)
        {
            tempeven.push_back(input[i][ch]);
            tempodd.push_back(input[i+1][ch]);
        }
        even.push_back(tempeven);
        odd.push_back(tempodd);
    }

    fastFourier1D(even,inverse);
    fastFourier1D(odd,inverse);

    for(uint ch = 0; ch < channels; ch++)
    {
        for(uint i = 0; i < input.size()/2; i++)
        {
            if(inverse == false)
                angle = -2.0 * M_PI * static_cast<double>(i)/static_cast<double>(input.size());
            else
                angle = 2.0 * M_PI * static_cast<double>(i)/static_cast<double>(input.size());
            double real = cos(angle);
            double imaginary = sin(angle);
            Complex W{real, imaginary};

            W = W * odd[i][ch];

            input[i][ch] = even[i][ch] + W;
            input[(input.size() / 2) + i][ch] = even[i][ch] - W;

        }
    }

}

std::vector<std::vector<std::vector<Complex>>>
FFT::reverseVector(const std::vector<std::vector<std::vector<Complex>>> &input)
{
    std::vector<std::vector<std::vector<Complex>>> temp(input);
    for(int i = 0; i < c_img->rows; i++)
    {
        for(int j =0; j<c_img->cols; j++)
        {
            for(int c = 0; c < c_img->channels(); c++)
            {
                temp[i][j][c] = input[j][i][c];
            }
        }
    }
    return temp;
}

cv::Mat FFT::fastFourierTransform()
{
    copyToComplex(c_img);
    c_img->copyTo(oimg);
    for(int i = 0; i < c_img->rows; i++)
        fastFourier1D(out[i], false);
    out = reverseVector(out);
    for(int i = 0; i < c_img->rows; i++)
        fastFourier1D(out[i], false);



}

