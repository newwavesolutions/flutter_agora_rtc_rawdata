#ifndef LIVESTREAM_FILTER_H
#define LIVESTREAM_FILTER_H

#include "../common/effect.h"

class FilterEffect: public Effect {
 public:
  void init(const effect_config &config);
  std::vector<int> createLUT(std::vector<double> &x, std::vector<double> &y);
  inline uchar saturate_cast_uchar(double val) {
    val += 0.5; // to round the value
    return uchar(val < 0 ? 0 : (val > 0xff ? 0xff : val));
  }
  
  virtual ~FilterEffect();
};

#endif
