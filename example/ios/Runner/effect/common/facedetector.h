#ifndef FACE_DETECTOR_H
#define FACE_DETECTOR_H

#include <opencv2/opencv.hpp>
//#include <dlib/image_processing/frontal_face_detector.h>
//#include <dlib/image_io.h>
//#include <dlib/opencv.h>
#include <opencv2/objdetect.hpp>

class CascadeDetectorAdapter: public cv::DetectionBasedTracker::IDetector
{
public:
  CascadeDetectorAdapter(cv::Ptr<cv::CascadeClassifier> detector):
          IDetector(),
          Detector(detector)
  {
    CV_Assert(detector);
  }

  void detect(const cv::Mat &Image, std::vector<cv::Rect> &objects)
  {
    std::cout << "image face size : " << Image.cols << ", " << Image.rows << std::endl;
    Detector->detectMultiScale(Image, objects, scaleFactor, minNeighbours, 0, minObjSize, maxObjSize);
  }

  virtual ~CascadeDetectorAdapter()
  {
  }

private:
  CascadeDetectorAdapter();
  cv::Ptr<cv::CascadeClassifier> Detector;
};

struct DetectorAgregator
{
  cv::Ptr<CascadeDetectorAdapter> mainDetector;
  cv::Ptr<CascadeDetectorAdapter> trackingDetector;

  cv::Ptr<cv::DetectionBasedTracker> tracker;
  DetectorAgregator(cv::Ptr<CascadeDetectorAdapter>& _mainDetector, cv::Ptr<CascadeDetectorAdapter>& _trackingDetector):
          mainDetector(_mainDetector),
          trackingDetector(_trackingDetector)
  {
    CV_Assert(_mainDetector);
    CV_Assert(_trackingDetector);

    cv::DetectionBasedTracker::Parameters DetectorParams;
    tracker = cv::makePtr<cv::DetectionBasedTracker>(mainDetector, trackingDetector, DetectorParams);
  }
};

class FaceDetector
{
private:
  // cv::CascadeClassifier _opencvDetector;
//  dlib::frontal_face_detector _dlibDetector;
  double _scale;
  DetectorAgregator *detector;
  
public:
  // init detector with trained data
  FaceDetector();
  FaceDetector(const std::string &path, double scale = 1);

  // init
  void init(const std::string &path, double scale = 1);
  // detect faces and return all rects
  void detectFaces(const cv::Mat &image, std::vector<cv::Rect> &rects);
//  void detectFacesDlib(const cv::Mat &image, std::vector<cv::Rect> &rects);

  ~FaceDetector();
};

#endif
