#include "bigeye_effect.h"
#include "scrub_effect.h"
#include "../common/blend.h"


int checkpoint(int h, int k, int x, int y, int a, int b)
{
  int p = (pow((x - h), 2) / pow(a, 2))+ (pow((y - k), 2) / pow(b, 2));
  return p;
}

void EyeRegion(PointVec& pv, cv::Mat& src, int intensity) {
  cv::Rect r1 = cv::boundingRect(pv);  
  float rscale_x = 3.5f;
  float rscale_y = 4.5f;
  /*r1.x -= r1.width * (rscale_x - 1.0f) * 0.5;
  r1.y -= r1.height * (rscale_y - 1.0f) * 0.5;
  r1.width *= rscale_x;
  r1.height *= rscale_y;*/
  {
	  cv::Point2f center = (r1.tl() + r1.br()) * 0.5;
	  int w = r1.width * rscale_x;
	  int h = r1.height * rscale_y;
	  r1 = cv::Rect((int)(center.x - w * 0.5 + 0.5), (int)(center.y - h * 0.5), w, h);
    if (r1.x < 0 || r1.y < 0 || r1.width + r1.x > src.cols || r1.height + r1.y > src.rows) {
      return;
    }
  }
  //cv::rectangle(src, r1, cv::Scalar(0, 0, 255));
  //cv::Point center = (r1.tl() + r1.br()) * 0.5;
  cv::Point2f center(r1.width * 0.5, r1.height * 0.5);
  cv::Mat roi = src(r1);
  cv::Mat roi2 = roi.clone();
  float kmax = 0.5f, kmin = 0.0f;
  float k = kmin + (kmax - kmin) * intensity / 100.0f;
  
  cv::Mat blur1 = cv::Mat::zeros(roi.rows, roi.cols, CV_32FC1);
  {
    PointVec pvall2(pv.size());
    for (int i = 0; i < pvall2.size(); ++i)
      pvall2[i] = cv::Point(pv[i]) - r1.tl();
    std::vector<PointVec> pvall2vec = { pvall2 };
    cv::fillPoly(blur1, pvall2vec, cv::Scalar(1.0));
    Dilation(blur1, blur1, 0, 3);
    int gd = int(blur1.cols * 0.1) * 2 + 1;
    cv::blur(blur1, blur1, cv::Size(gd, gd));
  }

  for (int y = 1; y < roi.rows-1; y++) {    
    float* pmask = blur1.ptr<float>(y);
    for (int x = 1; x < roi.cols-1; x++) {
      cv::Point2f p(x, y);
      cv::Point p2;
      float d = cv::norm(center - p);
      float k2 = k;
      
	  // nearest
      //p2.x = (int)(p.x + (center.x - p.x) * k2 * pmask[x] + 0.5);
      //p2.y = (int)(p.y + (center.y - p.y) * k2 * pmask[x] + 0.5);
	  //roi.at<cv::Vec3b>(y, x) = roi2.at<cv::Vec3b>(p2);

	  float nx = p.x + (center.x - p.x) * k2 * pmask[x];
	  float ny = p.y + (center.y - p.y) * k2 * pmask[x];
	  float x1 = (int)nx, x2 = x1 + 1;
	  float y1 = (int)ny, y2 = y1 + 1;
	  cv::Point pn(nx, ny);
	  cv::Point pa11(x1, y1), pa12(x1, y2), pa21(x2, y1), pa22(x2, y2);
	  cv::Vec3f ix1 = (x2 - nx) * roi2.at<cv::Vec3b>(pa11) + (nx - x1) * roi2.at<cv::Vec3b>(pa21);
	  cv::Vec3f ix2 = (x2 - nx) * roi2.at<cv::Vec3b>(pa21) + (nx - x1) * roi2.at<cv::Vec3b>(pa22);
	  cv::Vec3f iy = (y2 - ny) * ix1 + (ny - y1) * ix2;
	  
	  roi.at<cv::Vec3b>(y, x) = iy;
    }
  }
  //cv::imshow("roi", roi);
  //cv::imshow("roi2", roi2);
}




void BigEyeEffect::init(const effect_config &config) {
}

cv::Mat BigEyeEffect::apply(cv::Mat &to, const void *data) {
  double t = (double)cv::getTickCount();
  beauty_data const *bData = static_cast<beauty_data const*>(data);
  auto & pvf = *(bData->pvf);
  cv::Mat src = to;
  const int& intensity = bData->intensity;
  
  if (pvf.size() == 68) {
    {
      PointVec pv(6);
      for (int i = 36; i < 42; ++i)
        pv[i - 36] = pvf[i];
      EyeRegion(pv, src, intensity);
    }
    {
      PointVec pv(6);
      for (int i = 42; i < 48; ++i)
        pv[i - 42] = pvf[i];
      EyeRegion(pv, src, intensity);
    }
  }
  else {
    {
      int d = cv::norm(pvf[2] - pvf[3]);
      PointVec pv{ pvf[2],pvf[3],
        cv::Point((pvf[2].x + pvf[3].x) * 0.5,(pvf[2].y + pvf[3].y) * 0.5 - d * 0.3),
        cv::Point((pvf[2].x + pvf[3].x) * 0.5,(pvf[2].y + pvf[3].y) * 0.5 + d * 0.3) };
      EyeRegion(pv, src, intensity);
    }
    {
      int d = cv::norm(pvf[0] - pvf[1]);
      PointVec pv{ pvf[0],pvf[1],
        cv::Point((pvf[0].x + pvf[1].x) * 0.5,(pvf[0].y + pvf[1].y) * 0.5 - d * 0.3),
        cv::Point((pvf[0].x + pvf[1].x) * 0.5,(pvf[0].y + pvf[1].y) * 0.5 + d * 0.3) };
      EyeRegion(pv, src, intensity);
    }
  }
  
  
  t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
#ifdef IMG_DEBUG_LOG
  std::cout << "BigEyeEffect effect : " << t * 1000 << " milliseconds" << std::endl;
#endif
  return src;
}

BigEyeEffect::~BigEyeEffect() { std::cout << "destroy BigEyeEffect" << std::endl; }
