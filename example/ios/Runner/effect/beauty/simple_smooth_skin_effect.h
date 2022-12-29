#ifndef SIMPLE_SMOOTH_SKIN_EFFECT_H
#define SIMPLE_SMOOTH_SKIN_EFFECT_H

#include "../common/common.h"
#include "../common/effect.h"

class SimpleSmoothSkinEffect : public Effect {

public:
  void init(const effect_config &config);
  cv::Mat apply(cv::Mat &to, const void *data = NULL);
  virtual ~SimpleSmoothSkinEffect();
};

#endif

