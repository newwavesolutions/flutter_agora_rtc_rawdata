#include "./cartoonfilter.h"

void CartoonFilter::init(const effect_config &config) {
}

cv::Mat CartoonFilter::apply(cv::Mat &to, const void *data) {
  double t = (double)cv::getTickCount();
  int numDownSamples = 2;       // number of downscaling steps
  int numBilateralFilters = 1;  // number of bilateral filtering steps
  cv::Mat img_color = to.clone();
  for (int i=0; i<numDownSamples; i++) {
    cv::pyrDown(img_color, img_color);
  }
  cv::Mat tmp;
  for (int i=0; i<numBilateralFilters; i++) {
    tmp = img_color.clone();
    cv::bilateralFilter(tmp, img_color, 9, 9, 7);
  }
  for (int i=0; i<numDownSamples; i++) {
    cv::pyrUp(img_color, img_color);
  }
  cv::Mat img_gray;
  cv::cvtColor(to, img_gray, cv::COLOR_BGR2GRAY);
  cv::Mat img_blur;
  // cv::medianBlur(img_gray, img_blur, 7);
  cv::GaussianBlur(img_gray, img_blur, cv::Size(13, 13), 0, 0);
  cv::Mat img_edge;
  cv::adaptiveThreshold(img_blur, img_edge, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 9, 2);
  cv::cvtColor(img_edge, img_edge, cv::COLOR_GRAY2RGB);
  cv::bitwise_and(img_color, img_edge, to);
  t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
#ifdef IMG_DEBUG_LOG
  std::cout << "duration applying cartoon filter : " << t*1000 << " milliseconds" << std::endl;
#endif
  return to;
}

CartoonFilter::~CartoonFilter() {
}
