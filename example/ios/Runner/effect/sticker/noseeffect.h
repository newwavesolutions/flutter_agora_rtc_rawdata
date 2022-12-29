#ifndef NOSE_EFFECT_H
#define NOSE_EFFECT_H

#include "stickereffect.h"

class NoseEffect: public StickerEffect {
 public:
  cv::Mat apply(cv::Mat &to, const void *data);
  ~NoseEffect();
};

#endif
