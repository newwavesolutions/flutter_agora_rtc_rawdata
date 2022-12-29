#ifndef STICKER_EFFECT_H
#define STICKER_EFFECT_H

#include "../common/common.h"
#include "../common/effect.h"

class StickerEffect: public Effect {
 protected:
  cv::Mat _image;
  PointVecf _pvf;
  std::list<cv::Mat> _image_list;
  std::list<cv::Mat>::iterator _image_list_iter;  
  cv::Mat get_template();
 public:
  void init(const effect_config &config);
  virtual ~StickerEffect();
};

#endif
