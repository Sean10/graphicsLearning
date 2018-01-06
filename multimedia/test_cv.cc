#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;
void changeBrightness(Mat image, double contrast = 2.0, int brightness = 50);
void rotate(Mat src, double angle = 90);
void scale(Mat image,float scale_x = 1.5, float scale_y = 1.5);
void translate(Mat image, int dx = 50, int dy = 50);


void scale(Mat image,float scale_x, float scale_y)
{
	Mat outputImage;
	resize(image, outputImage, Size(),scale_x,scale_y,CV_INTER_LINEAR);
	imwrite("out.bmp", outputImage);
	imshow("scale", outputImage);
	waitKey(0);
}

void rotate(Mat src, double angle)
{
    Mat dst;
    Point2f pt(src.cols/2., src.rows/2.);
	int ds_max = max(src.cols, src.rows);
    Mat r = getRotationMatrix2D(pt, angle, 1.0);
    warpAffine(src, dst, r, Size(ds_max, ds_max));
    imwrite("out_rotate.bmp", dst);
	imshow("rotate", dst);
	waitKey(0);
}

void mirror(Mat image)
{
	Mat dst;
	flip(image, dst,0);
	imshow("mirror", dst);
	imwrite("out_mirror.bmp",dst);
	waitKey(0);
}

void changeBrightness(Mat image, double contrast, int brightness)
{
	Mat new_image = Mat::zeros( image.size(), image.type() );

	for( int y = 0; y < image.rows; y++ )
	{
		for( int x = 0; x < image.cols; x++ )
	    {
			for( int c = 0; c < 3; c++ )
	        {
	      		new_image.at<Vec3b>(y,x)[c] = saturate_cast<uchar>( contrast*( image.at<Vec3b>(y,x)[c] ) + brightness );
	        }
	    }
	}

	imwrite("out_bright_contrast.bmp", new_image);
	imshow("brightness and contrast", new_image);
	waitKey(0);

}

void translate(Mat image, int dx, int dy)
{

	int rows = image.rows+ abs(dx), cols = image.cols+abs(dy);
	Mat dst;
	Point2f pt(image.cols/2.+dx, image.rows/2.+dy);
	Mat r = getRotationMatrix2D(pt, 0, 1.0);
	warpAffine(image, dst, r, Size(cols, rows));
	imshow("shift", dst);
	waitKey(0);
	imwrite("out_shift.bmp", dst);

}

int main(int argc, char** argv)
{
    //String imageName("../data/HappyFish.jpg"); // by default  使用String构造函数设置路径
    string imageName;// = "1.bmp";
	cout << "please input your bmp file name:" << endl;
	cin >> imageName;

    if (argc > 1)
    {
        imageName = argv[1];
    }

    Mat image;
    image = imread(imageName, IMREAD_COLOR); // Read the file
    if (image.empty())                      // Check for invalid input
    {
        cout << "Could not open or find the image" << std::endl;
        return -1;
    }
    namedWindow("Display window", WINDOW_AUTOSIZE);
	// Create a window for display.
    imshow("Display window", image);                // Show our image inside it.
	double contrast = 0; /**< Simple contrast control */
	int brightness = 0;  /**< Simple brightness control */

	// std::cout<<" Basic Linear Transforms "<<std::endl;
	// std::cout<<"-------------------------"<<std::endl;
	// std::cout<<"* Enter the contrast value [1.0-3.0]: ";
	// std::cin>>contrast;
	// std::cout<<"* Enter the brightness value [0-100]: ";
	// std::cin>>brightness;


	if(contrast == 0 || brightness == 0)
		changeBrightness(image);
	else
		changeBrightness(image, contrast, brightness);

	mirror(image);

	int dx = 0, dy = 0;
	// cout << "Please input how long you want to move, dx and dy:" << endl;
	// cin >> dx >> dy;
	if(dx == 0 || dy == 0)
		translate(image);
	else
		translate(image, dx, dy);
	scale(image);
	rotate(image);

    waitKey(0);


	// Wait for a keystroke in the window  Zero means to wait forever
    destroyAllWindows();
    return 0;
}
