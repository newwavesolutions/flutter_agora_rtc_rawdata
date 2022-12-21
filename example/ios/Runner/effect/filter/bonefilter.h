#ifndef BONE_FILTER_H
#define BONE_FILTER_H

#include "./filtereffect.h"

class BoneFilter: public FilterEffect {
public:
  cv::Mat apply(cv::Mat &to, const void *data = NULL);
  virtual ~BoneFilter();
};

#endif
