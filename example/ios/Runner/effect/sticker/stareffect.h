#ifndef STAR_EFFECT_H
#define STAR_EFFECT_H

#include "stickereffect.h"

class StarEffect: public StickerEffect {
 public:
  cv::Mat apply(cv::Mat &to, const void *data);
  ~StarEffect();
};

#endif
