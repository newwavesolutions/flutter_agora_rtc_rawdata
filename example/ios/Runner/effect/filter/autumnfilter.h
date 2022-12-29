#ifndef AUTUMN_FILTER_H
#define AUTUMN_FILTER_H

#include "./filtereffect.h"

class AutumnFilter: public FilterEffect {
public:
  cv::Mat apply(cv::Mat &to, const void *data = NULL);
  virtual ~AutumnFilter();
};

#endif
