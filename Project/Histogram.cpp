//
// Created by olga on 07/05/19.
//

#include "Histogram.h"

extern short truncate(short val);

uint Histogram::cou =0;

Histogram::Histogram(cv::Mat &image) : image(image)
{
    cou++;
    this->channels = image.channels();
    this->pixels = image.cols* image.rows;
    data.resize(channels, std::vector<uint>(256));
    this->histogram = new cv::Mat(hist_h, hist_w, CV_8UC3, cv::Scalar( 0,0,0));
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
    cou--;
    delete histogram;
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

void Histogram::showTable() {
    for(int c = 0; c < image.channels(); c++)
    {
        //std::cout<<"channel "<<c+1<<'\n';
        for(int j = 0; j < 256; j++)
        {
            //std::cout<<j<<'='<<data[c][j]<<"\n";
        }
    }

}

void Histogram::show() {
    const int margin = 3;
    const int min_y = margin;
    const int max_y = hist_h - margin;
    const double scale = 0.05;
    cv::Scalar colours[] =
            {
                    {255, 0, 0},    // blue
                    {0, 255, 0},    // green
                    {0, 0, 255}     // red
            };
    float bin_w = static_cast<float>(hist_w) / 256.0f;
    for(int c = 0; c < image.channels(); c++)
    {
        for(int i = 1; i < 256; i++)
        {
//            cv::line(*histogram, cv::Point(std::round(bin_w*(i-1)), std::clamp(hist_h - static_cast<int>(data[c][i-1]),min_y,max_y)),
//                    cv::Point(bin_w*(i),std::clamp(hist_h - static_cast<int>(data[c][i]),min_y,max_y)), colours[c],2,8,0);
            cv::line(*histogram, cv::Point(std::round(bin_w*(i-1)), std::clamp(hist_h - static_cast<int>(data[c][i-1]*scale),min_y,max_y)),
                     cv::Point(bin_w*(i),std::clamp(hist_h - static_cast<int>(data[c][i]*scale),min_y,max_y)), (image.channels() == 1)? cv::Scalar(255, 255, 255):colours[c],2,8,0);
        }
    }
    std::string name = "Histogram";
    cv::namedWindow(name +std::to_string(cou), cv::WINDOW_AUTOSIZE);
    cv::imshow(name + std::to_string(cou), *histogram );

}

void Histogram::equalization(cv::Mat &out, uint gmin,uint gmax) {
    double sum, dmin;
    int l = 0;
    double temp [this->channels][256];
    int **lut = new int*[this->channels];
    for(int i = 0; i < this->channels; i++) {
        lut [i] = new int[256];
        sum =0.0;
        for(int j = 0; j<256;j++)
        {
            temp[i][j] = 0;

            sum += (static_cast<double >(data[i][j])/this->pixels);

            temp[i][j] += sum;

            //std::cout<<"new "<<i<<": "<< j <<" = --------"<<sum<<'\n';
        }

    }
    for(int i = 0; i < this->channels; i++)
    {
        l = 0;
        while (temp[i][l] == 0) l++;
        dmin = temp[i][l];

        for(int j = 0; j<256;j++)
        {
            //lut[i][j] = round(((temp[i][j]-dmin)/(static_cast<double>(this->pixels)-dmin))*255);
            lut[i][j] = round(gmin + (gmax - gmin)*temp[i][j]);

            //std::cout<<"new "<<i<<": "<< j <<" = "<<lut[i][j]<<'\n';
        }
    }


    for(int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols ; j++) {
            for (int c = 0; c < image.channels(); c++) {
                out.at<cv::Vec3b>(i,j)[c] = lut[c][image.at<cv::Vec3b>(i,j)[c]];
            }
        }
    }

    for(int i = 0; i < this->channels; i++) {
        delete [] lut[i];
    }
    delete [] lut;




}

uint Histogram::getCou() {
    return cou;
}

