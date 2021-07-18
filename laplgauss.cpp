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
        cout << "laplaciano" << endl;
        printmask(mask);
        break;
      case 'b':
        mask = Mat(3, 3, CV_32F, boost);
        break;
      case 'z'://laplaciano do gaussiano
        mask = Mat(5, 5, CV_32F, log);
        cout << "laplaciano do gaussiano" << endl;
        printmask(mask);
        break;   
      default:
        break;
    }
  }

  return 0;
}