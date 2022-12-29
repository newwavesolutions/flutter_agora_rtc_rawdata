#ifndef CARTOON_FILTER_H
#define CARTOON_FILTER_H

#include "./filtereffect.h"

class CartoonFilter: public FilterEffect {
public:
  void init(const effect_config &config);
  cv::Mat apply(cv::Mat &to, const void *data = NULL);
  virtual ~CartoonFilter();
};

#endif
