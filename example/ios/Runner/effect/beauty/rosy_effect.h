#ifndef ROSY_EFFECT_H
#define ROSY_EFFECT_H

#include "../common/common.h"
#include "../common/effect.h"

// struct beauty_data {
//     std::vector<cv::Point2f> *pvf;
//     float intensity;
// };

class RosyEffect : public Effect {

public:
  void init(const effect_config &config);
  cv::Mat apply(cv::Mat &to, const void *data = NULL);
  virtual ~RosyEffect();
private:
  float _intensity = 50;
};

#endif

