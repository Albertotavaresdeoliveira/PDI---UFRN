#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  cv::Mat original, image, semobjnasbordas, realce;
  cv::Vec3b val, B, P, C;
  int width, height, nobjects = 0;

  //Branco
  B[0] = 255; B[1] = 255; B[2] = 255;
  //Preto
  P[0] = 0; P[1] = 0; P[2] = 0;
  //Cinza
  C[0] = 127; C[1] = 127; C[2] = 127;

  cv::Point p;
  //image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
  image= cv::imread(argv[1],cv::IMREAD_COLOR);
  original = image.clone();

  if(!image.data){ std::cout << "imagem nao carregou corretamente\n"; return(-1); }

  width=image.cols; height=image.rows;
  cout << width << "x" << height << endl;

  p.x=0; p.y=0;

  //Processo para gerar imagem apenas em preto e branco (elimina tons intermediários)
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) >= 127) { image.at<uchar>(i,j) = 255; }
      else { image.at<uchar>(i,j) = 0; }
    }
  }

  semobjnasbordas = image.clone();

  //Removendo os objetos das 4 bordas
  for(int i=0; i<height; i++){
      if(semobjnasbordas.at<Vec3b>(i,0) == B){  p.x=0; p.y=i; cv::floodFill(semobjnasbordas,p,P); }
    }

  for(int i=0; i<height; i++){
      if(semobjnasbordas.at<Vec3b>(i,width-1) == B){ p.x=width-1; p.y=i; cv::floodFill(semobjnasbordas,p,P); }
    }
    
  for(int j=0; j<width; j++){
      if(semobjnasbordas.at<Vec3b>(0,j) == B){ p.x=j; p.y=0; cv::floodFill(semobjnasbordas,p,P); }
    }

  for(int j=0; j<width; j++){
      if(semobjnasbordas.at<Vec3b>(height-1,j) == B){ p.x=j; p.y=height-1; cv::floodFill(semobjnasbordas,p,P); }
    }

    image = semobjnasbordas.clone();

    val[0] = 0; //B
    val[1] = 0; //G
    val[2] = 0; //R

    // busca objetos presentes
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<Vec3b>(i,j) == B){
        // achou um objeto
        p.x=j; p.y=i; nobjects++;
        val[0] = 0;
        val[1] = nobjects % 255;
        val[2] = nobjects - ((nobjects % 255) * 255);

  		  // preenche o objeto com o contador
        //cv::floodFill(image,p,cv::Scalar((val[0]), (val[1]), (val[2]), 0));
        //cv::floodFill(image,p,cv::Scalar((val[0]), (val[1]), (val[2]), 0));
        cv::floodFill(image,p,val);
      }
    }
  }

  cout << "a figura tem " << nobjects << " bolhas" << endl;
  //cv::equalizeHist(image, realce);
  cv::imshow("imagem original clone", original);
  cv::imshow("image", image);
  cv::imshow("sem objetos que tocam as bordas", semobjnasbordas);
  cv::imwrite("labelingRGB.png", image);
  cv::waitKey();
  return 0;
}