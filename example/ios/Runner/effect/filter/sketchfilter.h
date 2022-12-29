#ifndef SKETCH_FILTER_H
#define SKETCH_FILTER_H

#include "./filtereffect.h"

class SketchFilter: public FilterEffect {
private:
  cv::Mat img_bg;
public:
  void init(const effect_config &config);
  cv::Mat apply(cv::Mat &to, const void *data = NULL);
  virtual ~SketchFilter();
};

#endif
