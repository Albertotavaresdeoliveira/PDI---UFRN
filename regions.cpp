#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int, char**){
  cv::Mat image;
  cv::Point p1, p2;

  image= cv::imread("biel.png",cv::IMREAD_GRAYSCALE);
  if(!image.data)
    std::cout << "nao abriu a imagem" << std::endl;

    cout<<"Digite P1 e P2 (x1,y1,x2,y2):"<<endl;
    cin>>p1.x >> p1.y >> p2.x >> p2.y;

  cv::namedWindow("janela", cv::WINDOW_AUTOSIZE);

  for(int i=p1.x;i<p2.x;i++){
    for(int j=p1.y;j<p2.y;j++){
       image.at<uchar>(i,j) = 255 - image.at<uchar>(i,j);
    }
  }

  cv::imshow("negativo da imagem", image);
  cv::imwrite("negativo da imagem.png", image);
  cv::waitKey();
  return 0;
}