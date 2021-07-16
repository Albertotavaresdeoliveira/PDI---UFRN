#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image, escalacinza, escalacinzaeq, histCinza, histCinzaeq;
  int width, height;
  VideoCapture cap;
  int nbins = 64;
  float range[] = {0, 255};
  const float *histrange = { range };
  bool uniform = true;
  bool acummulate = false;
  int key;

	cap.open(0);
  if(!cap.isOpened()){ cout << "cameras indisponiveis"; return -1; }

  cap.set(CAP_PROP_FRAME_WIDTH, 640);
  cap.set(CAP_PROP_FRAME_HEIGHT, 480);
  width = cap.get(CAP_PROP_FRAME_WIDTH);
  height = cap.get(CAP_PROP_FRAME_HEIGHT);

  cout << "largura = " << width << endl;
  cout << "altura  = " << height << endl;

  int histw = nbins, histh = nbins/2;
  Mat histImgCinza(histh, histw, CV_8UC1, Scalar(0));
  Mat histImgCinzaEq(histh, histw, CV_8UC1, Scalar(0));

  while(1){
    cap >> image;
    cvtColor(image,escalacinza, COLOR_RGB2GRAY);

    //Equalização
    equalizeHist(escalacinza, escalacinzaeq);

    calcHist(&escalacinza, 1, 0, Mat(), histCinza, 1, &nbins, &histrange, uniform, acummulate);
    calcHist(&escalacinzaeq, 1, 0, Mat(), histCinzaeq, 1, &nbins, &histrange, uniform, acummulate);

    normalize(histCinza, histCinza, 0, histImgCinza.rows, NORM_MINMAX, -1, Mat());
    normalize(histCinzaeq, histCinzaeq, 0, histImgCinzaEq.rows, NORM_MINMAX, -1, Mat());

    histImgCinza.setTo(Scalar(0));
    histImgCinzaEq.setTo(Scalar(0));

    for(int i=0; i<nbins; i++){
      line(histImgCinza, Point(i, histh), Point(i, histh-cvRound(histCinza.at<float>(i))), Scalar(255, 255, 255), 1, 8, 0);
      line(histImgCinzaEq, Point(i, histh), Point(i, histh-cvRound(histCinzaeq.at<float>(i))), Scalar(255, 255, 255), 1, 8, 0);
    }

    histImgCinza.copyTo(escalacinza(Rect(0, 0, nbins, histh)));
    histImgCinzaEq.copyTo(escalacinzaeq(Rect(0, 0, nbins, histh)));

    imshow("image", escalacinza);
    imshow("imagem escala de cinza", escalacinzaeq);
    key = waitKey(30);
    if(key == 27) break;
    if(key == 32) { imwrite("Imagem em cinza.png", escalacinza); imwrite("Equalizada.png", escalacinzaeq);break;}
  }
  return 0;
}

