#ifndef FREEZE_FILTER_H
#define FREEZE_FILTER_H

#include "./filtereffect.h"

class FreezeFilter: public FilterEffect {
private:
  std::vector<int> r_ch_lut;
  std::vector<int> g_ch_lut;
  std::vector<int> b_ch_lut;
  std::vector<int> v_ch_lut;
public:
  void init(const effect_config &config);
  cv::Mat apply(cv::Mat &to, const void *data = NULL);
  virtual ~FreezeFilter();
};

#endif
