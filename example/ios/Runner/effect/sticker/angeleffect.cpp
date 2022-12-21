#include "angeleffect.h"
#include "../common/blend.h"

cv::Mat AngelEffect::apply(cv::Mat &to, const void *data) {
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
  // move up
  int distance = left.y - nose.y;
  left.y += distance;
  right.y += distance;
  nose.y += distance;
  PointVecf pv2;// = { left, right, pvf[30] };
  pv2.push_back(left);
  pv2.push_back(right);
  pv2.push_back(nose);
  cv::Mat template_image = get_template();

  cv::Scalar pv1_center = cv::mean(pv1);
  cv::Scalar pv2_center = cv::mean(pv2);
  // get scale from tempate to current video size
  double scale = (pv1[0].x - pv1[1].x) / (pv2[0].x - pv2[1].x);

  // scale tempate
  cv::Mat resized = cv::Mat::zeros(template_image.rows/scale, template_image.cols/scale, template_image.type());
  cv::resize(template_image, resized, resized.size(), 0, 0, cv::INTER_LINEAR);
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
  cv::Point center1 = (pv1[0] + pv1[1]) * 0.5;
  cv::Point center2 = (pv2[0] + pv2[1]) * 0.5;
  int x = center2.x - center1.x;
  int y = center2.y - center1.y;

  cv::Rect rect2(x, y, resized.cols, resized.rows);
  /*cv::Mat src_roi = to(rect2);
  alpha_blend_8u_self(transformed, src_roi);*/
  cv::Rect r = rect2 & cv::Rect(0, 0, to.cols, to.rows);
  cv::Mat transformed_roi = transformed(cv::Rect(r.x - rect2.x, r.y - rect2.y, r.width, r.height));
  cv::Mat src_roi = to(r);
  alpha_blend_8u_self(transformed_roi, src_roi);
 
  t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
#ifdef IMG_DEBUG_LOG
  std::cout << "angel effect : " << t*1000 << " milliseconds" << std::endl;
#endif
  return to;
}

AngelEffect::~AngelEffect() {
  std::cout << "destroy angel effect" << std::endl;
}
