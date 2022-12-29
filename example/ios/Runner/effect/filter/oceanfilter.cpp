#include "./oceanfilter.h"

cv::Mat OceanFilter::apply(cv::Mat &to, const void *data) {
  double t = (double)cv::getTickCount();

#ifdef __ANDROID__
  cv::cvtColor(to, to, cv::COLOR_BGR2RGB);
#else
#endif

  cv::applyColorMap(to, to, cv::COLORMAP_OCEAN);

#ifdef __ANDROID__
  cv::cvtColor(to, to, cv::COLOR_RGB2BGR);
#else
#endif

  t = ((double)cv::getTickCount() - t)/cv::getTickFrequency();
#ifdef IMG_DEBUG_LOG
  std::cout << "duration applying ocean filter : " << t*1000 << " milliseconds" << std::endl;
#endif
  return to;
}

OceanFilter::~OceanFilter() {
}
