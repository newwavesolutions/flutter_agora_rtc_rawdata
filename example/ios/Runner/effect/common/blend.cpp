#include "blend.h"
#include "common.h"
float LineAngle(const cv::Point& a, const cv::Point& b)
{
  cv::Point c = a + b;
  c *= 0.5;
  return atan2(float(a.y - c.y), float(a.x - c.x));
}
float GeometryHelper_Radian2Degree(float rad)
{
  return rad * 180.0f / M_PI;
}

cv::Mat alpha_blend_8u(cv::Mat template8u4, cv::Mat base8u) {
  cv::Mat effect(base8u.size(), CV_8UC3);
  for (int y = 0; y < base8u.rows; ++y) {
    cv::Vec3b* peffect = effect.ptr<cv::Vec3b>(y);
    cv::Vec3b* pbase = base8u.ptr<cv::Vec3b>(y);
    cv::Vec4b* ptemplate = template8u4.ptr<cv::Vec4b>(y);
    for (int x = 0; x < base8u.cols; ++x) {
      float alpha = ptemplate[x][3] / 255.0;
      float invalpha = 1 - alpha;
      for (int i = 0; i < 3; ++i)
        peffect[x][i] = ptemplate[x][i] * alpha + pbase[x][i] * invalpha;
    }
  }
  return effect;
}

void alpha_blend_8u_self(cv::Mat template8u4, cv::Mat base8u) {
  //cv::Mat effect(base8u.size(), CV_8UC3);
  for (int y = 0; y < base8u.rows; ++y) {
    //cv::Vec3b* peffect = effect.ptr<cv::Vec3b>(y);
    cv::Vec3b* pbase = base8u.ptr<cv::Vec3b>(y);
    cv::Vec4b* ptemplate = template8u4.ptr<cv::Vec4b>(y);
    for (int x = 0; x < base8u.cols; ++x) {
      float alpha = ptemplate[x][3] / 255.0;
      if (alpha > 0.1) {
        float invalpha = 1 - alpha;
        for (int i = 0; i < 3; ++i)
          pbase[x][i] = ptemplate[x][i] * alpha + pbase[x][i] * invalpha;
      }
    }
  }
  //return effect;
}

void alpha_blend_8u_color_self(cv::Vec3b& color, cv::Mat base8u, float alpha, float beta, PointVec& pts) {  
  float invalpha = 1 - alpha;
  for (int y = 0; y < base8u.rows; ++y) {    
    cv::Vec3b* pbase = base8u.ptr<cv::Vec3b>(y);    
    for (int x = 0; x < base8u.cols; ++x) {      
      if (cv::pointPolygonTest(pts, cv::Point2f(x,y), false) > 0) {        
        for (int i = 0; i < 3; ++i)
          pbase[x][i] = color[i] * alpha + pbase[x][i] * invalpha + beta;
      }
    }
  }  
}

void alpha_blend_8u_color_self2(cv::Vec3b& color, cv::Mat base8u, float alpha, float beta) {
  float invalpha = 1 - alpha;
  for (int y = 0; y < base8u.rows; ++y) {
    cv::Vec3b* pbase = base8u.ptr<cv::Vec3b>(y);
    for (int x = 0; x < base8u.cols; ++x) {
      {        
        for (int i = 0; i < 3; ++i)
          pbase[x][i] = color[i] * alpha + pbase[x][i] * invalpha + beta;
      }
    }
  }
}

void alpha_blend_8u_color_self3(cv::Vec3b& color, cv::Mat base8u, float alpha, float beta, cv::Mat mask) {
  float invalpha = 1 - alpha;
  for (int y = 0; y < base8u.rows; ++y) {
    cv::Vec3b* pbase = base8u.ptr<cv::Vec3b>(y);    
    uchar* pmask = mask.ptr<uchar>(y);
    for (int x = 0; x < base8u.cols; ++x) {   
      if(pmask[x] > 0)
      {
        for (int i = 0; i < 3; ++i)
          pbase[x][i] = color[i] * alpha + pbase[x][i] * invalpha + beta;
      }
    }
  }
}

void alpha_blend_8u_color_self4(cv::Mat effect, cv::Mat base8u, float alpha, cv::Mat mask) {
  for (int y = 0; y < base8u.rows; ++y) {
    cv::Vec3b* pbase = base8u.ptr<cv::Vec3b>(y);
    cv::Vec3b* peffect = effect.ptr<cv::Vec3b>(y);
    uchar* pmask = mask.ptr<uchar>(y);
    for (int x = 0; x < base8u.cols; ++x) {
      if (pmask[x] > 0)
      {
        float invalpha = 1 - alpha;
        for (int i = 0; i < 3; ++i)
          pbase[x][i] = peffect[x][i] * alpha + pbase[x][i] * invalpha;
      }
    }
  }
}

