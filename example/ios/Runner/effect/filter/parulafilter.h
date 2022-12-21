#ifndef PARULA_FILTER_H
#define PARULA_FILTER_H

#include "./filtereffect.h"

class ParulaFilter: public FilterEffect {
public:
  cv::Mat apply(cv::Mat &to, const void *data = NULL);
  virtual ~ParulaFilter();
};

#endif
