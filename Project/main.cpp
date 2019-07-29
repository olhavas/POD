#include <iostream>
#include <exception>

#include "Histogram.h"
#include "FFT.h"

#include <experimental/filesystem>
#define PRPATH fs::current_path()//.parent_path()
#define POS en-1
namespace fs = std::experimental::filesystem;

using namespace cv;
using namespace std;

enum Options
{
    changeImg = 1,
    brightness,
    contrast,
    negative ,
    arithmeticMeanFilter,
    medianFilter,
    hightpassfilter,
    histogram,
    operatorRobertsaII

};

string option [] = {"changeImg","brightness", "contrast", "negative", "arithmeticMeanFilter",
                    "medianFilter", "hightpassfilter", "histogram", "operatorRobertsaII"};

class FFT;

vector<vector<vector<double>>> matToVec(const Mat & image)
{
    vector<vector<vector<double>>> out(image.channels(), vector<vector<double>>(image.rows, vector<double>(image.cols)));

    for(int c = 0; c < image.channels(); c++)
    {
        for(int i = 0; i < image.rows; i++)
        {
            for(int j =0; j<image.cols; j++)
            {
                out[c][i][j]= static_cast<double >(image.at<Vec3b>(i,j)[c]);
            }
        }
    }

}


inline double MSE(const Mat & in, const Mat & out)
{
    if(in.cols != out.cols || in.rows != out.rows)
    {
        cout<<"Images have different size and it means that images are different\n";
        return -1;
    }
    double error[in.channels()], val = 0;
    const char * rgb = "BGR";
    for(int i =0; i <in.channels(); i++)
    {
        error[i] = 0;
    }
    for(int c =0; c<in.channels(); c++)
    {
        for(int i = 0; i < in.rows; i++)
        {
            for(int j =0; j<in.cols; j++)
            {
                error[c]+=pow(in.at<Vec3b>(i,j)[c] - out.at<Vec3b>(i,j)[c],2)/(in.rows*in.cols);
            }
        }
    }
    for(int i = in.channels()-1; i >=0;i--)
    {
        cout<<"MSE ["<<rgb[i]<< "] = "<< error[i]<<'\n';
        val += error[i];
    }
    val/=in.channels();
    return val;
}

