#ifndef BEAR_EFFECT_H
#define BEAR_EFFECT_H

#include "stickereffect.h"

class BearEffect: public StickerEffect {
 public:
  cv::Mat apply(cv::Mat &to, const void *data);
  ~BearEffect();
};

#endif
