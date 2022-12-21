#include "./bonefilter.h"

cv::Mat BoneFilter::apply(cv::Mat &to, const void *data) {
  double t = (double)cv::getTickCount();

#ifdef __ANDROID__
  cv::cvtColor(to, to, cv::COLOR_BGR2RGB);
#else
#endif

  cv::applyColorMap(to, to, cv::COLORMAP_BONE);

#ifdef __ANDROID__
  cv::cvtColor(to, to, cv::COLOR_RGB2BGR);
#else
#endif

  t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
#ifdef IMG_DEBUG_LOG
  std::cout << "duration applying bone filter : " << t*1000 << " milliseconds" << std::endl;
#endif
  return to;
}

BoneFilter::~BoneFilter() {
}
