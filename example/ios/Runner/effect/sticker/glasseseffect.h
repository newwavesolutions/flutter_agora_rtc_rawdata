#ifndef GLASSES_EFFECT_H
#define GLASSES_EFFECT_H

#include "stickereffect.h"

class GlassesEffect: public StickerEffect {
 private:
  
 public:
  cv::Mat apply(cv::Mat &to, const void *data);
  ~GlassesEffect();
};

#endif
