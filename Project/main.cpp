#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"
#include <iostream>
#include <exception>
#include <math.h>
#include <vector>
#include <algorithm>

using namespace cv;
using namespace std;

enum Options
{
    brightness = 1,
    contrast = 2,
    negative = 3,
    arithmeticMeanFilter = 4,
    medianFilter =5,
    hightpassfilter = 6


};
//enumeration types (both scoped and unscoped) can have overloaded operators
std::ostream& operator<<(std::ostream& os, Options c)
{
    switch(c)
    {
        case brightness   : os << "brightness";    break;
        case contrast: os << "contrast"; break;
        case negative : os << "negative";  break;

        default    : os.setstate(std::ios_base::failbit);
    }
    return os;
}

inline void readImg(const string & f, Mat & img)
{
    img = imread(f, CV_LOAD_IMAGE_COLOR);   // Read the file

    if(! img.data )                              // Check for invalid input
    {
        throw "Could not open or find the image!";
    }
}

inline short truncate(short val)
{

    if(val < 0) return 0;
    if(val > 255) return 255;
    return val;
}

void bright(const Mat &input, Mat &output, int val)
{
    for(int i = 0; i < input.rows; i++)
    {
        for(int j =0; j<input.cols; j++)
        {
            for(int c =0; c<input.channels(); c++)
            output.at<Vec3b>(i,j)[c] =truncate(input.at<Vec3b>(i,j)[c] + val); //r
//            output.at<Vec3b>(i,j)[1] =truncate(input.at<Vec3b>(i,j)[1] + val); //g
//            output.at<Vec3b>(i,j)[0] =truncate(input.at<Vec3b>(i,j)[0] + val); //b
        }
    }
}
void contr(const Mat &input, Mat &output, int val)
{
    float factor = (259.0 * (val + 255.0)) / (255.0 * (259.0 - val));
    for(int i = 0; i < input.rows; i++)
    {
        for(int j =0; j<input.cols; j++)
        {
            for(int c =0; c<input.channels(); c++)
                output.at<Vec3b>(i,j)[c] =truncate(factor*(input.at<Vec3b>(i,j)[c] -128 ) +128);

        }
    }
}
void neg(const Mat &input, Mat &output)
{
    for(int i = 0; i < input.rows; i++)
    {
        for(int j =0; j<input.cols; j++)
        {
            for(int c =0; c<input.channels(); c++)
                output.at<Vec3b>(i,j)[c] =255 -input.at<Vec3b>(i,j)[c];

        }
    }
}
void amean(const Mat &input, Mat &output, int val)
{
    int margin =0, result = 255;
    margin = (!val%2) ? (val/2)-1 : val/2;
    for(int i = margin; i < input.rows - margin; i++)
    {
        for(int j = margin; j<input.cols - margin; j++)
        {
            for(int c =0; c<input.channels(); c++)
            {
                result = input.at<Vec3b>(i,j)[c];
                for (int m = margin; m >=1 ; m--)
                {
                    result +=(input.at<Vec3b>(i-m,j-m)[c]+input.at<Vec3b>(i-m,j)[c]
                            +input.at<Vec3b>(i,j-m)[c]+input.at<Vec3b>(i+m,j+m)[c]+input.at<Vec3b>(i+m,j)[c]
                             +input.at<Vec3b>(i,j+m)[c] + input.at<Vec3b>(i-m,j+m)[c]+input.at<Vec3b>(i+m,j-m)[c]);
                }
                output.at<Vec3b>(i,j)[c] =result/(val*val);
            }
        }
    }
}

