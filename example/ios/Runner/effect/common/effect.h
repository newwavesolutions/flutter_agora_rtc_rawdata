#ifndef EFFECT_H
#define EFFECT_H

#include "./common.h"
#ifndef __ANDROID__
#define __ANDROID__
#endif

struct effect_config {
  std::string image;
  std::string meta;
  std::string image_folder;
  std::string name_pattern;
};

class Effect {
 public:
  virtual void init(const effect_config &config) = 0;
  virtual cv::Mat apply(cv::Mat &to, const void *data = NULL) = 0;
  virtual ~Effect(){  std::cout << "destroy effect" << std::endl;}
};

#endif