void alpha_blend_8u_color_self5(cv::Mat effect, cv::Mat base8u, float alpha) {
  for (int y = 0; y < base8u.rows; ++y) {
    cv::Vec3b* pbase = base8u.ptr<cv::Vec3b>(y);
    cv::Vec3b* peffect = effect.ptr<cv::Vec3b>(y);
    //uchar* pmask = mask.ptr<uchar>(y);
    for (int x = 0; x < base8u.cols; ++x) {
      //if (pmask[x] > 0)
      {
        float invalpha = 1 - alpha;
        for (int i = 0; i < 3; ++i)
          pbase[x][i] = peffect[x][i] * alpha + pbase[x][i] * invalpha;
      }
    }
  }
}

void alpha_blend_8u_color_self6(cv::Vec3b& color, cv::Mat base8u, cv::Mat alpha, float beta, cv::Mat mask) {
  for (int y = 0; y < base8u.rows; ++y) {
    cv::Vec3b* pbase = base8u.ptr<cv::Vec3b>(y);
    uchar* pmask = mask.ptr<uchar>(y);
    float* palpha = alpha.ptr<float>(y);
    for (int x = 0; x < base8u.cols; ++x) {
      if (pmask[x] > 0)
      {
        float invalpha = 1 - palpha[x];
        for (int i = 0; i < 3; ++i)
          pbase[x][i] = color[i] * palpha[x] + pbase[x][i] * invalpha + beta;
      }
    }
  }
}

void alpha_blend_8u_color_self7a(cv::Mat effect, cv::Mat base8u, float alpha, cv::Mat mask) {
  for (int y = 0; y < base8u.rows; ++y) {
    cv::Vec3b* pbase = base8u.ptr<cv::Vec3b>(y);
    cv::Vec3b* peffect = effect.ptr<cv::Vec3b>(y);
    float* pmask = mask.ptr<float>(y);
    for (int x = 0; x < base8u.cols; ++x) {
      if (pmask[x] > 0)
      {
        float alpha2 = alpha * pmask[x];
        float invalpha = 1 - alpha2;
        for (int i = 0; i < 3; ++i)
          pbase[x][i] = peffect[x][i] * alpha2 + pbase[x][i] * invalpha;
      }
    }
  }
}

void alpha_blend_8u_color_self7(cv::Mat effect, cv::Mat base8u, cv::Mat alphamask) {
  for (int y = 0; y < base8u.rows; ++y) {
    cv::Vec3b* pbase = base8u.ptr<cv::Vec3b>(y);
    cv::Vec3b* peffect = effect.ptr<cv::Vec3b>(y);
    float* pmask = alphamask.ptr<float>(y);
    for (int x = 0; x < base8u.cols; ++x) {
      if (pmask[x] > 0)
      {
        float alpha2 = pmask[x];
        float invalpha = 1 - alpha2;
        for (int i = 0; i < 3; ++i)
          pbase[x][i] = peffect[x][i] * alpha2 + pbase[x][i] * invalpha;
      }
    }
  }
}