void medfilter(const Mat &input, Mat &output, int val)
{
    vector<int> colors;
    colors.reserve(val*val);
    int margin =0, result = 255;
    margin = (!val%2) ? (val/2)-1 : val/2;
    for(int i = margin; i < input.rows - margin; i++)
    {
        for(int j =margin; j<input.cols - margin; j++)
        {
            for(int c =0; c<input.channels(); c++)
            {
                colors.push_back(input.at<Vec3b>(i, j)[c]);

                //output.at<Vec3b>(i, j)[c] = input.at<Vec3b>(i, j)[c];
                for (int m = margin; m >=1 ; m--)
                {
                    colors.push_back(input.at<Vec3b>(i-m,j-m)[c]);
                    colors.push_back(input.at<Vec3b>(i-m,j)[c]);
                    colors.push_back(input.at<Vec3b>(i,j-m)[c]);
                    colors.push_back(input.at<Vec3b>(i+m,j+m)[c]);
                    colors.push_back(input.at<Vec3b>(i+m,j)[c]);
                    colors.push_back(input.at<Vec3b>(i,j+m)[c]);
                    colors.push_back(input.at<Vec3b>(i-m,j+m)[c]);
                    colors.push_back(input.at<Vec3b>(i+m,j-m)[c]);
                }
                sort(colors.begin(),colors.end());
                output.at<Vec3b>(i, j)[c] = colors[(!colors.size()%2) ? (colors.size()/2)-1 : colors.size()/2];
                colors.clear();
            }
        }
    }
}
void highpfilter(const Mat &input, Mat &output, const vector<int> & mask)
{
    uint masksize = 3;
    int margin =3, result = 0, masksum = 0;
    margin /= 2;
    for(int m =0; m< mask.size();m++)
    {
        masksum += mask[m];
    }
    for(int i = margin; i < input.rows - margin; i++)
    {
        for (int j = margin; j < input.cols - margin; j++)
        {
            for (int c = 0; c < input.channels(); c++)
            {
                for(int k = 0; k < masksize; k++)
                {
                    for(int l = 0; l < masksize; l++)
                    {
                        result+= mask[k * masksize + l] * input.at<Vec3b>(i + k - margin,j+l-margin)[c];
                    }
                }
//          szybsze
//                result +=(mask[0]*input.at<Vec3b>(i-1,j-1)[c]
//                        +mask[1]*input.at<Vec3b>(i,j-1)[c]
//                        +mask[2]*input.at<Vec3b>(i+1,j-1)[c]
//                        +mask[3]*input.at<Vec3b>(i-1,j)[c]
//                        +mask[4]*input.at<Vec3b>(i,j)[c]
//                        +mask[5]*input.at<Vec3b>(i+1,j)[c]
//                        +mask[6] * input.at<Vec3b>(i-1,j+1)[c]
//                        +mask[7] *input.at<Vec3b>(i,j+1)[c]
//                        +mask[8] *input.at<Vec3b>(i+1,j+1)[c] );
                if(masksum!=0)
                    output.at<Vec3b>(i, j)[c] = truncate(result/masksum);
                result = 0;
            }
        }
    }

}



int main( int argc, char** argv )
{
    bool endless = true;
    string wind_name = "Lenna";
    Options en;
    const string file_name = "//home//olga//Studia//CPS/POD//Project//lena500.jpg";
    Mat image, out;
    vector<int> *mask = new vector<int>();
    mask->reserve(9);
    try
    {
        readImg(file_name, image);
        //resize(image, image,Size(500,500));
    }
    catch (const char* msg)
    {
        cerr<<msg<<'\n';
        return -1;
    }
    image.copyTo(out);
    namedWindow( wind_name, WINDOW_NORMAL);// Create a window for display.

    imshow( wind_name, image );
    for(int i = brightness; i<= negative; i++)
    {
        cout<<i<<". "<< Options (i)<<"\n";
    }
    mask->push_back(0);
    mask->push_back(-1);
    mask->push_back(0);
    mask->push_back(-1);
    mask->push_back(5);
    mask->push_back(-1);
    mask->push_back(0);
    mask->push_back(-1);
    mask->push_back(0);
//    while (endless)
//    {
//        cin >> reinterpret_cast<int &>(en);
//        switch (en)
//        {
//            case brightness:
//                //
//                break;
//            case contrast:
//                //
//                break;
//            case negative:
//                //
//                break;
//            default:
//                endless = ! endless;
//                break;
//        }
//    }





    // Show our image inside it.
    //bright(image,out,50);
    //contr(image,out,50);
    //neg(image,out);
    //amean(image,out, 3);
    //medfilter(image, out, 7);
    highpfilter(image,out, *mask) ;
    namedWindow( wind_name.append("xD"), WINDOW_NORMAL);// Create a window for display.


    imshow( wind_name, out );


    waitKey(0);             // Wait for a keystroke in the window
    delete mask;
    return 0;
}