#include "glasseseffect.h"
#include "../common/blend.h"

//float LineAngle(const cv::Point& a, const cv::Point& b)
//{
//  cv::Point c = a + b;
//  c *= 0.5;
//  return atan2(float(a.y - c.y), float(a.x - c.x));
//}
//float GeometryHelper_Radian2Degree(float rad)
//{
//  return rad * 180.0f / M_PI;
//}
//
//cv::Mat GlassesEffect::alpha_blend_8u(cv::Mat template8u4, cv::Mat base8u) {
//  cv::Mat effect(base8u.size(), CV_8UC3);
//  for (int y = 0; y < base8u.rows; ++y) {
//    cv::Vec3b* peffect = effect.ptr<cv::Vec3b>(y);
//    cv::Vec3b* pbase = base8u.ptr<cv::Vec3b>(y);
//    cv::Vec4b* ptemplate = template8u4.ptr<cv::Vec4b>(y);    
//    for (int x = 0; x < base8u.cols; ++x) {
//      float alpha = ptemplate[x][3] / 255.0;
//      float invalpha = 1 - alpha;
//      for (int i = 0; i < 3; ++i)
//        peffect[x][i] = ptemplate[x][i] * alpha + pbase[x][i] * invalpha;
//    }
//  }
//  return effect;
//}
//
//void GlassesEffect::alpha_blend_8u_self(cv::Mat template8u4, cv::Mat base8u) {
//	//cv::Mat effect(base8u.size(), CV_8UC3);
//	for (int y = 0; y < base8u.rows; ++y) {
//		//cv::Vec3b* peffect = effect.ptr<cv::Vec3b>(y);
//		cv::Vec3b* pbase = base8u.ptr<cv::Vec3b>(y);
//		cv::Vec4b* ptemplate = template8u4.ptr<cv::Vec4b>(y);
//		for (int x = 0; x < base8u.cols; ++x) {
//			float alpha = ptemplate[x][3] / 255.0;
//			if (alpha > 0.1) {
//				float invalpha = 1 - alpha;
//				for (int i = 0; i < 3; ++i)
//					pbase[x][i] = ptemplate[x][i] * alpha + pbase[x][i] * invalpha;
//			}
//		}
//	}
//	//return effect;
//}


