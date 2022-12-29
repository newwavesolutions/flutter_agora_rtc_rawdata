#include "facemark.h"

FaceMark::FaceMark() {
}

FaceMark::FaceMark(const std::string &path68, const std::string &path5) {
  this->init(path68, path5);
}

void FaceMark::init(const std::string &path68, const std::string &path5) {
  //this->_facemark = cv::face::FacemarkLBF::create();
  //this->_facemark->loadModel(path);
  if (path68.length() > 0) {
    dlib::deserialize(path68) >> sp68;
  }
  if (path5.length() > 0) {
    dlib::deserialize(path5) >> sp5;
  }
}

bool FaceMark::getMarks(const cv::Mat &image, const std::vector<cv::Rect> &faces, std::vector<std::vector<cv::Point2f> > &landmarks, FaceMarkSize size) {
  //this->_facemark->fit(image, faces, landmarks);
  double t = (double)cv::getTickCount();
  dlib::cv_image<unsigned char> cimg(image);
  for(auto face: faces) {
    dlib::rectangle rect(face.tl().x, face.tl().y, face.br().x, face.br().y);
    dlib::full_object_detection shape;
    switch (size) {
    case LANDMARK5:
      shape = sp5(cimg, rect);
      break;
    case LANDMARK68:
      shape = sp68(cimg, rect);
      break;
    default:
      shape = sp5(cimg, rect);
      break;
    }
    std::vector<cv::Point2f> pvec(size);
    for (unsigned int i = 0; i < size; ++i) {
      pvec[i] = cv::Point2f(shape.part(i).x(), shape.part(i).y());
    }
    landmarks.push_back(pvec);
  }
  t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
#ifdef IMG_DEBUG_LOG
  std::cout << "duration getMarks : " << t*1000 << " milliseconds" << std::endl;
#endif
  return true;
}