inline void readImg(const string & f, Mat & img)
{
    img = imread(f, CV_LOAD_IMAGE_COLOR);   // Read the file
    cout<<f<<'\n';

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
            output.at<Vec3b>(i,j)[c] =truncate(input.at<Vec3b>(i,j)[c] + val);

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
void opRobertsaII(const Mat &input, Mat &output)
{
    //int result= 0;
    for(int i = 0; i < input.rows; i++)
    {
        for(int j =0; j<input.cols; j++)
        {
            for(int c =0; c<input.channels(); c++)
            {
                if(i == input.rows -1 && j == input.cols - 1)
                {
                    output.at<Vec3b>(i,j)[c]=truncate(abs(input.at<Vec3b>(i,j)[c] - input.at<Vec3b>(i-1,j-1)[c])
                                                      +abs(input.at<Vec3b>(i,j-1)[c] - input.at<Vec3b>(i-1,j)[c]));
                }
                else if(i == input.rows-1)
                {
                    output.at<Vec3b>(i,j)[c]=truncate(abs(input.at<Vec3b>(i,j)[c] - input.at<Vec3b>(i-1,j+1)[c])
                                                      +abs(input.at<Vec3b>(i,j+1)[c] - input.at<Vec3b>(i-1,j)[c]));
                }
                else if (j == input.cols - 1)
                {
                    output.at<Vec3b>(i,j)[c]=truncate(abs(input.at<Vec3b>(i,j)[c] - input.at<Vec3b>(i+1,j-1)[c])
                                                      +abs(input.at<Vec3b>(i,j-1)[c] - input.at<Vec3b>(i-1,j)[c]));
                }
                else
                {
                    output.at<Vec3b>(i,j)[c]=truncate(abs(input.at<Vec3b>(i,j)[c] - input.at<Vec3b>(i+1,j+1)[c])
                                                      +abs(input.at<Vec3b>(i,j+1)[c] - input.at<Vec3b>(i+1,j)[c]));
                }
            }

        }
    }
}



int main( int argc, char** argv )
{
    bool endless = true;
    int temporary = 0, temporary1 = 0;
    string wind_name = "Lenna", img_name = "lena500.jpg";;
    int en;
    string file_name = PRPATH + "/"+ img_name;//"//home//olga//Studia//CPS/POD//Project//lenac_uniform2.bmp";
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

    if(fs::exists(PRPATH / "Processed"))
    {
        fs::remove_all(PRPATH / "Processed");
    }
    fs::create_directory(static_cast<string>(PRPATH)+"/Processed");

    image.copyTo(out);

    for(int i = changeImg; i<= operatorRobertsaII; i++)
    {
        cout<<i<<". "<< option[i-1]<<"\n";
    }
    cout<<"Finish program enter any number except number of options!\n";
    mask->push_back(0);
    mask->push_back(-1);
    mask->push_back(0);
    mask->push_back(-1);
    mask->push_back(5);
    mask->push_back(-1);
    mask->push_back(0);
    mask->push_back(-1);
    mask->push_back(0);


    auto srd_FFT = make_shared<FFT>(image, out);

    namedWindow( wind_name, WINDOW_NORMAL);// Create a window for display.
    imshow( wind_name, image );
    cout<<"Set brightness: ";

    namedWindow( static_cast<string>(wind_name+"fft"), WINDOW_NORMAL);// Create a window for display.
    imshow( static_cast<string>(wind_name+"fft"), srd_FFT->fastFourierTransform() );
    waitKey(0);             // Wait for a keystroke in the window



    /*
    while (endless)
    {
        cout<<"Choose an option: ";
        cin >> en;
        switch (en)
        {
            case changeImg:
            {
                try
                {
                    cin>>img_name;
                    file_name = PRPATH + "/"+ img_name;
                    readImg(file_name, image);
                }
                catch (const char* msg)
                {
                    cerr<<msg<<'\n';
                }
                image.copyTo(out);
            } break;
            case brightness:
                namedWindow( wind_name, WINDOW_NORMAL);// Create a window for display.
                imshow( wind_name, image );
                cout<<"Set brightness: ";
                cin>>temporary;
                bright(image,out,temporary);
                imwrite(static_cast<string>((PRPATH)/"Processed/") + option[POS]+'('+ to_string(temporary) +")_"+ fs::path(file_name).filename(),out);
                namedWindow( static_cast<string>(wind_name+'_'+ option[POS]), WINDOW_NORMAL);// Create a window for display.
                imshow( static_cast<string>(wind_name+'_'+ option[POS]), out );
                waitKey(0);             // Wait for a keystroke in the window
                image.copyTo(out);
                break;
            case contrast:
                namedWindow( wind_name, WINDOW_NORMAL);// Create a window for display.
                imshow( wind_name, image );
                cout<<"Set contrast: ";
                cin>>temporary;
                contr(image,out,temporary);
                imwrite(static_cast<string>((PRPATH)/"Processed/") + option[POS]+'('+ to_string(temporary) +")_"+ fs::path(file_name).filename(),out);
                namedWindow( static_cast<string>(wind_name +'_'+ option[POS]), WINDOW_NORMAL);// Create a window for display.
                imshow( static_cast<string>(wind_name+'_'+ option[POS]), out );
                waitKey(0);             // Wait for a keystroke in the window
                image.copyTo(out);
                break;
            case negative:
                namedWindow( wind_name, WINDOW_NORMAL);// Create a window for display.
                imshow( wind_name, image );
                neg(image,out);
                imwrite(static_cast<string>((PRPATH)/"Processed/") + option[POS]+'_'+ fs::path(file_name).filename(),out);
                namedWindow( static_cast<string>(wind_name +'_'+ option[POS]), WINDOW_NORMAL);// Create a window for display.
                imshow( static_cast<string>(wind_name +'_'+ option[POS]), out );
                waitKey(0);             // Wait for a keystroke in the window
                image.copyTo(out);
                break;
            case arithmeticMeanFilter:
                namedWindow( wind_name, WINDOW_NORMAL);// Create a window for display.
                imshow( wind_name, image );
                cout<<"Set mask size (NxN): ";
                cin>>temporary;
                amean(image,out,temporary);
                cout<< MSE(image,out)<<'\n';
                imwrite(static_cast<string>((PRPATH)/"Processed/") + option[POS]+"mask("+ to_string(temporary) +'x'+ to_string(temporary)
                                                                    +")_"+ fs::path(file_name).filename(),out);
                namedWindow( static_cast<string>(wind_name +'_'+ option[POS]), WINDOW_NORMAL);// Create a window for display.
                imshow( static_cast<string>(wind_name +'_'+ option[POS]), out );
                waitKey(0);             // Wait for a keystroke in the window
                image.copyTo(out);
                break;
            case medianFilter:
                namedWindow( wind_name, WINDOW_NORMAL);// Create a window for display.
                imshow( wind_name, image );
                cout<<"Set mask size (NxN): ";
                cin>>temporary;
                medfilter(image,out,temporary);
                cout<< MSE(image,out)<<'\n';
                imwrite(static_cast<string>((PRPATH)/"Processed/") + option[POS]+"mask("+ to_string(temporary) +'x'+ to_string(temporary)
                        +")_"+ fs::path(file_name).filename(),out);
                namedWindow( static_cast<string>(wind_name +'_'+ option[POS]), WINDOW_NORMAL);// Create a window for display.
                imshow( static_cast<string>(wind_name +'_'+ option[POS]), out );
                waitKey(0);             // Wait for a keystroke in the window
                image.copyTo(out);
                break;
            case hightpassfilter:
                namedWindow( wind_name, WINDOW_NORMAL);// Create a window for display.
                imshow( wind_name, image );
                cout<<"0 -1  0\n-1 5 -1\n0 -1  0\n";
                cout<<"Use default mask ? <yes = 1 >";
                cin>> temporary;
                if(temporary!=1)
                {
                    mask->clear();
                    mask->reserve(9);
                    cout<<"Set hightpass mask (3x3): ";
                    for(int i = 0; i < 9; i++)
                    {
                        cin>>temporary;
                        mask->push_back(temporary);
                    }
                }
                highpfilter(image,out, *mask) ;
                imwrite(static_cast<string>((PRPATH)/"Processed/") + option[POS]+'_'+ fs::path(file_name).filename(),out);
                namedWindow( static_cast<string>(wind_name +'_'+ option[POS]), WINDOW_NORMAL);// Create a window for display.
                imshow( static_cast<string>(wind_name +'_'+ option[POS]), out );
                waitKey(0);             // Wait for a keystroke in the window
                image.copyTo(out);
                break;
            case histogram: {
                namedWindow(wind_name, WINDOW_NORMAL);// Create a window for display.
                imshow(wind_name, image);
                Histogram histog (image);
                histog.show();
                cout<<"Histogram equalization < yes = 1>: ";
                cin>> temporary;
                if(temporary == 1)
                {
                    cout << "Set min max <0...255>: ";
                    cin >> temporary >> temporary1;
                    histog.equalization(out, temporary, temporary1);
                    temporary = 0;
                }
                Histogram hout(out);
                hout.show();
                imwrite(static_cast<string>((PRPATH) / "Processed/") + option[POS] + '_' +
                        fs::path(file_name).filename(), out);
                ////////dla sprawka po zaliczeniu usunąć
                histog.saveHistogramImage(static_cast<string>((PRPATH) / "Processed/")+"histogram_of_" + fs::path(file_name).filename());
                hout.saveHistogramImage(static_cast<string>((PRPATH) / "Processed/")+"histogram_of_" + option[POS] + '_' +
                                          fs::path(file_name).filename());
                ///////
                namedWindow(static_cast<string>(wind_name + '_' + option[POS]),
                            WINDOW_NORMAL);// Create a window for display.
                imshow(static_cast<string>(wind_name + '_' + option[POS]), out);
                waitKey(0);             // Wait for a keystroke in the window
                image.copyTo(out);
            }break;
            case operatorRobertsaII:
                namedWindow( wind_name, WINDOW_NORMAL);// Create a window for display.
                imshow( wind_name, image );
                opRobertsaII(image,out);
                imwrite(static_cast<string>((PRPATH)/"Processed/") + option[POS]+'_'+ fs::path(file_name).filename(),out);
                namedWindow( static_cast<string>(wind_name +'_'+ option[POS]), WINDOW_NORMAL);// Create a window for display.
                imshow( static_cast<string>(wind_name +'_'+ option[POS]), out );
                waitKey(0);             // Wait for a keystroke in the window
                image.copyTo(out);
                break;
            default:
                endless = ! endless;
                waitKey(0);             // Wait for a keystroke in the window
                delete mask;
                return 0;

        }
    } */

    delete  mask;



    // Show our image inside it.
    //bright(image,out,50);
    //contr(image,out,50);
    //neg(image,out);
    //amean(image,out, 3);
    //medfilter(image, out, 3);

    //cout<< MSE(image,out)<<'\n';
//    histog.showTable();
//    histog.show();
//    histog.equalization(out, 0,150);
//    Histogram hout (out);
//    hout.show();

    //highpfilter(image,out, *mask) ;
    //opRobertsaII(image,out);
//
//    namedWindow( wind_name+"xD", WINDOW_NORMAL);// Create a window for display.
//
//    imshow( wind_name+"xD", out );


    return 0;
}