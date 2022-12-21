#ifndef CAT_EFFECT_H
#define CAT_EFFECT_H

#include "stickereffect.h"

class CatEffect: public StickerEffect {
 public:
  cv::Mat apply(cv::Mat &to, const void *data);
  ~CatEffect();
};

#endif
