#include "./coolfilter.h"

void CoolFilter::init(const effect_config &config) {
  std::vector<double> x{0, 64, 128, 192, 256};
  std::vector<double> y{0, 70, 140, 210, 256};
  std::vector<double> x2{0, 64, 128, 192, 256};
  std::vector<double> y2{0, 30,  80, 120, 192};
  this->incr_ch_lut = createLUT(x, y);
  this->decr_ch_lut = createLUT(x2, y2);
}

cv::Mat CoolFilter::apply(cv::Mat &to, const void *data) {
  double t = (double)cv::getTickCount();

#ifdef __ANDROID__
  int ir = 0, ib = 2, code = cv::COLOR_RGB2HSV;
#else
  int ir = 2, ib = 0, code = cv::COLOR_BGR2HSV;
#endif
  // warming filter: increase blue, decrease red
  std::vector<cv::Mat> bgr;
  cv::split(to, bgr);
  cv::Mat r, b;
  cv::LUT(bgr[ir], this->decr_ch_lut, r);
  cv::LUT(bgr[ib], this->incr_ch_lut, b);
  r.convertTo(r, bgr[ir].type());
  b.convertTo(b, bgr[ib].type());
#ifdef __ANDROID__
  std::vector<cv::Mat> bgr2{r, bgr[1], b};
#else
  std::vector<cv::Mat> bgr2{b, bgr[1], r};
#endif
  cv::merge(bgr2, to);

  // decrease color saturation
  cv::Mat img_hsv;
  std::vector<cv::Mat> hsv;
  cv::cvtColor(to, img_hsv, code);
  cv::split(to, hsv);
  cv::Mat s;
  cv::LUT(hsv[1], decr_ch_lut, s);
  s.convertTo(s, hsv[1].type());
  hsv[1] = s;
  cv::merge(hsv, to);
  
  t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
#ifdef IMG_DEBUG_LOG
  std::cout << "duration applying cool filter : " << t*1000 << " milliseconds" << std::endl;
#endif
  return to;
}

CoolFilter::~CoolFilter() {
}
