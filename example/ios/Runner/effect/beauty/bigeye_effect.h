#ifndef BIGEYE_EFFECT_H
#define BIGEYE_EFFECT_H

#include "../common/common.h"
#include "../common/effect.h"

// struct beauty_data {
//     std::vector<cv::Point2f> *pvf;
//     float intensity;
// };

class BigEyeEffect : public Effect {

public:
  void init(const effect_config &config);
  cv::Mat apply(cv::Mat &to, const void *data = NULL);
  virtual ~BigEyeEffect();
private:
  float _intensity = 50;
};

#endif

