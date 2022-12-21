#ifndef LIVEEFFECT_COMMON_BLEND_H
#define LIVEEFFECT_COMMON_BLEND_H

#include "common.h"

float LineAngle(const cv::Point& a, const cv::Point& b);
float GeometryHelper_Radian2Degree(float rad);
cv::Mat alpha_blend_8u(cv::Mat template8u4, cv::Mat base8u);
void alpha_blend_8u_self(cv::Mat template8u4, cv::Mat base8u);
void alpha_blend_8u_color_self(cv::Vec3b& color, cv::Mat base8u, float alpha, float beta, PointVec& pts) ;

void alpha_blend_8u_color_self2(cv::Vec3b& color, cv::Mat base8u, float alpha, float beta) ;
void alpha_blend_8u_color_self3(cv::Vec3b& color, cv::Mat base8u, float alpha, float beta, cv::Mat mask) ;

void alpha_blend_8u_color_self4(cv::Mat effect, cv::Mat base8u, float alpha, cv::Mat mask) ;
void alpha_blend_8u_color_self5(cv::Mat effect, cv::Mat base8u, float alpha);
void alpha_blend_8u_color_self6(cv::Vec3b& color, cv::Mat base8u, cv::Mat alpha, float beta, cv::Mat mask);
void alpha_blend_8u_color_self7(cv::Mat effect, cv::Mat base8u, cv::Mat mask);
PointVecf pointAllCircle(PointVecf& pvf);
PointVecf pointAllCircleInner(PointVecf& pvf);
void Dilation(cv::Mat src, cv::Mat dilation_dst, int dilation_elem, int dilation_size);
PointVecf pvoutside(PointVecf& pvf_origin);
PointVec pvf2pv(PointVecf& pvf);
void scale_channel_u(cv::Mat& mat, int channel, float scale);
void weight_channel_u(cv::Mat& mat, int channel, float value, float weight);
#endif
