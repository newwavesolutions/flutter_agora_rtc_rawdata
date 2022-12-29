#ifndef OCEAN_FILTER_H
#define OCEAN_FILTER_H

#include "./filtereffect.h"

class OceanFilter: public FilterEffect {
public:
  cv::Mat apply(cv::Mat &to, const void *data = NULL);
  virtual ~OceanFilter();
};

#endif
