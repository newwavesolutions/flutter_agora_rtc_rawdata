#ifndef CAT2_EFFECT_H
#define CAT2_EFFECT_H

#include "stickereffect.h"

class Cat2Effect: public StickerEffect {
 public:
  cv::Mat apply(cv::Mat &to, const void *data);
  ~Cat2Effect();
};

#endif
