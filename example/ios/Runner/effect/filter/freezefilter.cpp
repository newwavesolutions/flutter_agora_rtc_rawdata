#include "./freezefilter.h"

void FreezeFilter::init(const effect_config &config) {
  std::vector<double> x{0, 25, 122, 165, 256};
  std::vector<double> y{0, 21, 153, 206, 256};
  std::vector<double> x2{0, 25, 122, 165, 256};
  std::vector<double> y2{0, 21,  153, 206, 256};
  std::vector<double> x3{0, 41, 183, 256};
  std::vector<double> y3{0, 28,  105, 165};
  std::vector<double> x4{0, 128, 221, 256};
  std::vector<double> y4{0, 118, 215, 256};
  this->b_ch_lut = createLUT(x, y);
  this->g_ch_lut = createLUT(x2, y2);
  this->r_ch_lut = createLUT(x3, y3);
  this->v_ch_lut = createLUT(x4, y4);
}

cv::Mat FreezeFilter::apply(cv::Mat &to, const void *data) {
  double t = (double)cv::getTickCount();

#ifdef __ANDROID__
  int ir = 0, ib = 2, ig=1, code = cv::COLOR_RGB2HSV;
#else
  int ir = 2, ib = 0, ig=1, code = cv::COLOR_BGR2HSV;
#endif
  // warming filter: increase blue, decrease red
  std::vector<cv::Mat> bgr;
  cv::split(to, bgr);
  cv::Mat r, b, g;
  cv::LUT(bgr[ir], this->r_ch_lut, r);
  cv::LUT(bgr[ib], this->b_ch_lut, b);
  cv::LUT(bgr[ig], this->g_ch_lut, g);
  r.convertTo(r, bgr[ir].type());
  b.convertTo(b, bgr[ib].type());
  g.convertTo(g, bgr[ig].type());
#ifdef __ANDROID__
  std::vector<cv::Mat> bgr2{r, g, b};
#else
  std::vector<cv::Mat> bgr2{b, g, r};
#endif
  cv::merge(bgr2, to);

  // decrease color saturation
  cv::Mat img_hsv;
  std::vector<cv::Mat> hsv;
  cv::cvtColor(to, img_hsv, code);
  cv::split(to, hsv);
  cv::Mat s;
  cv::LUT(hsv[1], this->v_ch_lut, s);
  s.convertTo(s, hsv[1].type());
  hsv[1] = s;
  cv::merge(hsv, to);
  
  t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
#ifdef IMG_DEBUG_LOG
  std::cout << "duration applying sunset filter : " << t*1000 << " milliseconds" << std::endl;
#endif
  return to;
}

FreezeFilter::~FreezeFilter() {
}
