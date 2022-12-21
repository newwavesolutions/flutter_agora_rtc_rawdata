#ifndef DEVIL_EFFECT_H
#define DEVIL_EFFECT_H

#include "stickereffect.h"

class DevilEffect: public StickerEffect {
 public:
  cv::Mat apply(cv::Mat &to, const void *data);
  ~DevilEffect();
};

#endif
