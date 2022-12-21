#include "soften_effect.h"
#include "scrub_effect.h"
#include "../common/blend.h"

void SoftenEffect::init(const effect_config &config) {
}

cv::Mat SoftenEffect::apply(cv::Mat &to, const void *data) {
  double t = (double)cv::getTickCount();
  beauty_data const *bData = static_cast<beauty_data const*>(data);
  const int& intensity = bData->intensity;

  cv::Mat src = to;
  PointVecf& pvf = *(bData->pvf);
  {
    //cv::Mat src = makeover;
    //cv::Mat mask;// = m_maskErode;
    //GenMaskErode(mask);
    // High pass filter
    cv::Mat blurMat;
    float softenRatio = intensity / 100.0f;
    float maxSigma = 0.5;
    float minSigma = 0.0;
    float alpha = minSigma + softenRatio * (maxSigma - minSigma);    
    cv::Rect r = cv::boundingRect(pvf);
    PointVecf pvcircle = pointAllCircle(pvf);
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

    int dmax = std::max((int)(r.width * 0.1), 3), dmin = 1;
    int d = int(dmin + softenRatio * (dmax - dmin)) * 2 + 1;
    r &= cv::Rect(0, 0, src.cols, src.rows);
    if (r.width > 0 && r.height > 0) {
      
      cv::blur(src(r), blurMat, cv::Size(d, d));
      
      cv::Vec3b lower(0, 48, 80);
      cv::Vec3b upper(20, 255, 255);
      cv::Mat hs;
      cv::cvtColor(src(r), hs, cv::COLOR_BGR2HSV);
      cv::Mat mask;
      cv::inRange(hs, lower, upper, mask);
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
      cv::multiply(maskBlur, maskoval, maskBlur);
      int dsize = std::max(int(maskBlur.cols * 0.05), 1) * 2 + 1;
      //cv::GaussianBlur(maskBlur, maskBlur, cv::Size(dsize, dsize), 0, 0);
      cv::blur(maskBlur, maskBlur, cv::Size(dsize, dsize));
      maskBlur *= alpha;

      //alpha_blend_8u_color_self4(blurMat, src(r), alpha, mask);
      alpha_blend_8u_color_self7(blurMat, src(r), maskBlur);
    }    

    
  }
  
  t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
#ifdef IMG_DEBUG_LOG
  std::cout << "SoftenEffect effect : " << t * 1000 << " milliseconds" << std::endl;
#endif
  return src;
}

SoftenEffect::~SoftenEffect() { std::cout << "destroy SoftenEffect" << std::endl; }
