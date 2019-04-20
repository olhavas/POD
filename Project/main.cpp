#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{


    const string file_name = "//home//olga//Studia//CPS/POD//Project//lena.jpg";
    Mat image;
    image = imread(file_name, CV_LOAD_IMAGE_COLOR);   // Read the file

    if(! image.data )                              // Check for invalid input
    {
        cout << "Could not open or find the image" << std::endl ;
        return -1;
    }

    namedWindow( "Lena", WINDOW_NORMAL);// Create a window for display.

    imshow( "Lena", image );                // Show our image inside it.

    waitKey(0);             // Wait for a keystroke in the window
    return 0;
}