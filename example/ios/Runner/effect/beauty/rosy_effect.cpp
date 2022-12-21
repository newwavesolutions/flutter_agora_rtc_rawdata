#include "rosy_effect.h"
#include "scrub_effect.h"
#include "../common/blend.h"

void RosyEffect::init(const effect_config &config) {
}

cv::Mat RosyEffect::apply(cv::Mat &to, const void *data) {
  double t = (double)cv::getTickCount();
  beauty_data const *bData = static_cast<beauty_data const*>(data);
  auto & pvf = *(bData->pvf);
  cv::Mat src = to;
  const int& intensity = bData->intensity;
  
  cv::Mat blurMat;
  float softenRatio = intensity / 100.0f;
  float alphaMax = 0.8, alphaMin = 0;
  float alpha = (alphaMax - alphaMin) * softenRatio + alphaMin;
  float betaMax = 0, betaMin = 0;
  float beta = (betaMax - betaMin) * softenRatio + betaMin;
  PointVecf pvcircle = pointAllCircleInner(pvf);
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

  cv::Vec3b pink(203, 192, 255),
    lightpink(193, 182, 255),
    hotpink(180, 105, 255),
    deeppink(147, 20, 255),
    palevioletred(147, 112, 219),
    mediumvioletred(133, 21, 199),
#ifdef __ANDROID__
    red(255,0,0),
#else
    red(0,0,255),
#endif
    rosy1(99, 99, 255),
    rosy2(236, 236, 255),
    rosy3(91, 56, 255),
    rosy4(195, 195, 253),
    rosy5(150, 150, 255);

  cv::Vec3b rosyColor = red;
  //cv::Vec3b rosyColorHs;
  cv::Mat rosyColorMat;
  //std::cout << rosyColor << std::endl;
  cv::cvtColor(cv::Mat_<cv::Vec3b>(rosyColor), rosyColorMat, cv::COLOR_BGR2HSV);
  //std::cout << (int)rosyColorMat.at<uchar>(0, 0) << std::endl;
  weight_channel_u(hs, 0, rosyColorMat.at<uchar>(0, 0), 0.0);
  weight_channel_u(hs, 1, rosyColorMat.at<uchar>(0, 1), 0.8);
  //scale_channel_u(hs, 2, 1.8);
  
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
  int dsize = std::max(int(mask.cols * 0.1), 1) * 2 + 1;
  cv::blur(maskBlur, maskBlur, cv::Size(dsize, dsize));
  maskBlur *= alpha;

  //cv::imshow("maskBlur ", maskBlur);
  //alpha_blend_8u_color_self4(effect, src(r), alpha, mask);
  //alpha_blend_8u_color_self7a(effect, src(r), alpha, maskBlur);
  alpha_blend_8u_color_self7(effect, src(r), maskBlur);


//   {
//     cv::Mat blurMat;
//     float softenRatio = intensity / 100.0f;
//     float alphaMax = 0.8, alphaMin = 0;
//     float alpha = (alphaMax - alphaMin) * softenRatio + alphaMin;
//     float betaMax = 0, betaMin = 0;
//     float beta = (betaMax - betaMin) * softenRatio + betaMin;
//     cv::Rect r = cv::boundingRect(pvf);
  
//     if ((*(bData->pvf)).size() == 68) {
//       r.y -= r.height / 2;
//       if (r.y < 0) r.y = 0;
//       r.height += r.height / 2;
//     }
//     else {
//       r.x -= (int)(r.width * 0.5);
//       r.y -= (int)(r.height * 1.25);
//       r.width *= 2;
//       r.height = (int)(r.height * 3.25);    
//     }
//     r &= cv::Rect(0, 0, src.cols, src.rows);

//       cv::Vec3b lower(0, 48, 80);
//       cv::Vec3b upper(20, 255, 255);
// #if 0
//       cv::Mat hs;
//       cv::cvtColor(src(r), hs, cv::COLOR_BGR2HSV);
//       cv::Mat mask;
//       cv::inRange(hs, lower, upper, mask);
//       //cv::imshow("abc", mask);
//       alpha_blend_8u_color_self3(cv::Vec3b(0, 0, 255), src(r), alpha, beta, mask);
// #endif
//     cv::Point pleft, pright, plip1, plip2;
//     if ((*(bData->pvf)).size() == 68) {
//       pleft = pvf[36]; pright = pvf[45];
//       plip1= pvf[31]; plip2=pvf[35];
//     }
//     else {
//       pleft = pvf[2]; pright = pvf[0];
//       plip1 = pvf[4]; plip2 = pvf[4];
//     }
//     cv::Vec3b pink(203, 192, 255),
//       lightpink(193, 182, 255),
//       hotpink(180, 105, 255),
//       deeppink(147, 20, 255),
//       palevioletred(147, 112, 219),
//       mediumvioletred(133, 21, 199);
      
//     cv::Vec3b color = hotpink;
//     // left
//     {
//       cv::Point center = ((pleft.x * 0.7 + plip1.x * 0.3), (pleft.y + plip1.y) * 0.5);//(pleft + plip1) * 0.5;
//       int w = cv::norm((pleft- plip1)) * 1.5;
//       cv::Rect rect1(center.x - w / 2, center.y - w / 2, w, w);
//       rect1 &= r;
//       cv::Mat hs1;
//       cv::cvtColor(src(rect1), hs1, cv::COLOR_BGR2HSV);
//       cv::Mat mask1;
//       cv::inRange(hs1, lower, upper, mask1);
//       cv::Mat alphaMat = cv::Mat::zeros(rect1.height, rect1.width, CV_32FC1);
//       cv::circle(alphaMat, cv::Point(alphaMat.cols / 2, alphaMat.rows / 2), rect1.width * 0.25, alpha, -1);
//       //alphaMat(cv::Rect(rect1.width * 0.25, rect1.height * 0.25, rect1.width * 0.5, rect1.height * 0.5)).setTo(alpha);
//       int gd = int(alphaMat.cols * 0.25) * 2 + 1;
//       cv::blur(alphaMat, alphaMat, cv::Size(gd, gd));
//       alpha_blend_8u_color_self6(color, src(rect1), alphaMat, beta, mask1);
//     }
//     {
//       cv::Point center = ((pright.x * 0.7 + plip2.x * 0.3), (pright.y + plip2.y) * 0.5);//(pright + plip2) * 0.5;
//       int w = cv::norm(pright- plip2) * 1.5;
//       cv::Rect rect1(center.x - w / 2, center.y - w / 2, w, w);
//       rect1 &= r;
//       cv::Mat hs1;
//       cv::cvtColor(src(rect1), hs1, cv::COLOR_BGR2HSV);
//       cv::Mat mask1;
//       cv::inRange(hs1, lower, upper, mask1);
//       cv::Mat alphaMat = cv::Mat::zeros(rect1.height, rect1.width, CV_32FC1);
//       cv::circle(alphaMat, cv::Point(alphaMat.cols / 2, alphaMat.rows / 2), rect1.width * 0.25, alpha, -1);
//       //alphaMat(cv::Rect(rect1.width * 0.25, rect1.height * 0.25, rect1.width * 0.5, rect1.height * 0.5)).setTo(alpha);
//       int gd = int(alphaMat.cols * 0.25) * 2 + 1;
//       cv::blur(alphaMat, alphaMat, cv::Size(gd, gd));
//       alpha_blend_8u_color_self6(color, src(rect1), alphaMat, beta, mask1);
//     }
//   }
  
  t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
#ifdef IMG_DEBUG_LOG
  std::cout << "RosyEffect effect : " << t * 1000 << " milliseconds" << std::endl;
#endif
  return src;
}

RosyEffect::~RosyEffect() { std::cout << "destroy RosyEffect" << std::endl; }
