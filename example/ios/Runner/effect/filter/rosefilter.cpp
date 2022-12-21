#include "./rosefilter.h"

cv::Mat RoseFilter::apply(cv::Mat &to, const void *data) {
  //uint8_t* pixelPtr = (uint8_t*)to.data;
  //int cn = to.channels();
  
  /*for (int i=0; i<to.rows; i++) {
    for (int j=0; j<to.cols; j++) {
      //pixelPtr[i*to.cols*cn + j*cn + 0] += 10; // B
      //pixelPtr[i*to.cols*cn + j*cn + 1] -= 10; // G
      pixelPtr[i*to.cols*cn + j*cn + 2] = 200; // R
    }
    }*/
  double t = (double)cv::getTickCount();
  //cv::Mat new_image = cv::Mat::zeros( to.size(), to.type() );
  double alpha = 0.5; /*< Simple contrast control */
  int beta = 100;       /*< Simple brightness control */
  /*for( int y = 0; y < to.rows; y++ ) {
    for( int x = 0; x < to.cols; x++ ) {
      for( int c = 0; c < to.channels(); c++ ) {
	new_image.at<cv::Vec3b>(y,x)[c] =
	  cv::saturate_cast<uchar>( alpha*to.at<cv::Vec3b>(y,x)[c] + beta );
      }
    }
    }*/
  //uint8_t* np = (uint8_t*)new_image.data;
  uint8_t* op = (uint8_t*)to.data;
  //int cn = new_image.channels();
  int cn = to.channels();
  for (int i=0; i<to.rows; i++) {
    for (int j=0; j<to.cols; j++) {
      op[i*to.cols*cn + j*cn + 0] = saturate_cast_uchar( alpha*op[i*to.cols*cn + j*cn + 0] + beta ); // B
      op[i*to.cols*cn + j*cn + 1] = saturate_cast_uchar( alpha*op[i*to.cols*cn + j*cn + 1] + beta ); // G
      op[i*to.cols*cn + j*cn + 2] = saturate_cast_uchar( alpha*op[i*to.cols*cn + j*cn + 2] + beta + 20 ); // R
    }
  }
  t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
#ifdef IMG_DEBUG_LOG
  std::cout << "duration applying rose filter : " << t*1000 << " milliseconds" << std::endl;
#endif
  return to;
}

RoseFilter::~RoseFilter() {
}