//cv::Mat GlassesEffect::apply(cv::Mat &to, const void *data) {
//  double t = (double)cv::getTickCount();
//  cv::Mat effect;
//  PointVecf pv1 = this->_pvf;
//  std::vector<cv::Point2f> const *pvf = static_cast<std::vector<cv::Point2f> const*>(data);
//  cv::Point2f left;
//  cv::Point2f right;
//  cv::Point2f nose;
//  if ((*pvf).size() == 68) {
//    left = ((*pvf)[36] + (*pvf)[37] + (*pvf)[38] + (*pvf)[39] + (*pvf)[40] + (*pvf)[41]) * (1 / 6.0f);
//    right = ((*pvf)[42] + (*pvf)[43] + (*pvf)[44] + (*pvf)[45] + (*pvf)[46] + (*pvf)[47]) * (1 / 6.0f);
//    nose = (*pvf)[30];
//  } else {
//    left = ((*pvf)[2] + (*pvf)[3]) * (1 / 2.0f);
//    right = ((*pvf)[0] + (*pvf)[1]) * (1 / 2.0f);
//    nose = (*pvf)[4];
//  }
//  PointVecf pv2;// = { left, right, pvf[30] };
//  pv2.push_back(left);
//  pv2.push_back(right);
//  pv2.push_back(nose);
////  std::cout << pv1 << std::endl;
////  std::cout << pv2 << std::endl;
//
//  // transform pv2 center to pv1 center
//  cv::Scalar pv1_center = cv::mean(pv1);
//  cv::Scalar pv2_center = cv::mean(pv2);
//  for (auto& p : pv2) {
//    p.x += pv1_center[0] - pv2_center[0];
//    p.y += pv1_center[1] - pv2_center[1];
//  }
////  std::cout << pv2 << std::endl;
//  float scale_x, scale_y;
//  scale_x = (pv1[0].x - pv1[1].x) / (pv2[0].x - pv2[1].x);
//  scale_y = scale_x;
//  for (auto&p : pv2) {
//    p.x += (p.x - pv1_center[0]) * scale_x;
//    p.y += (p.y - pv1_center[1]) * scale_y;
//  }
////  std::cout << pv2 << std::endl;
//
//  cv::Mat template_img = this->_image;
//  int size = 2 * cv::max(template_img.rows, template_img.cols);
//  for (auto&p : pv2) {
//    p.x += (size / 2 - template_img.cols / 2);
//    p.y += (size / 2 - template_img.rows / 2);
//  }
//  for (auto&p : pv1) {
//    p.x += (size / 2 - template_img.cols / 2);
//    p.y += (size / 2 - template_img.rows / 2);
//  }
//
//  /// Compute a rotation matrix with respect to the center of the image
//  float eyelashAngle = LineAngle(pv1[0], pv1[1]);
//  float realeyeAngle = LineAngle(pv2[0], pv2[1]);
//  float rotationAngle = GeometryHelper_Radian2Degree(eyelashAngle - realeyeAngle);
//  cv::Point center = (pv1[0] + pv1[1]) * 0.5;// cv::Point(pv1[0].cols / 2, warp_dst.rows / 2);
//  double angle = rotationAngle;
//  double scale = 1.0;
//  /// Get the rotation matrix with the specifications above
//  cv::Mat warpMat = cv::getRotationMatrix2D(center, angle, scale);
//  cv::Mat inputMat(3, pv1.size(), CV_64FC1);
//  for (unsigned int i = 0; i < pv1.size(); ++i) {
//    inputMat.at<double>(0, i) = pv1[i].x;// -param.xc;
//    inputMat.at<double>(1, i) = pv1[i].y;// -param.yc;
//    inputMat.at<double>(2, i) = 1;
//  }
////  std::cout << pv1 << std::endl;
////  std::cout << warpMat << std::endl;
//  cv::Mat outputMat = warpMat * inputMat;
//  for (unsigned int i = 0; i < pv1.size(); ++i) {
//    pv2[i].x = outputMat.at<double>(0, i);
//    pv2[i].y = outputMat.at<double>(1, i);
//  }
////  std::cout << pv2 << std::endl;
//  // Apply the Affine Transform just found to the src image
//  cv::Mat template_img2 = cv::Mat::zeros(size, size, template_img.type());
//  template_img.copyTo(template_img2(
//    cv::Rect((size - template_img.cols) / 2, (size - template_img.rows) / 2, template_img.cols, template_img.rows)));
//  
//  cv::Mat img2Cropped = cv::Mat::zeros(size, size, template_img.type());
//
//  warpAffine(template_img2, img2Cropped, warpMat, img2Cropped.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar(0));
//
//  //scale img
//  cv::Mat img2scale;
////  std::cout << cv::Size(size * 1 / scale_x, size * 1 / scale_y) << std::endl;
//  cv::resize(img2Cropped, img2scale, cv::Size(size * 1 / scale_x, size * 1 / scale_y));
//  for (auto& p : pv2) {
//    p.x *= 1 / scale_x;
//    p.y *= 1 / scale_y;
//  }
//
//  cv::Scalar pv2_center_patch = cv::mean(pv2);
//  cv::Rect r1(pv2_center[0] - pv2_center_patch[0], pv2_center[1] - pv2_center_patch[1], img2scale.cols, img2scale.rows);
////  std::cout <<"r1"<<r1<<std::endl;
//  cv::Rect r = r1 & cv::Rect(0, 0, to.cols, to.rows);
////  std::cout << "r" << r << std::endl;
//  cv::Mat src_roi = to(r);
//  cv::Rect rt = cv::Rect(-r1.x + r.x, -r1.y + r.y, r.width, r.height);
////  std::cout << "rt" << rt <<std::endl;
//  img2scale = img2scale(rt);
//  //cv::Mat effect8c3 = alpha_blend_8u(img2scale, src_roi);
//  //effect8c3.copyTo(to(r));
//  alpha_blend_8u_self(img2scale, src_roi);
//  
//  t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
//  std::cout << "duration applying effect : " << t*1000 << " milliseconds" << std::endl;
//  return to;
//}

