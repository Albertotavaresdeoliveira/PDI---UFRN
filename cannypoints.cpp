#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace cv;

#define STEP 5
#define JITTER 3
#define RAIO 6
#define RAIOCANNY 2

int top_slider = 10;
int top_slider_max = 200;

int width, height, gray;

char TrackbarName[50];

Mat image, border, points, cannypoints;

void on_trackbar_canny(int, void*){
  Canny(image, border, top_slider, 3*top_slider);

  points.copyTo(cannypoints);

  for(int i = 0; i<height; i++){
    for(int j = 0; j<width; j++){
      if(border.at<uchar>(i, j) == 255){
        gray = image.at<uchar>(i,j);
        circle(cannypoints, cv::Point(j,i), RAIOCANNY, CV_RGB(gray,gray,gray), -1, LINE_AA);
      }
    }
  }
  imshow("canny", border);
  imshow("Cannypoints.jpg", cannypoints);
}

int main(int argc, char** argv){
  vector<int> yrange;
  vector<int> xrange;

  int x, y;

  image = imread(argv[1],IMREAD_GRAYSCALE);

  srand(time(0));

  if(!image.data){
	cout << "nao abriu" << argv[1] << endl;
    cout << argv[0] << " imagem.jpg";
    exit(0);
  }

  width=image.size().width;
  height=image.size().height;

  xrange.resize(height/STEP);
  yrange.resize(width/STEP);

  iota(xrange.begin(), xrange.end(), 0);
  iota(yrange.begin(), yrange.end(), 0);

  for(uint i=0; i<xrange.size(); i++){
    xrange[i]= xrange[i]*STEP+STEP/2;
  }

  for(uint i=0; i<yrange.size(); i++){
    yrange[i]= yrange[i]*STEP+STEP/2;
  }

  points = Mat(height, width, CV_8U, Scalar(255));

  random_shuffle(xrange.begin(), xrange.end());

  for(auto i : xrange){
    random_shuffle(yrange.begin(), yrange.end());
    for(auto j : yrange){
      x = i+rand()%(2*JITTER)-JITTER+1;
      y = j+rand()%(2*JITTER)-JITTER+1;
      gray = image.at<uchar>(x,y);
      circle(points, cv::Point(y,x), RAIO, CV_RGB(gray,gray,gray), -1, LINE_AA);
    }
  }

  points.copyTo(cannypoints);

  sprintf( TrackbarName, "Threshold inferior", top_slider_max );

  namedWindow("canny",1);
  createTrackbar( TrackbarName, "canny", &top_slider, top_slider_max, on_trackbar_canny );

  on_trackbar_canny(top_slider, 0 );
  
  imshow("Image.jpg", image);
  imshow("Pontos.jpg", points);
  waitKey();

  imwrite("lenagray.jpg", image);
  imwrite("pontos.jpg", points);
  imwrite("cannypoints.jpg", cannypoints);

  return 0;
}