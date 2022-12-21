#ifndef ROSE_FILTER_H
#define ROSE_FILTER_H

#include "./filtereffect.h"

class RoseFilter: public FilterEffect {
 public:
  cv::Mat apply(cv::Mat &to, const void *data = NULL);
  virtual ~RoseFilter();
};

#endif
