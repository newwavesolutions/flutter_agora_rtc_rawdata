//
//  EffectWrapper.h
//  live_effect
//
//  Created by trung on 12/11/18.
//  Copyright © 2018 trung. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreMedia/CoreMedia.h>
#import <CoreMotion/CoreMotion.h>

NS_ASSUME_NONNULL_BEGIN

typedef enum : NSUInteger {
    STICKER_NONE = 0,
    STICKER_MIKE_GLASSES = 1,
    STICKER_STAR_GLASSES = 2,
    STICKER_MIKE_NOSE = 3,
    STICKER_ANGEL = 4,
    STICKER_BEAR = 5,
    STICKER_CAT = 6,
    STICKER_CAT2 = 7,
    STICKER_CORGI = 8,
    STICKER_DEVIL = 9,
    STICKER_DOG = 10,
} EFFECT_STICKER;

typedef enum : NSUInteger {
    BEAUTY_NONE = 1000,
    BEAUTY_SCRUB = 1001,
    BEAUTY_BIGEYE = 1002,
    BEAUTY_LIGHTEN = 1003,
    BEAUTY_ROSY = 1004,
    BEAUTY_SOFTEN = 1005,
    BEAUTY_VFACE = 1006
} EFFECT_BEAUTY;

typedef enum : NSUInteger {
    FILTER_NONE = 2000,
    FILTER_WARN = 2001,
    FILTER_COOL = 2002,
    FILTER_ROSE = 2003,
    FILTER_SKETCH = 2004,
    FILTER_AUTUMN = 2005,
    FILTER_BONE = 2006,
    FILTER_CARTOON = 2007,
    FILTER_FREEZE = 2008,
    FILTER_OCEAN = 2009,
    FILTER_PARULA = 2010,
    FILTER_SUNSET = 2011
} EFFECT_FILTER;

typedef enum : NSUInteger {
    FACE_DETECTOR_OPENCV_CASCADE,
    FACE_DETECTOR_GG_MLKIT
} FACE_DETECTOR_TYPE;

typedef enum : NSUInteger {
    FACE_VIDEO_355,
    FACE_VIDEO_640,
    FACE_VIDEO_720,
    FACE_VIDEO_1280
} FACE_VIDEO_SIZE;

@interface EffectConfig : NSObject

@property (nonatomic, assign) double faceScale;
@property (nonatomic, assign) FACE_DETECTOR_TYPE detectorType;
@property (nonatomic, assign) FACE_VIDEO_SIZE videoSize;
@property (nonatomic, assign) BOOL beauty;
@property (nonatomic, assign) BOOL facesDetectionByBuffer;

+ (EffectConfig*) defaultConfig;

@end

@interface EffectWrapper : NSObject

@property (nonatomic, assign) float quality;
@property (nonatomic, assign) EFFECT_STICKER iSticker;
@property (nonatomic, assign) EFFECT_BEAUTY iBeauty;
@property (nonatomic, assign) EFFECT_FILTER iFilter;
@property (nonatomic, strong) CMMotionManager *manager;
@property (nonatomic, assign) BOOL beautiful;

- (void) prepare:(EffectConfig*) config;
- (void) applyOnSampleBuffer:(CMSampleBufferRef)sampleBuffer;
- (void) applyOnImageBuffer:(CVImageBufferRef)imageBuffer OnFramePresentationTime:(CMTime)framePresentationTime OnFrameDuration:(CMTime)frameDuration;
- (void) applyOnSampleBuffer:(CMSampleBufferRef _Nullable)sampleBuffer OnImageBuffer:(CVImageBufferRef)imageBuffer;
- (void) startProcessing;
- (void) stopProcessing;
- (void) setBeauty:(EFFECT_BEAUTY) beauty Quality:(float) quality;
- (void) removeBeauty:(EFFECT_BEAUTY) beauty;
- (void) resetBeauty;
- (void) updateVideoSize:(FACE_VIDEO_SIZE) vsize;
- (void) free;
@end

