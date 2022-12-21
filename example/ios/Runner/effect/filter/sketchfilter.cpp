#include "./sketchfilter.h"

void SketchFilter::init(const effect_config &config) {
  cv::Mat img_tmp = cv::imread(config.image);
  cv::cvtColor(img_tmp, img_bg, cv::COLOR_BGR2GRAY);
//  std::istringstream iss(config.meta);
//  std::string sw, sh;
//  std::getline(iss, sw, 'x');
//  std::getline(iss, sh, 'x');
//  int w = std::stoi(sw);
//  int h = std::stoi(sh);
//  cv::resize(img_bg, img_bg, cv::Size(w, h));
}

void dodgeNaive(cv::Mat &src, cv::Mat &src2, cv::Mat &out) {
  for (int i=0; i<src.rows; i++) {
    for (int j=0; j<src.cols; j++) {
      if (src2.data[i*src.cols + j] == 255) {
        out.data[i*src.cols + j] = 255;
      } else {
        uint tmp = (src.data[i*src.cols + j] << 8) / (255 - src2.data[i*src.cols + j]);
        // if (tmp > 255) tmp = 255;
        out.data[i*src.cols + j] = (tmp>255)?255:tmp;
      }
    }
  }
}

cv::Mat SketchFilter::apply(cv::Mat &to, const void *data) {
  double t = (double)cv::getTickCount();
  if (img_bg.cols != to.cols || img_bg.rows != to.rows) {
    cv::resize(img_bg, img_bg, cv::Size(to.cols, to.rows));
  }
  // to gray image
  cv::Mat img_gray;
  cv::cvtColor(to, img_gray, cv::COLOR_BGR2GRAY);
  // get a negative
  cv::Mat img_gray_inv = 255 - img_gray;
  // apply gaussian blur
  cv::Mat img_blur;
  cv::GaussianBlur(img_gray_inv, img_blur, cv::Size(21, 21), 0, 0);
  cv::Mat img_blend(to.size(), 0);
#ifdef __APPLE__
    dodgeNaive(img_gray, img_blur, img_blend);
#else
    cv::divide(img_gray, 255-img_blur, img_blend, 255);
#endif
  cv::multiply(img_blend, img_bg, img_blend, 1.0/256);
  cv::cvtColor(img_blend, to, cv::COLOR_GRAY2RGB);
  t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
#ifdef IMG_DEBUG_LOG
  std::cout << "duration applying sketch filter : " << t*1000 << " milliseconds" << std::endl;
#endif
  return to;
}

SketchFilter::~SketchFilter() {
}
