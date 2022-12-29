#include "simple_smooth_skin_effect.h"
#include "../common/blend.h"

cv::Scalar lower(0, 48, 80);
cv::Scalar upper(20, 255, 255);

void SimpleSmoothSkinEffect::init(const effect_config &config) {
}

cv::Mat SimpleSmoothSkinEffect::apply(cv::Mat &to, const void *data) {
  double t = (double)cv::getTickCount();

//  cv::Mat converted, kernel, skinMask, skin, blur, tmp;
//  cv::cvtColor(to, converted, cv::COLOR_BGR2HSV);
//  cv::inRange(converted, lower, upper, skinMask);
  //kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(11, 11));
  //cv::erode(skinMask, skinMask, kernel, cv::Point(-1,-1), 2);
  //cv::dilate(skinMask, skinMask, kernel, cv::Point(-1,-1), 2);
  //cv::GaussianBlur(skinMask, skinMask, cv::Size(3, 3), 0);
  //cv::bitwise_and(to, to, skin, skinMask);
  //cv::blur(skin, blur, cv::Size(4, 4));
  //blur = blur * 1.15 + 25;
  //cv::cvtColor(blur, tmp, cv::COLOR_HSV2BGR);
  //alpha_blend_8u_self(tmp, to);
  //std::vector<std::vector<cv::Point> > contours;
  //cv::findContours(skinMask, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
  to = to * 1.05 + 25;
//  cv::Rect r = cv::boundingRect(skinMask);
//  if (r.width > 0 && r.height > 0) {
//    std::cout << r.x << ", " << r.y << ", " << r.width << ", " << r.height << std::endl;
//    cv::Mat roi = to(r);
//    cv::blur(roi, roi, cv::Size(3, 3));
//  }
//  roi = roi * 1.05 + 25;
  
  t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
#ifdef IMG_DEBUG_LOG
  std::cout << "simple smooth skin effect : " << t * 1000 << " milliseconds" << std::endl;
#endif
  return to;
}

SimpleSmoothSkinEffect::~SimpleSmoothSkinEffect() { std::cout << "destroy Simple smooth skin effect" << std::endl; }
