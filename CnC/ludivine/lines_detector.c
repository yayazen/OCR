#include "hough.hpp"
#include "opencv2/highgui/highgui.hpp" // for imread

using namespace cv;

int main(void)
{
  //load image and so on

  Canny(src, dst, 50, 200, 3);

  void* lines;
  //vector<Vec2f> lines;
  
   cvHoughLines2(ds(image)t, lines, 1, CV_PI/180, 100, 0, 0 );

  for( size_t i = 0; i < lines.size(); i++ )
{
  float rho = lines[i][0], theta = lines[i][1];
  Point pt1, pt2;
  double a = cos(theta), b = sin(theta);
  double x0 = a*rho, y0 = b*rho;
  pt1.x = cvRound(x0 + 1000*(-b));
  pt1.y = cvRound(y0 + 1000*(a));
  pt2.x = cvRound(x0 - 1000*(-b));
  pt2.y = cvRound(y0 - 1000*(a));
  line( cdst, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
}
  
  return 0;
}






