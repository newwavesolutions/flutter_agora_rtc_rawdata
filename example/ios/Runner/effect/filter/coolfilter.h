#ifndef COOL_FILTER_H
#define COOL_FILTER_H

#include "./filtereffect.h"

class CoolFilter: public FilterEffect {
private:
  std::vector<int> incr_ch_lut;
  std::vector<int> decr_ch_lut;
public:
  void init(const effect_config &config);
  cv::Mat apply(cv::Mat &to, const void *data = NULL);
  virtual ~CoolFilter();
};

#endif
