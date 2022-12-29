#include "stickereffect.h"
#include <fstream>

cv::Mat StickerEffect::get_template() {
  //index++;
  if (_image_list.size() > 0) {
    /*if (_image_list_iter is nullptr) 
    _image_list_iter = _image_list.begin();
    }*/
    std::list<cv::Mat>::iterator cur_iter = _image_list_iter;
    _image_list_iter++;// = std::next(_image_list_iter);
    if (_image_list_iter == _image_list.end())
      _image_list_iter = _image_list.begin();
    //if (img_transformed.data == nullptr) {
    //  img_transformed = cv::Mat(cur_iter->rows, cur_iter->cols, cur_iter->type());
    //}
    //std::cout << "Image index " << index << " : " << std::distance(_image_list.begin(), cur_iter) << std::endl;
    return *cur_iter;
  }
  else if (_image.data != nullptr) {
    return _image;
  }
  else {
    return cv::Mat();
  }
}

void StickerEffect::init(const effect_config &config) {
  if(config.image != "")
    this->_image = cv::imread(config.image, -1);
  
  if(config.image_folder != ""){
    std::vector<cv::String> fnames;
    cv::glob(config.image_folder + "/" + config.name_pattern, fnames, false);
    std::sort(fnames.begin(), fnames.end());
    for(auto f : fnames){
#ifdef __ANDROID__
      cv::Mat tmp = cv::imread(f, -1);
      cv::Mat tmp2;
      cv::cvtColor(tmp, tmp2, cv::COLOR_BGRA2RGBA);
      this->_image_list.push_back(tmp2);
#else
      this->_image_list.push_back(cv::imread(f, -1));  
#endif
    }
    this->_image_list_iter = this->_image_list.begin();
  }
  std::ifstream ifs(config.meta);
  while (ifs.good()) {
    int x, y;
    ifs >> x >> y;
    if(ifs.good()) {
      this->_pvf.push_back(cv::Point2f(x, y));
    }
  }
}

StickerEffect::~StickerEffect() {
  std::cout << "destroy sticker effect" << std::endl;
}
