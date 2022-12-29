#include "vface_effect.h"
#include "scrub_effect.h"
#include "../common/blend.h"

void VFaceEffect::init(const effect_config &config) {
}


cv::Mat VFaceEffect::apply(cv::Mat &to, const void *data) {
  double t = (double)cv::getTickCount();
  beauty_data const *bData = static_cast<beauty_data const*>(data);
  auto & pvf = *(bData->pvf);
  cv::Mat src = to; 
  const int& intensity = bData->intensity;
  
  PointVecf pvout = pvoutside(pvf);
  {
    // angle
    // from 0 to 16
    PointVecf pvl(8);
    for (int i = 0; i < 8; i++)
      pvl[i] = pvout[i];
    PointVecf pvr(8);
    for (int i = 16; i >= 9; i--)
      pvr[i - 9] = pvout[i];
    PointVecf pv2(8);
    for (int i = 0; i < 8; i++)
      pv2[i] = pvr[i] - pvl[i];
    std::vector<float> pf3(8);
    for (int i = 0; i < 8; i++) {
      if (std::abs(pv2[i].x) < 0.001)
        pv2[i].x = 0.001;
      pf3[i] = pv2[i].y / pv2[i].x;
    }
    float m = cv::mean(pf3)[0];

    /*PointVecf pcenter(8);
    for (int i = 0; i < 8; i++)
      pcenter[i] = (pvr[i] + pvl[i]) * 0.5;
    PointVecf pcentersub(8);
    for (int i = 0; i < 8; i++)
      pcentersub[i] = pcenter[i] - pvf[8];
    std::vector<float> pf4(8);
    for (int i = 0; i < 8; i++) {
      if (abs(pv2[i].x) < 0.001)
        pcentersub[i].x = 0.001;
      pf4[i] = pcentersub[i].y / pcentersub[i].x;
    }
    float k = cv::mean(pf3)[0];*/
    {
      PointVecf pvall(17);
      for (int i = 0; i < 17; i++)
        pvall[i] = pvout[i];
      cv::Rect r1 = cv::boundingRect(pvall);
      float rscale_x = 1.5f;
      float rscale_y = 1.5f;
      r1.x -= r1.width * (rscale_x - 1.0f) * 0.5;
      r1.y -= r1.height * (rscale_y - 1.0f) * 0.5;
      r1.width *= rscale_x;
      r1.height *= rscale_y;
      r1 &= cv::Rect(0, 0, src.cols, src.rows);
      if (r1.width == 0 || r1.height == 0) {
        return src;
      }

      cv::Mat roi = src(r1);
      cv::Mat roi2 = roi.clone();

      float kmax = 0.9f, kmin = 0.0f;
      float k = kmin + (kmax - kmin) * intensity / 100.0f;

      cv::Mat blur1 = cv::Mat::zeros(roi.rows, roi.cols, CV_32FC1);
      PointVec pvall2(pvall.size());
      for (int i = 0; i < pvall2.size(); ++i)
        pvall2[i] = cv::Point(pvall[i]) - r1.tl();
      //std::cout << "boudning rect:" << cv::boundingRect(pvall2);
      std::vector<PointVec> pvall2vec = { pvall2 };
      cv::fillPoly(blur1, pvall2vec, cv::Scalar(1.0));
      Dilation(blur1, blur1, 0, 3);
      int gd = int(blur1.cols * 0.2) * 2 + 1;
      cv::blur(blur1, blur1, cv::Size(gd, gd));
      /*{
        cv::Mat a = blur1 * 255;
        a.convertTo(a, CV_8UC1);
        cv::imshow("blur1", a);
      }*/


      for (int y = 0; y < roi.rows; y++) {
        cv::Point center(r1.width * 0.5, y);

        for (int x = 0; x < roi.cols; x++) {
          cv::Point p(x, y);
          cv::Point p2;
          float d = cv::norm(center - p);
          float k3 = 1 - d / (center.x + 1);
          float k4 = blur1.at<float>(y, x);
          float k2 = k * (pow(y / float(roi.rows), 2)) * k3 * k4;

          p2.x = p.x + (-center.x + p.x) * k2;
          p2.y = p.y + (-center.y + p.y) * k2;

          roi.at<cv::Vec3b>(p) = roi2.at<cv::Vec3b>(p2);
        }
      }

    }
  }
    
  
  t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
#ifdef IMG_DEBUG_LOG
  std::cout << "VFaceEffect effect : " << t * 1000 << " milliseconds" << std::endl;
#endif
  return src;
}

VFaceEffect::~VFaceEffect() { std::cout << "destroy VFaceEffect" << std::endl; }