NS_ASSUME_NONNULL_END

NSString * const BEAUTY_S = @"precision highp float;\
\
uniform vec2 singleStepOffset;\
uniform sampler2D inputImageTexture;\
varying vec2 textureCoordinate;\
\
const vec4 params = vec4(0.33, 0.63, 0.4, 0.35);\
const vec3 W = vec3(0.299,0.587,0.114);\
const mat3 saturateMatrix = mat3(\
                                 1.1102,-0.0598,-0.061,\
                                 -0.0774,1.0826,-0.1186,\
                                 -0.0228,-0.0228,1.1772);\
\
vec2 blurCoordinates[24];\
\
float hardLight(float color) {\
    if(color <= 0.5) {\
        color = color * color * 2.0;\
    } else {\
        color = 1.0 - ((1.0 - color)*(1.0 - color) * 2.0);\
    }\
    return color;\
}\
\
void main() {\
    vec3 centralColor = texture2D(inputImageTexture, textureCoordinate).rgb;\
    \
    blurCoordinates[0] = textureCoordinate.xy + singleStepOffset * vec2(0.0, -10.0);\
    blurCoordinates[1] = textureCoordinate.xy + singleStepOffset * vec2(0.0, 10.0);\
    blurCoordinates[2] = textureCoordinate.xy + singleStepOffset * vec2(-10.0, 0.0);\
    blurCoordinates[3] = textureCoordinate.xy + singleStepOffset * vec2(10.0, 0.0);\
    blurCoordinates[4] = textureCoordinate.xy + singleStepOffset * vec2(5.0, -8.0);\
    blurCoordinates[5] = textureCoordinate.xy + singleStepOffset * vec2(5.0, 8.0);\
    blurCoordinates[6] = textureCoordinate.xy + singleStepOffset * vec2(-5.0, 8.0);\
    blurCoordinates[7] = textureCoordinate.xy + singleStepOffset * vec2(-5.0, -8.0);\
    blurCoordinates[8] = textureCoordinate.xy + singleStepOffset * vec2(8.0, -5.0);\
    blurCoordinates[9] = textureCoordinate.xy + singleStepOffset * vec2(8.0, 5.0);\
    blurCoordinates[10] = textureCoordinate.xy + singleStepOffset * vec2(-8.0, 5.0);\
    blurCoordinates[11] = textureCoordinate.xy + singleStepOffset * vec2(-8.0, -5.0);\
    blurCoordinates[12] = textureCoordinate.xy + singleStepOffset * vec2(0.0, -6.0);\
    blurCoordinates[13] = textureCoordinate.xy + singleStepOffset * vec2(0.0, 6.0);\
    blurCoordinates[14] = textureCoordinate.xy + singleStepOffset * vec2(6.0, 0.0);\
    blurCoordinates[15] = textureCoordinate.xy + singleStepOffset * vec2(-6.0, 0.0);\
    blurCoordinates[16] = textureCoordinate.xy + singleStepOffset * vec2(-4.0, -4.0);\
    blurCoordinates[17] = textureCoordinate.xy + singleStepOffset * vec2(-4.0, 4.0);\
    blurCoordinates[18] = textureCoordinate.xy + singleStepOffset * vec2(4.0, -4.0);\
    blurCoordinates[19] = textureCoordinate.xy + singleStepOffset * vec2(4.0, 4.0);\
    blurCoordinates[20] = textureCoordinate.xy + singleStepOffset * vec2(-2.0, -2.0);\
    blurCoordinates[21] = textureCoordinate.xy + singleStepOffset * vec2(-2.0, 2.0);\
    blurCoordinates[22] = textureCoordinate.xy + singleStepOffset * vec2(2.0, -2.0);\
    blurCoordinates[23] = textureCoordinate.xy + singleStepOffset * vec2(2.0, 2.0);\
    \
    float sampleColor = centralColor.g * 22.0;\
    sampleColor += texture2D(inputImageTexture, blurCoordinates[0]).g;\
    sampleColor += texture2D(inputImageTexture, blurCoordinates[1]).g;\
    sampleColor += texture2D(inputImageTexture, blurCoordinates[2]).g;\
    sampleColor += texture2D(inputImageTexture, blurCoordinates[3]).g;\
    sampleColor += texture2D(inputImageTexture, blurCoordinates[4]).g;\
    sampleColor += texture2D(inputImageTexture, blurCoordinates[5]).g;\
    sampleColor += texture2D(inputImageTexture, blurCoordinates[6]).g;\
    sampleColor += texture2D(inputImageTexture, blurCoordinates[7]).g;\
    sampleColor += texture2D(inputImageTexture, blurCoordinates[8]).g;\
    sampleColor += texture2D(inputImageTexture, blurCoordinates[9]).g;\
    sampleColor += texture2D(inputImageTexture, blurCoordinates[10]).g;\
    sampleColor += texture2D(inputImageTexture, blurCoordinates[11]).g;\
    sampleColor += texture2D(inputImageTexture, blurCoordinates[12]).g * 2.0;\
    sampleColor += texture2D(inputImageTexture, blurCoordinates[13]).g * 2.0;\
    sampleColor += texture2D(inputImageTexture, blurCoordinates[14]).g * 2.0;\
    sampleColor += texture2D(inputImageTexture, blurCoordinates[15]).g * 2.0;\
    sampleColor += texture2D(inputImageTexture, blurCoordinates[16]).g * 2.0;\
    sampleColor += texture2D(inputImageTexture, blurCoordinates[17]).g * 2.0;\
    sampleColor += texture2D(inputImageTexture, blurCoordinates[18]).g * 2.0;\
    sampleColor += texture2D(inputImageTexture, blurCoordinates[19]).g * 2.0;\
    sampleColor += texture2D(inputImageTexture, blurCoordinates[20]).g * 3.0;\
    sampleColor += texture2D(inputImageTexture, blurCoordinates[21]).g * 3.0;\
    sampleColor += texture2D(inputImageTexture, blurCoordinates[22]).g * 3.0;\
    sampleColor += texture2D(inputImageTexture, blurCoordinates[23]).g * 3.0;\
    sampleColor = sampleColor / 62.0;\
    \
    float highPass = centralColor.g - sampleColor + 0.5;\
    \
    for(int i = 0; i < 5;i++)\
    {\
        highPass = hardLight(highPass);\
    }\
    float luminance = dot(centralColor, W);\
    float alpha = pow(luminance, params.r);\
    \
    vec3 smoothColor = centralColor + (centralColor-vec3(highPass))*alpha*0.1;\
    \
    smoothColor.r = clamp(pow(smoothColor.r, params.g),0.0,1.0);\
    smoothColor.g = clamp(pow(smoothColor.g, params.g),0.0,1.0);\
    smoothColor.b = clamp(pow(smoothColor.b, params.g),0.0,1.0);\
    \
    vec3 screen = vec3(1.0) - (vec3(1.0)-smoothColor) * (vec3(1.0)-centralColor);\
    vec3 lighten = max(smoothColor, centralColor);\
    vec3 softLight = 2.0 * centralColor*smoothColor + centralColor*centralColor\
    - 2.0 * centralColor*centralColor * smoothColor;\
    \
    gl_FragColor = vec4(mix(centralColor, screen, alpha), 1.0);\
    gl_FragColor.rgb = mix(gl_FragColor.rgb, lighten, alpha);\
    gl_FragColor.rgb = mix(gl_FragColor.rgb, softLight, params.b);\
    \
    vec3 satColor = gl_FragColor.rgb * saturateMatrix;\
    gl_FragColor.rgb = mix(gl_FragColor.rgb, satColor, params.a);\
}";
