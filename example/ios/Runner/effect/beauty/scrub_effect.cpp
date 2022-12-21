#include "scrub_effect.h"

void ScrubEffect::init(const effect_config &config) {
}

cv::Mat ScrubEffect::apply(cv::Mat &to, const void *data) {
  double t = (double)cv::getTickCount();
  beauty_data const *bData = static_cast<beauty_data const*>(data);

  cv::Mat src = to;


  cv::Mat blurMat;
  float softenRatio = bData->intensity / 100.0f;
  float maxSigma = 50;
  float minSigma = 5;
  float sigmaColor = minSigma + softenRatio * (maxSigma - minSigma);
  float sigmaSpace = sigmaColor;
  int dmax = 7, dmin = 3;
  int d = dmin + softenRatio * (dmax - dmin);
  cv::Rect r = cv::boundingRect(*(bData->pvf));
  if ((*(bData->pvf)).size() == 68) {
    r.y -= r.height / 2;
    if (r.y < 0) r.y = 0;
    r.height += r.height / 2;
  }
  else {
    r.x -= (int)(r.width * 0.5);
    r.y -= (int)(r.height * 1.25);
    r.width *= 2;
    r.height = (int)(r.height * 3.25);
  }

  r &= cv::Rect(0, 0, src.cols, src.rows);
  if (r.width > 0 && r.height > 0) {
    cv::bilateralFilter(src(r), blurMat, d, sigmaColor, sigmaSpace);
    blurMat.copyTo(src(r));
  }
  t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
#ifdef IMG_DEBUG_LOG
  std::cout << "scrub effect : " << t * 1000 << " milliseconds" << std::endl;
#endif
  return src;
}

ScrubEffect::~ScrubEffect() { std::cout << "destroy ScrubEffect" << std::endl; }