PointVecf pointAllCircle(PointVecf& pvf) {
  PointVecf pvcircle(32);
  if (pvf.size() == 68)
  {
    PointVecf& pv17 = pvf;
    cv::Point2f center = (pv17[0] + pv17[16]) * 0.5f;
    for (int i = 0; i < 17; ++i)
      pvcircle[i] = pv17[i];
    for (int i = 17, j = 1; i < 32; ++i, ++j)
    {
      pvcircle[i].x = pv17[i - 2 * j].x;
      pvcircle[i].y = pv17[i - 2 * j].y - 2 * (pv17[i - 2 * j].y - center.y);
    }
  }
  else
  {
    //PointVec pv17(17);
    ////std::cout << "pv17 1 size:" << pvf.size() << std::endl;
    //{
    //  int w = 2 * std::abs(pvf[0].x - pvf[2].x);
    //  float dw = w * 1 / 16.0f;
    //  int h = 2.3 * std::abs(pvf[4].y - (pvf[0].y + pvf[2].y) * 0.5);
    //  float dh = h * 1 / 8.0f;
    //  int startx = (pvf[0].x + pvf[2].x) * 0.5f - w * 0.5f;
    //  //std::cout << "pv17 1 1size:" << pvf.size() << std::endl;
    //  for (int i = 0; i < 17; i++)
    //    pv17[i].x = startx + i * dw;
    //  int starty = (pvf[0].y + pvf[2].y) * 0.5f;
    //  //std::cout << "pv17 1 2size:" << pvf.size() << std::endl;
    //  for (int i = 0; i < 9; i++)
    //    pv17[i].y = starty + i * dh;
    //  //std::cout << "pv17 1 3size:" << pvf.size() << std::endl;
    //  for (int i = 16; i >= 9; i--) {
    //    //std::cout << i << std::endl;
    //    pv17[i].y = starty + (16 - i) * dh;
    //  }
    //  //std::cout << "pv17 1 4size:" << pvf.size() << std::endl;
    //}
    ////std::cout << "pv17 123" << std::endl;
    ////std::cout << "pv17 2" << std::endl;
    //cv::Point center = (pv17[0] + pv17[16]) * 0.5f;
    //for (int i = 0; i < 17; ++i)
    //  pvcircle[i] = pv17[i];
    //std::cout << "pv17 3" << std::endl;
    //for (int i = 17, j = 1; i < 32; ++i, ++j)
    //{
    //  std::cout << (i - 2 * j) << std::endl;
    //  pvcircle[i].x = pv17[i - 2 * j].x;
    //  pvcircle[i].y = pv17[i - 2 * j].y - 2 * (pv17[i - 2 * j].y - center.y);
    //}
    std::vector<cv::Point2d> pellipse;
    cv::ellipse2Poly(
      cv::Point((pvf[0] + pvf[2]) * 0.5),
      cv::Size(std::abs(pvf[0].x - pvf[2].x), 2.3 * std::abs(pvf[4].y - (pvf[0].y + pvf[2].y) * 0.5)),
      0, 0, 360, 10, pellipse);
    pvcircle.clear();
    for (auto p : pellipse) {
      pvcircle.push_back(cv::Point2f(p.x, p.y));
    }
    //pvcircle = pellipse;
  }
  return pvcircle;
}

PointVecf pointAllCircleInner(PointVecf& pvf) {
  PointVecf pvcircle(32);
  if (pvf.size() == 68)
  {
    PointVecf& pv17 = pvf;
    cv::Point2f center = (pv17[0] + pv17[16]) * 0.5f;
    for (int i = 0; i < 17; ++i)
      pvcircle[i] = pv17[i];
    for (int i = 17, j = 1; i < 32; ++i, ++j)
    {
      pvcircle[i].x = pv17[i - 2 * j].x;
      pvcircle[i].y = pv17[i - 2 * j].y - 1.8 * (pv17[i - 2 * j].y - center.y);
    }
  }
  else
  {
    //PointVec pv17(17);
    ////std::cout << "pv17 1 size:" << pvf.size() << std::endl;
    //{
    //  int w = 2 * std::abs(pvf[0].x - pvf[2].x);
    //  float dw = w * 1 / 16.0f;
    //  int h = 2.3 * std::abs(pvf[4].y - (pvf[0].y + pvf[2].y) * 0.5);
    //  float dh = h * 1 / 8.0f;
    //  int startx = (pvf[0].x + pvf[2].x) * 0.5f - w * 0.5f;
    //  //std::cout << "pv17 1 1size:" << pvf.size() << std::endl;
    //  for (int i = 0; i < 17; i++)
    //    pv17[i].x = startx + i * dw;
    //  int starty = (pvf[0].y + pvf[2].y) * 0.5f;
    //  //std::cout << "pv17 1 2size:" << pvf.size() << std::endl;
    //  for (int i = 0; i < 9; i++)
    //    pv17[i].y = starty + i * dh;
    //  //std::cout << "pv17 1 3size:" << pvf.size() << std::endl;
    //  for (int i = 16; i >= 9; i--) {
    //    //std::cout << i << std::endl;
    //    pv17[i].y = starty + (16 - i) * dh;
    //  }
    //  //std::cout << "pv17 1 4size:" << pvf.size() << std::endl;
    //}
    ////std::cout << "pv17 123" << std::endl;
    ////std::cout << "pv17 2" << std::endl;
    //cv::Point center = (pv17[0] + pv17[16]) * 0.5f;
    //for (int i = 0; i < 17; ++i)
    //  pvcircle[i] = pv17[i];
    //std::cout << "pv17 3" << std::endl;
    //for (int i = 17, j = 1; i < 32; ++i, ++j)
    //{
    //  std::cout << (i - 2 * j) << std::endl;
    //  pvcircle[i].x = pv17[i - 2 * j].x;
    //  pvcircle[i].y = pv17[i - 2 * j].y - 2 * (pv17[i - 2 * j].y - center.y);
    //}
    std::vector<cv::Point2d> pellipse;
    cv::Point eyeCenter((pvf[0] + pvf[2]) * 0.5);
    cv::Point center;
    center.x = eyeCenter.x;
    center.y = pvf[4].y * 0.3 + eyeCenter.y * 0.7;
    cv::ellipse2Poly(
      center,
      cv::Size(std::abs(pvf[0].x - pvf[2].x)*0.75, 2.0 * std::abs(pvf[4].y - (pvf[0].y + pvf[2].y) * 0.5)),
      0, 0, 360, 10, pellipse);
    pvcircle.clear();
    for (auto p : pellipse) {
      pvcircle.push_back(cv::Point2f(p.x, p.y));
    }
    //pvcircle = pellipse;
  }
  return pvcircle;
}
void Dilation(cv::Mat src, cv::Mat dilation_dst, int dilation_elem, int dilation_size)
{
  int dilation_type = 0;
  if (dilation_elem == 0) { dilation_type = cv::MORPH_RECT; }
  else if (dilation_elem == 1) { dilation_type = cv::MORPH_CROSS; }
  else if (dilation_elem == 2) { dilation_type = cv::MORPH_ELLIPSE; }
  cv::Mat element = cv::getStructuringElement(dilation_type,
    cv::Size(2 * dilation_size + 1, 2 * dilation_size + 1),
    cv::Point(dilation_size, dilation_size));
  cv::dilate(src, dilation_dst, element);
  //imshow("Dilation Demo", dilation_dst);
}

