## Alberto Tavares de Oliveira

Página dedicada às respostas dos exercicios de programação da disciplina.

### Questão 1 - Manipulando pixels em uma imagem

Imagem original:

![biel](https://user-images.githubusercontent.com/56025096/124983972-0a7c9480-e00f-11eb-8aae-d0994bdaf1bb.png)

Código [regions.cpp](https://github.com/Albertotavaresdeoliveira/PDI-UFRN/blob/gh-pages/regions.cpp):

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

Código [trocaregioes.cpp]()

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
```
Imagem obtida:

![quadrantes trocados](https://user-images.githubusercontent.com/56025096/125082069-1d3daa80-e09d-11eb-855a-3207b8a69225.png)


For more details see [GitHub Flavored Markdown](https://guides.github.com/features/mastering-markdown/).

### Jekyll Themes

Your Pages site will use the layout and styles from the Jekyll theme you have selected in your [repository settings](https://github.com/Albertotavaresdeoliveira/PDI---UFRN/settings/pages). The name of this theme is saved in the Jekyll `_config.yml` configuration file.

### Support or Contact

Having trouble with Pages? Check out our [documentation](https://docs.github.com/categories/github-pages-basics/) or [contact support](https://support.github.com/contact) and we’ll help you sort it out.