/*cv::Mat GlassesEffect::apply(cv::Mat &to, const void *data) {
  double t = (double)cv::getTickCount();
  cv::Mat effect;
  PointVecf pv1 = this->_pvf;
  std::vector<cv::Point2f> const *pvf = static_cast<std::vector<cv::Point2f> const*>(data);
  cv::Point2f left;
  cv::Point2f right;
  cv::Point2f nose;
  if ((*pvf).size() == 68) {
    left = ((*pvf)[36] + (*pvf)[37] + (*pvf)[38] + (*pvf)[39] + (*pvf)[40] + (*pvf)[41]) * (1 / 6.0f);
    right = ((*pvf)[42] + (*pvf)[43] + (*pvf)[44] + (*pvf)[45] + (*pvf)[46] + (*pvf)[47]) * (1 / 6.0f);
    nose = (*pvf)[30];
  }
  else {
    left = ((*pvf)[2] + (*pvf)[3]) * (1 / 2.0f);
    right = ((*pvf)[0] + (*pvf)[1]) * (1 / 2.0f);
    nose = (*pvf)[4];
  }
  PointVecf pv2;// = { left, right, pvf[30] };
  pv2.push_back(left);
  pv2.push_back(right);
  pv2.push_back(nose);

  cv::Rect roi_src = cv::boundingRect((*pvf));
  if ((*pvf).size() != 68) {
    roi_src.x -= roi_src.width * 0.5f;
    roi_src.y -= roi_src.height * 0.5f;
    roi_src.width *= 2;
    roi_src.height *= 1.5f;
  }
  roi_src &= cv::Rect(0, 0, src.cols, src.rows);
  cv::Mat roi = src(roi_src);

  //cv::Mat effect;
  //PointVecf pv1 = glass_data.pvf;

  //cv::Point2f left = (pvf[36] + pvf[37] + pvf[38] + pvf[39] + pvf[40] + pvf[41]) * (1 / 6.0f);
  //cv::Point2f right = (pvf[42] + pvf[43] + pvf[44] + pvf[45] + pvf[46] + pvf[47]) * (1 / 6.0f);
  //PointVecf pv2 = { left, right, cv::Point2f(pvf[30]) };
  //for (auto& p : pv2) {
  //  p.x -= roi_src.x;
  //  p.y -= roi_src.y;
  //}

  float eyelashAngle = LineAngle(pv1[0], pv1[1]);
  float realeyeAngle = LineAngle(pv2[0], pv2[1]);
  float rotationAngle = GeometryHelper_Radian2Degree(eyelashAngle - realeyeAngle);
  cv::Point2f center = (pv2[0] + pv2[1]) * 0.5;// cv::Point(pv1[0].cols / 2, warp_dst.rows / 2);
  double angle = rotationAngle;
  double scale = 1.0;
  /// Get the rotation matrix with the specifications above
  cv::Mat warpMat = cv::getRotationMatrix2D(center, angle, scale);
  cv::Mat_<double> inputMat(3, 1);

  float scale_x = (pv1[0].x - pv1[1].x) / (pv2[0].x - pv2[1].x);
  float scale_y = scale_x;
  
  cv::Point2f pv1_center = (pv1[0] + pv1[1]) * 0.5f; //cv::mean(pv1);
  cv::Point2f pv2_center = (pv2[0] + pv2[1]) * 0.5f; //cv::mean(pv2);

  for (int y = 0; y < roi.rows; ++y) {
    cv::Vec3b* p = roi.ptr<cv::Vec3b>(y);
    for (int x = 0; x < roi.cols; ++x) {
      float xtf = x, ytf = y;
      inputMat << x, y, 1;
      cv::Mat outputMat = warpMat * inputMat;
      xtf = outputMat.at<double>(0, 0);
      ytf = outputMat.at<double>(1, 0);
      xtf -= pv2_center.x;
      ytf -= pv2_center.y;
      xtf *= scale_x;
      ytf *= scale_y;
      xtf += pv1_center.x;
      ytf += pv1_center.y;
      // alpha blending
      int yt = (int)(ytf + 0.5);
      int xt = (int)(xtf + 0.5);
      if (yt < this->_image.rows && yt >= 0 && xt < this->_image.cols && xt >= 0)
      {
        cv::Vec4b g = this->_image.at<cv::Vec4b>(yt, xt);
        float alpha = g[3] / 255.0f;
        if (alpha > 0.01) {
          for (int i = 0; i < 3; ++i) {
            p[x][i] = p[x][i] * (1 - alpha) + g[i] * alpha;
          }
        }
      }
    }
  }

  return src;
 }
*/

