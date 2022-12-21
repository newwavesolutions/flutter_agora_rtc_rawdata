#ifndef DOG_EFFECT_H
#define DOG_EFFECT_H

#include "stickereffect.h"

class DogEffect: public StickerEffect {
 public:
  cv::Mat apply(cv::Mat &to, const void *data);
  ~DogEffect();
};

#endif
