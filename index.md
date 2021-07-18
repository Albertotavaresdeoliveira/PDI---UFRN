### Alberto Tavares de Oliveira
### Francisco Bianor Souza de Medeiros

## Sumário
- [Sumário](#sumário)
  - [1 - Manipulando pixels em uma imagem](#1---manipulando-pixels-em-uma-imagem)
    - [Exercício 1.1 - regions.cpp](#exercício-11---regionscpp)
    - [Exercício 1.2 - trocaregioes.cpp](#exercício-12---trocaregioescpp)
  - [2 - Preenchendo regiões](#2---preenchendo-regiões)
    - [Exercício 2.1 - labelingRGB.cpp](#exercício-21---labelingrgbcpp)
    - [Exercício 2.2 - labelingholes.cpp](#exercício-22---labelingholescpp)
  - [3 - Manipulação de histogramas](#3---manipulação-de-histogramas)
    - [Exercício 3.1 - equalize.cpp](#exercício-31---equalizecpp)
    - [Exercício 3.2 - motiondetector.cpp](#exercício-32---motiondetectorcpp)
  - [4 - Filtragem no domínio espacial i](#4---filtragem-no-domínio-espacial-i)
    - [Exercício 4.1 - laplgauss.cpp](#exercício-41---laplgausscpp)
  - [5 - Filtragem no domínio espacial ii](#5---filtragem-no-domínio-espacial-ii)
    - [Exercício 5.1 - tiltshift.cpp](#exercício-51---tiltshiftcpp)
    - [Exercício 5.2 - tiltshiftvideo.cpp](#exercício-52---tiltshiftvideocpp)
  - [Jekyll Themes](#jekyll-themes)
  - [Support or Contact](#support-or-contact)

Página dedicada às respostas dos exercicios de programação da disciplina.

### 1 - Manipulando pixels em uma imagem

#### Exercício 1.1 - regions.cpp

Imagem original:

![biel.png](https://user-images.githubusercontent.com/56025096/124983972-0a7c9480-e00f-11eb-8aae-d0994bdaf1bb.png "biel.png") 

Código [regions.cpp](https://raw.githubusercontent.com/Albertotavaresdeoliveira/PDI-UFRN/gh-pages/regions.cpp):

```c++
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
```
Imagem obtida usando os pontos P1(20, 50) e P2(200, 200):

![negativo da imagem](https://user-images.githubusercontent.com/56025096/124983322-2e8ba600-e00e-11eb-8a58-b124cd974f17.png)


#### Exercício 1.2 - trocaregioes.cpp

O código a seguir foi implementado de forma a trocar os quadrantes de uma imagem ao longo das diagonais. Foi usada a imagem **biel.png** como exemplo.

Código [trocaregioes.cpp](https://raw.githubusercontent.com/Albertotavaresdeoliveira/PDI-UFRN/gh-pages/trocaregioes.cpp):

```c++
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int, char**){
  cv::Mat image, quadrantes;
  int width, height;

  image= cv::imread("biel.png",cv::IMREAD_GRAYSCALE);
  if(!image.data)
    std::cout << "nao abriu a imagem" << std::endl;

  cv::namedWindow("janela", cv::WINDOW_AUTOSIZE);

  quadrantes = image.clone();

  width=image.cols;
  height=image.rows;
  std::cout << width << "x" << height << std::endl;

  for(int i=0; i<height/2; i++){
    for(int j=0; j<width/2; j++){
      quadrantes.at<uchar>(i,width/2+j) = image.at<uchar>(height/2+i,j); //1° quadrante
      quadrantes.at<uchar>(i,j) = image.at<uchar>(height/2+i,width/2+j); //2° quadrante
      quadrantes.at<uchar>(height/2+i,j) = image.at<uchar>(i,width/2+j); //3° quadrante
      quadrantes.at<uchar>(height/2+i,width/2+j) = image.at<uchar>(i,j); //4° quadrante
    }
  }

  cv::imshow("imagem original", image);
  cv::imshow("quadrantes trocados", quadrantes);
  cv::imwrite("quadrantes trocados.png", quadrantes);
  cv::waitKey();
  return 0;
}
```
Imagem obtida:

![quadrantes trocados](https://user-images.githubusercontent.com/56025096/125082069-1d3daa80-e09d-11eb-855a-3207b8a69225.png)

### 2 - Preenchendo regiões

#### Exercício 2.1 - labelingRGB.cpp

Uma forma de identificar mais de 255 objetos na cena usando a rotulação é rotular os objetos com outras cores, ao invés de somente na escala de cinza. Para esse exemplo foi implementado o código abaixo.

Imagem utilizada:

![bolhas](https://user-images.githubusercontent.com/56025096/125695403-8a1ca6de-6ccf-4bbb-b1c0-709586fdadcf.png)   ![labelingRGB](https://user-images.githubusercontent.com/56025096/125695045-529364a3-5ecc-4914-bb7b-396731c7bd7b.png)

Código [labelingRGB.cpp](https://raw.githubusercontent.com/Albertotavaresdeoliveira/PDI-UFRN/gh-pages/labelingRGB.cpp):

```c++
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
```

#### Exercício 2.2 - labelingholes.cpp

O segundo código foi implementado para a rotulação de regiões e contagem de objetos, que não tocam na borda da imagem, com e sem buracos internos. Para o algorítimo foi previsto também que objetos com mais de um buraco também podem existir. 

Sequência do processamento da imagem:

![bolhas2buracos](https://user-images.githubusercontent.com/56025096/125688922-2fa6f0f9-b672-416b-a1c7-cf7fe9c7365a.png)   ![semobjnasbordas](https://user-images.githubusercontent.com/56025096/125699222-f8f1c3d2-3be5-470b-9c54-383db962f55c.png)   ![labeling](https://user-images.githubusercontent.com/56025096/125699305-09be40c6-1b2f-4e40-9d9d-ac9e2e5ec992.png)

![corfundoalterado](https://user-images.githubusercontent.com/56025096/125700096-5b16bcac-c760-4698-931f-5b0c81dc29b3.png)   ![output](https://user-images.githubusercontent.com/56025096/125699930-ed34b79b-2718-4cd2-8641-5b5d8c482f4d.png)


Abaixo é mostrado a saída no terminal de comando:

![Captura de tela de 2021-07-14 17-26-01](https://user-images.githubusercontent.com/56025096/125688485-3642f9b5-4e35-41b5-b4d9-7296ad32c969.png) 

Código [labelingholes.cpp](https://raw.githubusercontent.com/Albertotavaresdeoliveira/PDI-UFRN/gh-pages/labelingholes.cpp):

```c++
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

    //alterar a cor de fundo da imagem de preto para branco
    p.x=0; p.y=0; cv::floodFill(image,p,255);

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
  cv::imwrite("labeling.png", image);
  cv::waitKey();
  return 0;
}
```

### 3 - Manipulação de histogramas

#### Exercício 3.1 - equalize.cpp

Código [equalize.cpp](https://raw.githubusercontent.com/Albertotavaresdeoliveira/PDI-UFRN/gh-pages/equalize.cpp):

```c++
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
```

#### Exercício 3.2 - motiondetector.cpp

Código [motiondetector.cpp](https://raw.githubusercontent.com/Albertotavaresdeoliveira/PDI-UFRN/gh-pages/motiondetector.cpp):

```c++
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image, escalacinza, histAnterior, histNovo;
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

  Point Centro(width/2, height/2);

  cout << "largura = " << width << endl;
  cout << "altura  = " << height << endl;

  int histw = nbins, histh = nbins/2;
  Mat histImgAnterior(histh, histw, CV_8UC1, Scalar(0));
  Mat histImgNovo(histh, histw, CV_8UC1, Scalar(0));

  double Dif = 0, Limiar = 0.999;
  
  cap >> image;
  cvtColor(image,escalacinza, COLOR_RGB2GRAY);
  calcHist(&escalacinza, 1, 0, Mat(), histAnterior, 1, &nbins, &histrange, uniform, acummulate);
  normalize(histAnterior, histAnterior, 0, histImgAnterior.rows, NORM_MINMAX, -1, Mat());

  while(1){
    cap >> image;
    cvtColor(image,escalacinza, COLOR_RGB2GRAY);
    calcHist(&escalacinza, 1, 0, Mat(), histNovo, 1, &nbins, &histrange, uniform, acummulate);
    normalize(histNovo, histNovo, 0, histImgNovo.rows, NORM_MINMAX, -1, Mat());

    histImgNovo.setTo(Scalar(0));

    Dif = compareHist(histAnterior, histNovo, HISTCMP_CORREL);
    //Dif = compareHist(histAnterior, histNovo, HISTCMP_CHISQR);
    //Dif = compareHist(histAnterior, histNovo, HISTCMP_INTERSECT);
    //Dif = compareHist(histAnterior, histNovo, HISTCMP_BHATTACHARYYA);
    //Dif = compareHist(histAnterior, histNovo, HISTCMP_HELLINGER );

    if(Dif<=Limiar) { circle(escalacinza, Centro, 50, Scalar(255, 255, 255), FILLED, LINE_8 ); }

    for(int i=0; i<nbins; i++){
      line(histImgNovo, Point(i, histh), Point(i, histh-cvRound(histNovo.at<float>(i))), Scalar(255, 255, 255), 1, 8, 0);
    }

    histImgNovo.copyTo(escalacinza(Rect(0, 0, nbins, histh)));
    
    histAnterior = histNovo.clone();

    imshow("image", escalacinza);
    key = waitKey(30);
    if(key == 27) break;
  }
  return 0;
}
```

### 4 - Filtragem no domínio espacial i

#### Exercício 4.1 - laplgauss.cpp

Abaixo é mostrado a filtragem de imagens capturadas pela câmera usando o filtro laplaciano seguido do laplaciano do gaussiano e as máscaras usadas no processo:

![](https://media.giphy.com/media/xAOF1p6gl8XpKdqs9C/giphy.gif)

Código [laplgauss.cpp](https://raw.githubusercontent.com/Albertotavaresdeoliveira/PDI-UFRN/gh-pages/laplgauss.cpp):

```c++
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void printmask(Mat &m) {
  for (int i = 0; i < m.size().height; i++) {
    for (int j = 0; j < m.size().width; j++) {
      std::cout << m.at<float>(i, j) << ",";
    }
    std::cout << "\n";
  }
}

int main(int, char **) {
  VideoCapture cap;  // open the default camera
  float media[] = {0.1111, 0.1111, 0.1111, 0.1111, 0.1111,
                   0.1111, 0.1111, 0.1111, 0.1111};
  float gauss[] = {0.0625, 0.125,  0.0625, 0.125, 0.25,
                   0.125,  0.0625, 0.125,  0.0625};
  float horizontal[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
  float vertical[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
  float laplacian[] = {0, -1, 0, -1, 4, -1, 0, -1, 0};
  float boost[] = {0, -1, 0, -1, 5.2, -1, 0, -1, 0};
  float log[] = {0, 0, 1, 0, 0, 0, 1, 2, 1, 0, 1, 2, -16, 2, 1, 0, 1, 2, 1, 0, 0, 0, 1, 0, 0}; //laplaciano do gaussiano

  Mat frame, framegray, frame32f, frameFiltered;
  Mat mask(3, 3, CV_32F);
  Mat result;
  double width, height;
  int absolut;
  char key;

  cap.open(0);

  if (!cap.isOpened())  // check if we succeeded
    return -1;

  cap.set(CAP_PROP_FRAME_WIDTH, 640);
  cap.set(CAP_PROP_FRAME_HEIGHT, 480);
  width = cap.get(CAP_PROP_FRAME_WIDTH);
  height = cap.get(CAP_PROP_FRAME_HEIGHT);
  std::cout << "largura=" << width << "\n";
  ;
  std::cout << "altura =" << height << "\n";
  ;
  std::cout << "fps    =" << cap.get(CAP_PROP_FPS) << "\n";
  std::cout << "format =" << cap.get(CAP_PROP_FORMAT) << "\n";

  namedWindow("filtroespacial", WINDOW_NORMAL);
  namedWindow("original", WINDOW_NORMAL);

  mask = Mat(3, 3, CV_32F, media);

  absolut = 1;  // calcs abs of the image

  for (;;) {
    cap >> frame;  // get a new frame from camera
    cvtColor(frame, framegray, COLOR_BGR2GRAY);
    flip(framegray, framegray, 1);
    imshow("original", framegray);
    framegray.convertTo(frame32f, CV_32F);
    filter2D(frame32f, frameFiltered, frame32f.depth(), mask,
                 Point(1, 1), 0);
    if (absolut) {
      frameFiltered = abs(frameFiltered);
    }

    frameFiltered.convertTo(result, CV_8U);

    imshow("filtroespacial", result);

    key = (char)waitKey(10);
    if (key == 27) break;  // esc pressed!
    switch (key) {
      case 'a':
        absolut = !absolut;
        break;
      case 'm':
        mask = Mat(3, 3, CV_32F, media);
        printmask(mask);
        break;
      case 'g':
        mask = Mat(3, 3, CV_32F, gauss);
        printmask(mask);
        break;
      case 'h':
        mask = Mat(3, 3, CV_32F, horizontal);
        printmask(mask);
        break;
      case 'v':
        mask = Mat(3, 3, CV_32F, vertical);
        printmask(mask);
        break;
      case 'l':
        mask = Mat(3, 3, CV_32F, laplacian);
        printmask(mask);
        break;
      case 'b':
        mask = Mat(3, 3, CV_32F, boost);
        break;
      case 'z'://laplaciano do gaussiano
        mask = Mat(5, 5, CV_32F, log);
        printmask(mask);
        break;   
      default:
        break;
    }
  }
  return 0;
}
```

### 5 - Filtragem no domínio espacial ii

#### Exercício 5.1 - tiltshift.cpp

#### Exercício 5.2 - tiltshiftvideo.cpp

For more details see [GitHub Flavored Markdown](https://guides.github.com/features/mastering-markdown/).

### Jekyll Themes

Your Pages site will use the layout and styles from the Jekyll theme you have selected in your [repository settings](https://github.com/Albertotavaresdeoliveira/PDI---UFRN/settings/pages). The name of this theme is saved in the Jekyll `_config.yml` configuration file.

### Support or Contact

Having trouble with Pages? Check out our [documentation](https://docs.github.com/categories/github-pages-basics/) or [contact support](https://support.github.com/contact) and we’ll help you sort it out.
