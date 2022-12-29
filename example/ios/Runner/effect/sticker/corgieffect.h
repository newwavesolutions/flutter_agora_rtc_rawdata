#ifndef CORGI_EFFECT_H
#define CORGI_EFFECT_H

#include "stickereffect.h"

class CorgiEffect: public StickerEffect {
 public:
  cv::Mat apply(cv::Mat &to, const void *data);
  ~CorgiEffect();
};

#endif
