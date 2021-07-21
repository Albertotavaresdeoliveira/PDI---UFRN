#include <iostream>
#include <cstdio>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

double decaim;
int decaim_slider = 100;
int decaim_slider_max = 100;

int height_slider = 20;
int height_slider_max = 100;

int position_slider = 50;
int position_slider_max = 100;

int height_focus = 50; // altura da regiao central em foco
int position_focus = 50; // posição do centro da regiao central em foco

Mat image1, image2, blended, imgfiltered, imgpondered;
Mat imageTop; 

char TrackbarName[50];

int absolut = 1;

//float media[] = {0.1111, 0.1111, 0.1111, 0.1111, 0.1111, 0.1111, 0.1111, 0.1111, 0.1111};
//float media[] = {0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625, 0.0625};
/*
float media[] = { 0.04, 0.04, 0.04, 0.04, 0.04,
                  0.04, 0.04, 0.04, 0.04, 0.04, 
                  0.04, 0.04, 0.04, 0.04, 0.04, 
                  0.04, 0.04, 0.04, 0.04, 0.04, 
                  0.04, 0.04, 0.04, 0.04, 0.04 }; */

float media[] = { 0.0278, 0.0278, 0.0278, 0.0278, 0.0278, 0.0278, 
                  0.0278, 0.0278, 0.0278, 0.0278, 0.0278, 0.0278, 
                  0.0278, 0.0278, 0.0278, 0.0278, 0.0278, 0.0278, 
                  0.0278, 0.0278, 0.0278, 0.0278, 0.0278, 0.0278, 
                  0.0278, 0.0278, 0.0278, 0.0278, 0.0278, 0.0278, 
                  0.0278, 0.0278, 0.0278, 0.0278, 0.0278, 0.0278 };
                  
Mat gerarponderada(double l1, double l2, double d, int height, int width){
  Mat tmp(height, width, CV_8U); //CV_8U
  
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      tmp.at<uchar>(i,j) = 0.5 * ( tanh((i-l1)/d) - tanh((i-l2)/d) ) * 255; 
    }
  }
  cvtColor(tmp, tmp, IMREAD_COLOR);
  return tmp;
}

Mat borrar(Mat fromimage){
  Mat tmp;
  filter2D(fromimage, tmp, fromimage.depth(), Mat(6, 6, CV_32F, media), Point(-1, -1), 0);
  if (absolut) { tmp = abs(tmp); }
  return tmp;
}

Mat tiltshift(Mat input, int l1, int l2, int d){
  Mat borrado = borrar(input);

  //imshow("imagem original", input);
  //imshow("imagem borrada", borrado);
  
  Mat h1, h2, tsf, y1, y2;
  h1 = gerarponderada(l1, l2, d, input.rows, input.cols);
  bitwise_not(h1, h2);
  
  multiply(input, h1, y1, 0.007);
  multiply(borrado, h2, y2, 0.007);

  double a = 0.5;
  addWeighted(y1, 1-a, y2, a, 0.0, tsf);

  //imshow("imagem ponderada", h1);
  //imshow("imagem ponderada invertida", h2);
  //imshow("multiply 1", y1);
  //imshow("multiply 2", y2);

  return tsf;
}

//Força de decaimento da região borrada
void on_trackbar_blend(int, void*){
 decaim = (double) (decaim_slider*50/decaim_slider_max);

 position_focus = position_slider*image1.rows/100;
 int limit = height_slider*image1.rows/100;
 int L1 = position_focus-(limit/2);
 int L2 = position_focus+(limit/2);

 blended = tiltshift(image1, L1, L2, decaim);

 imshow("tiltshift", blended); 
}

//Altura da região central que entrará em foco
void on_trackbar_height(int, void*){
  on_trackbar_blend(height_slider,0);
}

//Posição vertical do centro da região em foco
void on_trackbar_position(int, void*){
  on_trackbar_blend(position_slider,0);
}

Mat frame;

int main(int argvc, char** argv){

  VideoCapture videoinput(argv[1]);

  //tiltshift(Mat input, int l1, int l2, int d);

  while(videoinput.read(frame)){
    imshow("video de entrada", frame);
    if(waitKey(25) >= 0){ break; }
  }

  //imshow("frame", frame);
  waitKey(0);

  if(!videoinput.isOpened()) { cout<< "Erro ao carregar o video de entrada" << endl; return -1; }

  image1 = frame.clone();

  //imshow("image2 inversa", image2);
  //waitKey(0);
  
  namedWindow("tiltshiftvideo", 1);

  std::sprintf( TrackbarName, "Alpha x %d", decaim_slider_max );
  createTrackbar( TrackbarName, "tiltshiftvideo",
                      &decaim_slider,
                      decaim_slider_max,
                      on_trackbar_blend );
  on_trackbar_blend(decaim_slider, 0 );

  std::sprintf( TrackbarName, "Altura x %d", height_slider_max );
  createTrackbar( TrackbarName, "tiltshiftvideo",
                      &height_slider,
                      height_slider_max,
                      on_trackbar_height );
  on_trackbar_height(height_slider, 0 );

  std::sprintf( TrackbarName, "Posicao x %d", position_slider_max );
  createTrackbar( TrackbarName, "tiltshiftvideo",
                      &position_slider,
                      position_slider_max,
                      on_trackbar_position );
  on_trackbar_position(position_slider, 0 );

  waitKey(0);

  int d = (double) (decaim_slider*50/decaim_slider_max);
  //position_focus = position_slider*image1.rows/100;
  int lim = height_slider*image1.rows/100;
  int l1 = position_focus-(lim/2);
  int l2 = position_focus+(lim/2);

  while(videoinput.read(frame)){
    imshow("video de entrada", tiltshift(frame, l1, l2, d));
    if(waitKey(25) >= 0){ break; }
  }

  videoinput.release();
  destroyAllWindows();
  return 0;
}