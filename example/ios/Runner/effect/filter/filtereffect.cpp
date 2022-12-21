#include "./filtereffect.h"
#include "../common/spline.h"

void FilterEffect::init(const effect_config &config) {
}

std::vector<int> FilterEffect::createLUT(std::vector<double> &x, std::vector<double> &y) {
  tk::spline s;
  s.set_points(x, y);
  std::vector<int> v3;
  for(size_t i = 0; i < 256; i++) {
    v3.push_back((int)s(i));
  }
  return v3;
}

FilterEffect::~FilterEffect() {
}
