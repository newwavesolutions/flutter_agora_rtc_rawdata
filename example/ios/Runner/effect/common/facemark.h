#ifndef FACE_MARK_H
#define FACE_MARK_H

//#include <opencv2/face.hpp>
#include <opencv2/opencv.hpp>
#include <dlib/image_processing.h>
#include <dlib/image_io.h>
#include <dlib/opencv.h>

enum FaceMarkSize { LANDMARK68 = 68, LANDMARK5 = 5 };

class FaceMark
{
private:
  //cv::Ptr<cv::face::Facemark> _facemark;
  dlib::shape_predictor sp68;
  dlib::shape_predictor sp5;
  
public:
  FaceMark();
  FaceMark(const std::string &path68, const std::string &path5);
  
  void init(const std::string &path68, const std::string &path5);
  bool getMarks(const cv::Mat &image, const std::vector<cv::Rect> &faces, std::vector<std::vector<cv::Point2f> > &landmarks, FaceMarkSize size);
};

#endif