cv::Mat GlassesEffect::apply(cv::Mat &to, const void *data) {
  double t = (double)cv::getTickCount();
  cv::Mat effect;
  PointVecf pv1 = this->_pvf;
  std::vector<cv::Point2f> const *pvf = static_cast<std::vector<cv::Point2f> const*>(data);
  cv::Point2f left;
  cv::Point2f right;
  cv::Point2f nose;
  if ((*pvf).size() == 68) {
    left = ((*pvf)[36] + (*pvf)[37] + (*pvf)[38] + (*pvf)[39] + (*pvf)[40] + (*pvf)[41]) * (1 / 6.0f);
    right = ((*pvf)[42] + (*pvf)[43] + (*pvf)[44] + (*pvf)[45] + (*pvf)[46] + (*pvf)[47]) * (1 / 6.0f);
    nose = (*pvf)[30];
  } else {
    left = ((*pvf)[2] + (*pvf)[3]) * (1 / 2.0f);
    right = ((*pvf)[0] + (*pvf)[1]) * (1 / 2.0f);
    nose = (*pvf)[4];
  }
  PointVecf pv2;// = { left, right, pvf[30] };
  pv2.push_back(left);
  pv2.push_back(right);
  pv2.push_back(nose);

  cv::Scalar pv1_center = cv::mean(pv1);
  cv::Scalar pv2_center = cv::mean(pv2);
  // get scale from tempate to current video size
  double scale = (pv1[0].x - pv1[1].x) / (pv2[0].x - pv2[1].x);

  // scale tempate
  cv::Mat resized = cv::Mat::zeros(this->_image.rows/scale, this->_image.cols/scale, this->_image.type());
  cv::resize(this->_image, resized, resized.size(), 0, 0, cv::INTER_LINEAR);
  pv1[0] *= 1 / scale;
  pv1[1] *= 1 / scale;
  pv1[2] *= 1 / scale;

  // get angle to rotate 2d
  float eyelashAngle = LineAngle(pv1[0], pv1[1]);
  float realeyeAngle = LineAngle(pv2[0], pv2[1]);
  float rotationAngle = GeometryHelper_Radian2Degree(eyelashAngle - realeyeAngle);
  cv::Point center = (pv1[0] + pv1[1]) * 0.5;
  double angle = rotationAngle;
  cv::Mat warpMat = cv::getRotationMatrix2D(center, angle, 1.0f);

  // rotate
  cv::Mat transformed = cv::Mat::zeros(resized.rows, resized.cols, resized.type());
  cv::warpAffine(resized, transformed, warpMat, transformed.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar(0));

  cv::Mat inputMat(3, pv1.size(), CV_64FC1);
  for (unsigned int i = 0; i < pv1.size(); ++i)
  {
    inputMat.at<double>(0, i) = pv1[i].x;
    inputMat.at<double>(1, i) = pv1[i].y;
    inputMat.at<double>(2, i) = 1;
  }
  cv::Mat outputMat = warpMat * inputMat;
  for (unsigned int i = 0; i < pv1.size(); ++i)
  {
    pv1[i].x = outputMat.at<double>(0, i);
    pv1[i].y = outputMat.at<double>(1, i);
  }

  // merge to video
  //int x = (pv2_center[0] - resized.cols/2)<0?0:pv2_center[0] - resized.cols/2;
  //int y = (pv2_center[1] - resized.rows/2)<0?0:pv2_center[1] - resized.rows/2;
  cv::Scalar pv1_center_new = cv::mean(pv1);
  int x = pv2_center[0] - pv1_center_new[0];
  int y = pv2_center[1] - pv1_center_new[1];
  cv::Rect rect2(x, y, resized.cols, resized.rows);
  /*cv::Mat src_roi = to(rect2);
  alpha_blend_8u_self(transformed, src_roi);*/
  cv::Rect r = rect2 & cv::Rect(0, 0, to.cols, to.rows);
  cv::Mat transformed_roi = transformed(cv::Rect(r.x - rect2.x, r.y - rect2.y, r.width, r.height));
  cv::Mat src_roi = to(r);
  alpha_blend_8u_self(transformed_roi, src_roi);
 
  t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
#ifdef IMG_DEBUG_LOG
  std::cout << "glasses effect : " << t*1000 << " milliseconds" << std::endl;
#endif
  return to;
}

GlassesEffect::~GlassesEffect() {
  std::cout << "destroy glasses effect" << std::endl;
}
