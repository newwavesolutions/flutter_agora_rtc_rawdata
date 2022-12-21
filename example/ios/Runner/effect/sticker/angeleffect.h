#ifndef ANGEL_EFFECT_H
#define ANGEL_EFFECT_H

#include "stickereffect.h"

class AngelEffect: public StickerEffect {
 public:
  cv::Mat apply(cv::Mat &to, const void *data);
  ~AngelEffect();
};

#endif
