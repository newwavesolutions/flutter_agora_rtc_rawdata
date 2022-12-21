#include "lighten_effect.h"
#include "scrub_effect.h"
#include "../common/blend.h"

void LightenEffect::init(const effect_config &config) {
}

cv::Mat LightenEffect::apply(cv::Mat &to, const void *data) {
  double t = (double)cv::getTickCount();
  beauty_data const *bData = static_cast<beauty_data const*>(data);
  PointVecf& pvf = *(bData->pvf);
  cv::Mat src = to;
  const int& intensity = bData->intensity;

  cv::Mat blurMat;
  float softenRatio = intensity / 100.0f;
  float alphaMax = 0.8, alphaMin = 0;
  float alpha = (alphaMax - alphaMin) * softenRatio + alphaMin;
  float betaMax = 0, betaMin = 0;
  float beta = (betaMax - betaMin) * softenRatio + betaMin;
  PointVecf pvcircle = pointAllCircle(pvf);
  cv::Rect r = cv::boundingRect(pvcircle);
  //{
  //  // drawing 
  //  cv::Mat drawing = src.clone();
  //  std::vector<PointVec> pk = { pvcircle };
  //  cv::polylines(drawing, pk, true, cv::Scalar(0, 0, 255), 3);
  //  cv::imshow("round face", drawing);
  //}
  if (pvf.size() == 68) {
    r.x -= r.width * 0.3;
    r.y -= r.height / 2;
    if (r.y < 0) r.y = 0;
    r.height = r.height * 2;
    r.width *= 1.6;
  }
  else {
    r.x -= (int)(r.width * 0.5);
    r.y -= (int)(r.height * 1.25);
    r.width *= 2.5;
    r.height = (int)(r.height * 3.5);
  }
  r |= cv::boundingRect(pvcircle);
  r &= cv::Rect(0, 0, src.cols, src.rows);

  cv::Mat hs;
  cv::cvtColor(src(r), hs, cv::COLOR_BGR2HSV);
  cv::Mat mask;
  bool useSkin = false;
  if (useSkin) {
    cv::Vec3b lower(0, 48, 80);
    cv::Vec3b upper(20, 255, 255);    
    cv::inRange(hs, lower, upper, mask);
  }
  else {
    mask = cv::Mat(r.height, r.width, CV_8UC1);
    mask.setTo(255);
  }
  
  scale_channel_u(hs, 2, 1.9);
  
  cv::Mat effect;
  cv::cvtColor(hs, effect, cv::COLOR_HSV2BGR);
  //effect.copyTo(src(r));
  cv::Mat maskBlur = mask;
  {
    //int dsize = std::max(int(mask.cols * 0.1), 1) * 2 + 1;
    //cv::GaussianBlur(mask, maskBlur, cv::Size(dsize, dsize), 0, 0);
    maskBlur.convertTo(maskBlur, CV_32FC1, 1 / 255.0f);
  }

  cv::Mat maskoval = cv::Mat::zeros(mask.size(), CV_32FC1);
  {
    for (int i = 0; i < pvcircle.size(); ++i)
    {
      pvcircle[i].x -= r.x;
      pvcircle[i].y -= r.y;
    }
    std::vector<PointVec> pk = { pvf2pv(pvcircle) };
    cv::fillPoly(maskoval, pk, 1.0);
    //int dsize = std::max(int(mask.cols * 0.1), 1) * 2 + 1;
    //cv::GaussianBlur(maskoval, maskoval, cv::Size(dsize, dsize), 0, 0);
  }
  //cv::imshow("maskoval ", maskoval);
  cv::multiply(maskBlur, maskoval, maskBlur);
  //cv::addWeighted(maskBlur, 0.3, maskoval, 0.7, 0.0, maskBlur);
  int dsize = std::max(int(mask.cols * 0.2), 1) * 2 + 1;
  cv::blur(maskBlur, maskBlur, cv::Size(dsize, dsize));
  maskBlur *= alpha;

  //cv::imshow("maskBlur ", maskBlur);
  //alpha_blend_8u_color_self4(effect, src(r), alpha, mask);
  //alpha_blend_8u_color_self7a(effect, src(r), alpha, maskBlur);
  alpha_blend_8u_color_self7(effect, src(r), maskBlur);
  
  t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
#ifdef IMG_DEBUG_LOG
  std::cout << "LightenEffect effect : " << t * 1000 << " milliseconds" << std::endl;
#endif
  return src;
}

LightenEffect::~LightenEffect() { std::cout << "destroy LightenEffect" << std::endl; }
