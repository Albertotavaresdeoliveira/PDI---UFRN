#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv){
  cv::Mat original, image, semobjnasbordas, realce;
  int width, height, nobjects = 0, objcomburacos = 0;

  cv::Point p;
  image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
  original = image.clone();

  if(!image.data){ std::cout << "imagem nao carregou corretamente\n"; return(-1); }

  width=image.cols; height=image.rows;
  std::cout << width << "x" << height << std::endl;

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
      if(semobjnasbordas.at<uchar>(i,0) == 255){  p.x=0; p.y=i; cv::floodFill(semobjnasbordas,p,0); }
    }

  for(int i=0; i<height; i++){
      if(semobjnasbordas.at<uchar>(i,width-1) == 255){ p.x=width-1; p.y=i; cv::floodFill(semobjnasbordas,p,0); }
    }
    
  for(int j=0; j<width; j++){
      if(semobjnasbordas.at<uchar>(0,j) == 255){ p.x=j; p.y=0; cv::floodFill(semobjnasbordas,p,0); }
    }

  for(int j=0; j<width; j++){
      if(semobjnasbordas.at<uchar>(height-1,j) == 255){ p.x=j; p.y=height-1; cv::floodFill(semobjnasbordas,p,0); }
    }

    cv::imwrite("semobjnasbordas.png", semobjnasbordas);
    image = semobjnasbordas.clone();

    // busca objetos presentes
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 255){
        // achou um objeto
        p.x=j; p.y=i; nobjects++;
  		  // preenche o objeto com o contador
		    cv::floodFill(image,p,nobjects);
      }
    }
  }

    cv::imwrite("labeling.png", image);

    //alterar a cor de fundo da imagem de preto para branco
    p.x=0; p.y=0; cv::floodFill(image,p,255);

    cv::imwrite("corfundoalterado.png", image);

    cv::imshow("image fundo branco", image);

    //Contagem de objetos com buracos
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 0){ 
        objcomburacos++;
        for(int k=j; k<width; k++){
          if(image.at<uchar>(i,k) != 0){ p.x=k; p.y=i; cv::floodFill(image,p,0); cv::floodFill(image,p,255); break;}
        }
      }
    }
  }

  cv::imshow("image buracos preenchidos", image);

  std::cout << "a figura tem " << nobjects << " bolhas, sendo " << objcomburacos << " bolhas com buracos\n";
  //cv::equalizeHist(image, realce);
  cv::imshow("imagem original clone", original);
  //cv::imshow("image", image);
  cv::imshow("sem objetos que tocam as bordas", semobjnasbordas);
  cv::imwrite("output.png", image);
  cv::waitKey();
  return 0;
}
