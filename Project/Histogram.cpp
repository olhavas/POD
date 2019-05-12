//
// Created by olga on 07/05/19.
//

#include "Histogram.h"

Histogram::Histogram(cv::Mat &image) : image(image)
{
    this->channels = image.channels();
    this->pixels = image.cols* image.rows;
    data.resize(channels, std::vector<uint>(256));
    //data.reserve(image.channels());
    for(int c = 0; c < image.channels(); c++)
    {
        //data[c].reserve(256);
        for(int j = 0; j < 256; j++)
        {
            data[c][j] = 0;
        }
        for(int i = 0; i < image.rows; i++)
        {
            for(int j =0; j<image.cols; j++)
            {
                data[c][image.at<cv::Vec3b>(i,j)[c]]++;
            }
        }
    }
}

Histogram::~Histogram() {

}

uint Histogram::getPixels() const {
    return pixels;
}

uint Histogram::getChannels() const {
    return channels;
}

void Histogram::update() {
    for(int c = 0; c < image.channels(); c++)
    {
        for(int j = 0; j < 256; j++)
        {
            data[c][j] = 0;
        }
        for(int i = 0; i < image.rows; i++)
        {
            for(int j =0; j<image.cols; j++)
            {
                data[c][image.at<cv::Vec3b>(i,j)[c]]++;
            }
        }
    }
}

void Histogram::show() {
    for(int c = 0; c < image.channels(); c++)
    {
        std::cout<<"channel "<<c+1<<'\n';
        for(int j = 0; j < 256; j++)
        {
            std::cout<<j<<'='<<data[c][j]<<"\n";
        }
    }

}

