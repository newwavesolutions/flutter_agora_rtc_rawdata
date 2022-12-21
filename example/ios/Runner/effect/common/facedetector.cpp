#include "facedetector.h"

FaceDetector::FaceDetector() {
}

FaceDetector::FaceDetector(const std::string &path, double scale) {
  this->init(path, scale);
}

void FaceDetector::init(const std::string &path, double scale) {
  // this->_opencvDetector.load(path);
//  this->_dlibDetector = dlib::get_frontal_face_detector();
  this->_scale = scale;
  cv::Ptr<CascadeDetectorAdapter> mainDetector = cv::makePtr<CascadeDetectorAdapter>(
            cv::makePtr<cv::CascadeClassifier>(path));
  cv::Ptr<CascadeDetectorAdapter> trackingDetector = cv::makePtr<CascadeDetectorAdapter>(
            cv::makePtr<cv::CascadeClassifier>(path));
  this->detector = new DetectorAgregator(mainDetector, trackingDetector);
  this->detector->tracker->run();
}

void FaceDetector::detectFaces(const cv::Mat &image, std::vector<cv::Rect> &rects) {
  // avoid scale <= 0
  if (this->_scale <= 0) {
    return;
  }
  
  double t = (double)cv::getTickCount();
//  cv::Mat gray;
  cv::Mat smallImg(cvRound(image.rows*this->_scale), cvRound(image.cols*this->_scale), cv::COLOR_BGR2GRAY);
  // to gray image
//  cvtColor(image, gray, cv::COLOR_BGR2GRAY);
  // scale image
  //std::cout << "smallImg size: " << smallImg.size() << std::endl;
  resize(image, smallImg, smallImg.size(), 0, 0, cv::INTER_LINEAR);
  // equalize image's histogram
  equalizeHist(smallImg, smallImg);

  // detect faces
  // this->_opencvDetector.detectMultiScale(tmpImage, rects);
  this->detector->tracker->process(smallImg);
  this->detector->tracker->getObjects(rects);

  // scale the rects
  if (this->_scale != 1) {
    for(std::vector<cv::Rect>::iterator r = rects.begin(); r != rects.end(); r++ ) {
      r->x /= this->_scale;
      r->y /= this->_scale;
      r->width /= this->_scale;
      r->height /= this->_scale;
    }
  }
  t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
#ifdef IMG_DEBUG_LOG
  std::cout << "duration detectFaces : " << t*1000 << " milliseconds" << std::endl;
#endif
}

//void FaceDetector::detectFacesDlib(const cv::Mat &image, std::vector<cv::Rect> &rects) {
//  // avoid scale <= 0
//  if (this->_scale <= 0) {
//    return;
//  }
//
//  double t = (double)cv::getTickCount();
//  cv::Mat smallImg(cvRound(image.rows*this->_scale), cvRound(image.cols*this->_scale), cv::COLOR_BGR2GRAY);
//  // to gray image
//  cv::resize(image, smallImg, smallImg.size(), 0, 0, cv::INTER_LINEAR);
//  dlib::array2d<unsigned char> img;
//  dlib::assign_image(img, dlib::cv_image<dlib::bgr_pixel>(smallImg));
//  std::vector<dlib::rectangle> dets;
//  dets = this->_dlibDetector(img, 0);
//  for (auto dlibrect : dets) {
//    cv::Rect r(dlibrect.left()/this->_scale, dlibrect.top()/this->_scale, dlibrect.width()/this->_scale, dlibrect.height()/this->_scale);
//    rects.push_back(r);
//  }
//  t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
//  std::cout << "duration detectFacesDlib : " << t*1000 << " milliseconds" << std::endl;
//}

FaceDetector::~FaceDetector() {
  this->detector->tracker->stop();
  delete this->detector;
}