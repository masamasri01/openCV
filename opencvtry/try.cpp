#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
Mat calc_histogram(Mat scr) {
    Mat hist;
    hist = Mat::zeros(256, 1, CV_32F);
    scr.convertTo(scr, CV_32F);
    double value = 0;
    for (int i = 0; i < scr.rows; i++)
    {
        for (int j = 0; j < scr.cols; j++)
        {
            value = scr.at<float>(i, j);
            hist.at<float>(value) = hist.at<float>(value) + 1;
        }
    }
    return hist;
}


void gammaTransformation( Mat 	src,Mat& dst,  double gamma)
 {

    double c, r_min, r_max;
    minMaxLoc(src, &r_min, &r_max);
    c = 255 / pow(r_max, gamma);
    src.convertTo(src, CV_32F);
    pow(src, gamma, dst);
    dst = c * dst;
    dst.convertTo(dst, CV_8UC1);
}

void plot_histogram(Mat histogram) {
    Mat histogram_image(495, 367, CV_8UC3, Scalar(0, 0, 0));
    Mat normalized_histogram;
    normalize(histogram, normalized_histogram, 0, 495, NORM_MINMAX, -1, Mat());

    for (int i = 0; i < 256; i++)
    {
        rectangle(histogram_image, Point(2 * i, histogram_image.rows - normalized_histogram.at<float>(i)),
            Point(2 * (i + 1), histogram_image.rows), Scalar(255, 0, 0));
    }

    namedWindow("Histogram", WINDOW_NORMAL);
    imshow("Histogram", histogram_image);
    waitKey(0);

}
//but i didnt need it
void log_transformation(Mat scr, Mat& dst) {
    double c;
    double r_min, r_max;

    // determine max value in the input image
    minMaxLoc(scr, &r_min, &r_max);
    c = 255 / log(r_max + 1);
    scr.convertTo(scr, CV_32F);
    scr = scr + 1;
    log(scr, dst);
    dst = c * dst;
    dst.convertTo(dst, CV_8UC1);
}

int main() {
    //read a color image 
    Mat colorMat = imread("tst.png");
    //namedWindow("color image", WINDOW_NORMAL);
    imshow("color image", colorMat);
    //convert it to gray scale
    Mat greyMat;
    cvtColor(colorMat, greyMat, COLOR_BGR2GRAY);
    imshow("gray image", greyMat);
    //show the histogram of the image
    Mat histogram1 = calc_histogram(greyMat);
   //  plot_histogram(histogram1);

    //values to recognize the shape of histogram
    cout << "Histogram is\n";
    for (int i = 0; i < histogram1.rows; i++)
        cout << i << " is " << histogram1.at<int>(i, 0) << "\t";



    //Enhance automatically using histogram equalization
    Mat dst;
    equalizeHist(greyMat, dst);
    imshow("Equalized image", dst);
    
   //using  power transformation function
    Mat img_gamma;
    double gamma = 2.5;
   gammaTransformation(greyMat, img_gamma, gamma);
   namedWindow("gamma", WINDOW_AUTOSIZE);
    imshow("gamma=2.5", img_gamma);

    gamma = 3.5;
    gammaTransformation(greyMat, img_gamma, gamma);
    namedWindow("gamma", WINDOW_AUTOSIZE);
    imshow("gamma=3.5", img_gamma);
    gamma = 4.5;
    gammaTransformation(greyMat, img_gamma, gamma); 
    namedWindow("gamma", WINDOW_AUTOSIZE);
    imshow("gamma=4.5", img_gamma);
   
   Mat histogram = calc_histogram(img_gamma); // histogram for enhanced img by power func
   plot_histogram(histogram);
  waitKey(0);
 return 0;

}

