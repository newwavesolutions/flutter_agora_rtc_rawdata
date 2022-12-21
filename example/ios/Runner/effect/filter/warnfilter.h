#ifndef WARN_FILTER_H
#define WARN_FILTER_H

#include "./filtereffect.h"

class WarnFilter: public FilterEffect {
private:
  std::vector<int> incr_ch_lut;
  std::vector<int> decr_ch_lut;
public:
  void init(const effect_config &config);
  cv::Mat apply(cv::Mat &to, const void *data = NULL);
  virtual ~WarnFilter();
};

#endif