PointVecf pvoutside(PointVecf& pvf_origin) {
  PointVecf pvf(17);
  if (pvf_origin.size() == 68) {
    for (int i = 0; i < 17; i++)
      pvf[i] = pvf_origin[i];
  }
  else {
    int w = 2 * std::abs(pvf_origin[0].x - pvf_origin[2].x);
    float dw = w * 1 / 16.0f;
    int h = 2.3 * std::abs(pvf_origin[4].y - (pvf_origin[0].y + pvf_origin[2].y) * 0.5);
    float dh = h * 1 / 8.0f;
    int startx = (pvf_origin[0].x + pvf_origin[2].x) * 0.5f - w * 0.5f;
    for (int i = 0; i < 17; i++)
      pvf[i].x = startx + i * dw;
    int starty = (pvf_origin[0].y + pvf_origin[2].y) * 0.5f;
    std::vector<float> scaley = { .0, .98, .96, .94, .9, .85, .8, .75, .7, .0 };
    for (int i = 0; i < 9; i++)
      pvf[i].y = starty + i * dh + dh * scaley[i];
    for (int i = 16; i >= 9; i--)
      pvf[i].y = starty + (16 - i) * dh + dh * scaley[16-i];
  }
  return pvf;
}

PointVec pvf2pv(PointVecf& pvf){
  PointVec pv(pvf.size());
  for(int i =0;i <pvf.size(); ++i)
    pv[i] = pvf[i];
  return pv;
}

void scale_channel_u(cv::Mat& mat, int channel, float scale)
{
  // make sure have enough channels
  if (mat.channels() < channel + 1)
    return;

  const int cols = mat.cols;
  const int step = mat.channels();
  const int rows = mat.rows;
  for (int y = 0; y < rows; y++) {
    // get pointer to the first byte to be changed in this row
    unsigned char* p_row = mat.ptr(y) + channel;
    unsigned char* row_end = p_row + cols * step;
    for (; p_row != row_end; p_row += step)
      (*p_row) = cv::saturate_cast<uchar>((*p_row)*scale);
  }
}

void weight_channel_u(cv::Mat& mat, int channel, float value, float weight)
{
  // make sure have enough channels
  if (mat.channels() < channel + 1)
    return;

  const int cols = mat.cols;
  const int step = mat.channels();
  const int rows = mat.rows;
  float weightinv = 1 - weight;
  for (int y = 0; y < rows; y++) {
    // get pointer to the first byte to be changed in this row
    unsigned char* p_row = mat.ptr(y) + channel;
    unsigned char* row_end = p_row + cols * step;
    for (; p_row != row_end; p_row += step)
      (*p_row) = cv::saturate_cast<uchar>((*p_row) * weight + value * weightinv);
  }
}
