#include <iostream>
#include <cstdio>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

double alfa;
int alfa_slider = 100;
int alfa_slider_max = 100;

int height_slider = 20;
int height_slider_max = 100;

int position_slider = 50;
int position_slider_max = 100;

int height_focus = 50; // altura da regiao central em foco
int position_focus = 50; // posição do centro da regiao central em foco

Mat image1, image2, blended;
Mat imageTop; 

char TrackbarName[50];

void on_trackbar_blend(int, void*){
 alfa = (double) alfa_slider/alfa_slider_max ;
 addWeighted(image1, 1-alfa, imageTop, alfa, 0.0, blended);
 imshow("addweighted", blended);
}

//Altura da região central que entrará em foco
void on_trackbar_height(int, void*){
  image2.copyTo(imageTop);
  int limit = height_slider*255/100;
  if(limit > 0){
    Mat tmp = image1(Rect(0, position_focus-(limit/2), 256, limit));//256
    tmp.copyTo(imageTop(Rect(0, position_focus-(limit/2), 256, limit)));
  }
  on_trackbar_blend(height_slider,0);
}

//Posição vertical do centro da região em foco
void on_trackbar_position(int, void*){
  image2.copyTo(imageTop);
  int limit = height_slider*255/100;
  position_focus = position_slider*255/100;
  if(limit > 0){
    Mat tmp = image1(Rect(0, position_focus-(limit/2), 256, limit));//256
    tmp.copyTo(imageTop(Rect(0, position_focus-(limit/2), 256, limit)));
  }
  on_trackbar_blend(height_slider,0);
}

int main(int argvc, char** argv){
  image1 = imread("blend1.jpg");
  image2 = imread("blend2.jpg");
  image2.copyTo(imageTop);
  namedWindow("addweighted", 1);

  std::sprintf( TrackbarName, "Alpha x %d", alfa_slider_max );
  createTrackbar( TrackbarName, "addweighted",
                      &alfa_slider,
                      alfa_slider_max,
                      on_trackbar_blend );
  on_trackbar_blend(alfa_slider, 0 );

  std::sprintf( TrackbarName, "Altura x %d", height_slider_max );
  createTrackbar( TrackbarName, "addweighted",
                      &height_slider,
                      height_slider_max,
                      on_trackbar_height );
  on_trackbar_height(height_slider, 0 );

  std::sprintf( TrackbarName, "Posicao x %d", position_slider_max );
  createTrackbar( TrackbarName, "addweighted",
                      &position_slider,
                      position_slider_max,
                      on_trackbar_position );
  on_trackbar_position(position_slider, 0 );

  waitKey(0);

  imwrite("blended.png", blended);
  cout << "Imagem blended.png salva" << endl;

  return 0;
}