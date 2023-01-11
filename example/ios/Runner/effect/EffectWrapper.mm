//
//  EffectWrapper.m
//  live_effect
//
//  Created by trung on 12/11/18.
//  Copyright © 2018 trung. All rights reserved.
//

#import "facedetector.h"
#import "facemark.h"
#import "EffectWrapper.h"
#import "glasseseffect.h"
#import "noseeffect.h"
#import "stareffect.h"
#import "angeleffect.h"
#import "beareffect.h"
#import "cateffect.h"
#import "cat2effect.h"
#import "corgieffect.h"
#import "devileffect.h"
#import "dogeffect.h"
#import "warnfilter.h"
#import "coolfilter.h"
#import "rosefilter.h"
#import "sketchfilter.h"
#import "autumnfilter.h"
#import "bonefilter.h"
#import "freezefilter.h"
#import "oceanfilter.h"
#import "parulafilter.h"
#import "sunsetfilter.h"
#import "cartoonfilter.h"
#import "scrub_effect.h"
#import "scrub_effect.h"
#import "bigeye_effect.h"
#import "lighten_effect.h"
#import "rosy_effect.h"
#import "soften_effect.h"
#import "vface_effect.h"
#import <UIKit/UIKit.h>
#import "FBFaceDetector.h"
#import <sys/utsname.h>
//#import <GPUImage/GPUImageFilter.h>
//#import <GPUImage/GPUImageOutput.h>
//#import <GPUImage/GPUImageVideoCamera.h>
//#import <GPUImage/GPUImageRawDataOutput.h>
//#import <GPUImage/GPUImageSketchFilter.h>
#import <CoreMedia/CoreMedia.h>

@interface EffectWrapper ()

//@property (atomic) FaceDetector detector;
//@property (atomic) FaceMark mark;
//@property (atomic) GlassesEffect effect;
@property (nonatomic, strong) EffectConfig *config;

@end

@implementation EffectWrapper

FaceDetector detector;
FaceMark mark;
Effect *sticker;
Effect *filter;
Effect *beauty;
beauty_data bData;
EFFECT_STICKER mSticker;
EFFECT_BEAUTY mBeauty;
EFFECT_FILTER mFilter;
FBFaceDetector *fbDetector;
//GPUImageFilter *gpuBeautyFilter;
//GPUImageSketchFilter *gpuSketchFilter;
//GPUImageVideoCamera *gpuVideoCamera;
//GPUImageRawDataOutput *gpuRawDataOutput;
CGPoint singleStepOffset = CGPointMake(2.0, 2.0);
CGSize size;
std::map<int, float> beauties;
std::map<int, Effect*> beautyEffects;
std::vector<int> beautyOrder = {BEAUTY_LIGHTEN, BEAUTY_ROSY, BEAUTY_SOFTEN, BEAUTY_SCRUB, BEAUTY_VFACE, BEAUTY_BIGEYE};

- (void) prepare:(EffectConfig*) config {
    self.config = config;
    self.quality = 50;
    NSString* bundleRoot = [[NSBundle mainBundle] bundlePath];
    detector.init([[NSFileManager defaultManager] fileSystemRepresentationWithPath:[bundleRoot stringByAppendingPathComponent:@"haarcascade_frontalface_alt2.xml"]], self.config.faceScale);
    mark.init("", [[NSFileManager defaultManager] fileSystemRepresentationWithPath:[bundleRoot stringByAppendingPathComponent:@"shape_predictor_5_face_landmarks.dat"]]);
    mSticker = STICKER_NONE;
    self.iSticker = STICKER_NONE;
    mBeauty = BEAUTY_NONE;
    self.iBeauty = BEAUTY_NONE;
    mFilter = FILTER_NONE;
    self.iFilter = FILTER_NONE;
    fbDetector = [[FBFaceDetector alloc] initWith:self.config.faceScale];
    self.beautiful = self.config.beauty;
    
    // gpuimage to run opengl shader
//    gpuVideoCamera = [[GPUImageVideoCamera alloc] initWithSessionPreset:AVCaptureSessionPreset640x480 cameraPosition:AVCaptureDevicePositionBack asYUV:NO];
//
//    size = [self getFrameSize];
//    gpuRawDataOutput = [[GPUImageRawDataOutput alloc] initWithImageSize:size resultsInBGRAFormat:YES];

//    NSError *error;
//    NSString *path = [[NSBundle mainBundle] pathForResource: @"beauty" ofType: @"glsl"];
//    NSString *fsh = [NSString stringWithContentsOfFile:path encoding:NSUTF8StringEncoding error:&error];
//    gpuBeautyFilter = [[GPUImageFilter alloc] initWithFragmentShaderFromString:BEAUTY_S];
//    [gpuBeautyFilter addTarget:gpuRawDataOutput];
//    [gpuBeautyFilter useNextFrameForImageCapture];
    
//    gpuSketchFilter = [[GPUImageSketchFilter alloc] init];
//    [gpuSketchFilter addTarget:gpuRawDataOutput];
//    [gpuSketchFilter useNextFrameForImageCapture];

//    [gpuVideoCamera addTarget:gpuBeautyFilter];
//    [gpuVideoCamera addTarget:gpuSketchFilter];
    
    // initialize all beauty effects
    Effect *effect;
    effect = [self getBeautyEffect:BEAUTY_ROSY];
    beautyEffects.insert(std::make_pair((int)BEAUTY_ROSY, effect));
    effect = [self getBeautyEffect:BEAUTY_LIGHTEN];
    beautyEffects.insert(std::make_pair((int)BEAUTY_LIGHTEN, effect));
    effect = [self getBeautyEffect:BEAUTY_SCRUB];
    beautyEffects.insert(std::make_pair((int)BEAUTY_SCRUB, effect));
    effect = [self getBeautyEffect:BEAUTY_VFACE];
    beautyEffects.insert(std::make_pair((int)BEAUTY_VFACE, effect));
    effect = [self getBeautyEffect:BEAUTY_BIGEYE];
    beautyEffects.insert(std::make_pair((int)BEAUTY_BIGEYE, effect));
    effect = [self getBeautyEffect:BEAUTY_SOFTEN];
    beautyEffects.insert(std::make_pair((int)BEAUTY_SOFTEN, effect));
}

- (int64_t) getCurrentMilis {
    return (int64_t) (NSDate.timeIntervalSinceReferenceDate * 1000);
}

- (void) applyOnImageBuffer:(CVImageBufferRef)imageBuffer OnFramePresentationTime:(CMTime)framePresentationTime OnFrameDuration:(CMTime)frameDuration {
    CMSampleTimingInfo info;
    info.presentationTimeStamp = framePresentationTime;
    info.duration = frameDuration;
    info.decodeTimeStamp = kCMTimeInvalid;
    CMVideoFormatDescriptionRef description = NULL;
    CMVideoFormatDescriptionCreateForImageBuffer(kCFAllocatorDefault, imageBuffer, &description);
    CMSampleBufferRef sampleBuffer = NULL;
    CMSampleBufferCreateReadyWithImageBuffer(kCFAllocatorDefault, imageBuffer, description, &info, &sampleBuffer);
    [self applyOnSampleBuffer:sampleBuffer OnImageBuffer:imageBuffer];
    if (description != NULL) {
        CFRelease(description);
    }
    if (sampleBuffer != NULL) {
        CFRelease(sampleBuffer);
    }
}
- (void) applyOnSampleBuffer:(CMSampleBufferRef)sampleBuffer {
    CVImageBufferRef imageBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
    [self applyOnSampleBuffer:sampleBuffer OnImageBuffer:imageBuffer];
}

- (void) applyOnSampleBuffer:(CMSampleBufferRef)sampleBuffer OnImageBuffer:(CVImageBufferRef)imageBuffer {
    int64_t t = [self getCurrentMilis];
    int rotation = [self getRotation];
#ifdef IMG_DEBUG_LOG
    NSLog(@"rotation: %d", rotation);
#endif
    cv::Mat img;
//    if (self.beautiful) {
//        dispatch_queue_t videoProcessingQueue = [GPUImageContext sharedContextQueue];
//        dispatch_sync(videoProcessingQueue, ^{
//            [gpuBeautyFilter setPoint:singleStepOffset forUniformName:@"singleStepOffset"];
//            [gpuVideoCamera processVideoSampleBuffer:sampleBuffer];
//        });
//        cv::Mat tmp(size.height, size.width, CV_8UC4, gpuRawDataOutput.rawBytesForImage);
//        cv::cvtColor(tmp, img, cv::COLOR_BGRA2BGR);
//    } else {
        img = [self matFromYUVBuffer:imageBuffer];
//    img = [self matFromImageBuffer:imageBuffer];
//    }
    // get effects
    if (self.iFilter != mFilter) {
        filter = [self getEffect:self.iFilter];
        mFilter = self.iFilter;
    }
    if (self.iBeauty != mBeauty) {
        beauty = [self getEffect:self.iBeauty];
        mBeauty = self.iBeauty;
    }
    if (self.iSticker != mSticker) {
        sticker = [self getEffect:self.iSticker];
        mSticker = self.iSticker;
    }
    // rotate if needed
    // always do rotation
//    if (sticker == NULL) {
//        rotation = -1;  // no rotation if sticker disabled
//    }
    if (rotation >= 0) {
        cv::rotate(img, img, rotation);
    }

    BOOL dirty = false;
    std::vector<cv::Rect> faces;
    std::vector< std::vector<cv::Point2f> > landmarks;
    cv::Mat imgOrigin = img.clone();
    cv::Mat imgOriginGray;
    cv::cvtColor(img, imgOriginGray, cv::COLOR_BGR2GRAY);
    dirty = true;
    if (filter != NULL) {
        filter->apply(img);
        dirty = true;
    }
    if (beauties.size() > 0 || sticker != NULL || YES) {
        if (self.config.detectorType == FACE_DETECTOR_OPENCV_CASCADE) {
            detector.detectFaces(imgOriginGray, faces);
        } else {
            NSArray<MLKFace *> *_faces;
            double scale = 1;
            if (self.config.facesDetectionByBuffer && sampleBuffer != nil) {
                _faces = [fbDetector detectFacesWith:sampleBuffer rotation:rotation];
            } else {
                _faces = [fbDetector detectFaces:imageBuffer rotation:rotation];
            }
            scale = self.config.faceScale;
            if (_faces != nil) {
                for (MLKFace *face in _faces) {
                    CGRect frame = face.frame;
                    cv::Rect rect(frame.origin.x/scale, frame.origin.y/scale, frame.size.width/scale, frame.size.height/scale);
                    faces.push_back(rect);
                }
            }
        }
        if (faces.size() > 0) {
            bool success = mark.getMarks(imgOriginGray, faces, landmarks, LANDMARK5);
            if (success) {
                //std::cout << "get marks" << " size: " << landmarks.size() << std::endl;
                // If successful, render the landmarks on the face
                for(int i = 0; i < landmarks.size(); i++) {
//                    if (beauty != NULL) {
//                        bData.pvf = &landmarks[i];
//                        bData.intensity = self.quality;
//                        beauty->apply(img, &bData);
//                        dirty = true;
//                    }
                    // apply all beauties
                    for (auto it = beautyOrder.begin(); it != beautyOrder.end(); ++it) {
                        auto itBeauty = beauties.find(*it);
                        if (itBeauty != beauties.end()) {
                            bData.pvf = &landmarks[i];
                            bData.intensity = itBeauty->second;
                            auto it2 = beautyEffects.find(itBeauty->first);
                            if (it2 != beautyEffects.end() && itBeauty->second > 0) {
                                it2->second->apply(img, &bData);
                            }
                        }
                    }
                    // apply sticker
                    if (sticker != NULL) {
                        sticker->apply(img, &landmarks[i]);
                        dirty = true;
                    }
                }
            }
        }
    }
    
    // rotate back if needed
    if (rotation >= 0) {
        if (rotation == cv::ROTATE_90_COUNTERCLOCKWISE) {
            rotation = cv::ROTATE_90_CLOCKWISE;
        } else if (rotation == cv::ROTATE_90_CLOCKWISE) {
            rotation = cv::ROTATE_90_COUNTERCLOCKWISE;
        }
        cv::rotate(img, img, rotation);
    }

    if (dirty) {
        [self mat:img ToYUVBuffer:imageBuffer];
//        [self mat:img ToImageBuffer:imageBuffer];
    }
    t = [self getCurrentMilis] - t;
#ifdef IMG_DEBUG_LOG
    NSLog(@"total: %d", (int)t);
#endif
}

- (int) getRotation {
    int rotate = cv::ROTATE_90_CLOCKWISE;
    if (self.manager != nil && self.manager.deviceMotion != nil) {
        double angle = abs(atan2(self.manager.deviceMotion.gravity.x, self.manager.deviceMotion.gravity.y) - M_PI);
        if (angle > M_PI_4 && angle < M_PI_2 + M_PI_4) {
            rotate = -1;
        } else if (angle > M_PI_2 + M_PI_4 && angle < M_PI + M_PI_4) {
            rotate = cv::ROTATE_90_COUNTERCLOCKWISE;
        } else if (angle > M_PI + M_PI_4 && angle < 2 * M_PI - M_PI_4) {
            rotate = cv::ROTATE_180;
        }
    }
    
    return rotate;
}

- (int) getRotation4Agora {
    int rotate = -1;
    if (self.manager != nil && self.manager.deviceMotion != nil) {
        double angle = abs(atan2(self.manager.deviceMotion.gravity.x, self.manager.deviceMotion.gravity.y) - M_PI);
        if (angle > M_PI_4 && angle < M_PI_2 + M_PI_4) {
            rotate = cv::ROTATE_90_COUNTERCLOCKWISE;
        } else if (angle > M_PI_2 + M_PI_4 && angle < M_PI + M_PI_4) {
            rotate = cv::ROTATE_180;
        } else if (angle > M_PI + M_PI_4 && angle < 2 * M_PI - M_PI_4) {
            rotate = cv::ROTATE_90_CLOCKWISE;
        }
    }
    
    return rotate;
}

- (void) startProcessing {
    self.manager = [[CMMotionManager alloc] init];
    if ([self.manager isDeviceMotionAvailable]) {
        [self.manager startDeviceMotionUpdates];
    }
}

- (void) stopProcessing {
    if (self.manager != nil) {
        [self.manager stopDeviceMotionUpdates];
    }
}

- (CGSize) getFrameSize
{
    CGSize size = CGSizeMake(288, 352);
    if (self.config.videoSize == FACE_VIDEO_640) {
        size = CGSizeMake(480, 640);
    } else if (self.config.videoSize == FACE_VIDEO_720) {
        size = CGSizeMake(720, 1280);
    } else if (self.config.videoSize == FACE_VIDEO_1280) {
        size = CGSizeMake(1280, 720);
    }
    return size;
}

- (cv::Mat) matFromImageBuffer:(CVImageBufferRef) imageBuffer
{
    // lock for reading
    CVPixelBufferLockBaseAddress(imageBuffer, kCVPixelBufferLock_ReadOnly);
    int width = (int)CVPixelBufferGetWidth(imageBuffer);
    int height = (int)CVPixelBufferGetHeight(imageBuffer);
    
    cv::Mat img;
    char *baseBuffer = (char *)CVPixelBufferGetBaseAddress(imageBuffer);
    cv::Mat mat = cv::Mat(height, width, CV_8UC4, baseBuffer, 0);
    cv::cvtColor(mat, img, cv::COLOR_BGRA2BGR);
    // unlock buffer again until we need it again
    CVPixelBufferUnlockBaseAddress(imageBuffer, kCVPixelBufferLock_ReadOnly);
    
    return img;
}

- (void) mat:(cv::Mat) img ToImageBuffer:(CVImageBufferRef) imageBuffer
{
    // lock for writing
    CVPixelBufferLockBaseAddress(imageBuffer, 0);
    
    cv::Mat mat;
    char *baseBuffer = (char *)CVPixelBufferGetBaseAddress(imageBuffer);
    cv::cvtColor(img, mat, cv::COLOR_BGR2BGRA);
    memcpy(baseBuffer, mat.data, mat.total()*4);
    // unlock buffer again until we need it again
    CVPixelBufferUnlockBaseAddress(imageBuffer, 0);
}

- (cv::Mat) matFromYUVBuffer:(CVImageBufferRef) imageBuffer
{
    // lock for reading
    CVPixelBufferLockBaseAddress(imageBuffer, kCVPixelBufferLock_ReadOnly);
    
    cv::Mat img;
    cv::Mat plane1 = [self buffer:imageBuffer ofPlane:0 withType:CV_8UC1];
    cv::Mat plane2 = [self buffer:imageBuffer ofPlane:1 withType:CV_8UC2];
    cv::cvtColorTwoPlane(plane1, plane2, img, cv::COLOR_YUV2BGR_NV21);
    // unlock buffer again until we need it again
    CVPixelBufferUnlockBaseAddress(imageBuffer, kCVPixelBufferLock_ReadOnly);
    
    return img;
}

- (cv::Mat) buffer:(CVImageBufferRef) imageBuffer ofPlane:(size_t) plane withType:(int) type
{
    char *planeBuffer = (char *)CVPixelBufferGetBaseAddressOfPlane(imageBuffer, plane);
    int width = (int)CVPixelBufferGetWidthOfPlane(imageBuffer, plane);
    int height = (int)CVPixelBufferGetHeightOfPlane(imageBuffer, plane);
    
    cv::Mat mat = cv::Mat(height, width, type, planeBuffer, 0);
    return mat;
}

- (void) mat:(cv::Mat) img ToYUVBuffer:(CVImageBufferRef) imageBuffer
{
    // lock for writing
    CVPixelBufferLockBaseAddress(imageBuffer, 0);
    
    cv::Mat mat;
    cv::cvtColor(img, mat, cv::COLOR_BGR2YUV_YV12);
    // plane1
    int width = (int)CVPixelBufferGetWidthOfPlane(imageBuffer, 0);
    int height = (int)CVPixelBufferGetHeightOfPlane(imageBuffer, 0);
    cv::Mat plane1 = mat(cv::Rect(0, 0, width, height));
    char *plane1Buffer = (char *)CVPixelBufferGetBaseAddressOfPlane(imageBuffer, 0);
    memcpy(plane1Buffer, plane1.data, plane1.total());
    
    // plane2
    int width2 = (int)CVPixelBufferGetWidthOfPlane(imageBuffer, 1);
    int height2 = (int)CVPixelBufferGetHeightOfPlane(imageBuffer, 1);
    cv::Mat plane2 = mat(cv::Rect(0, height, width2*2, height2/2));
    cv::Mat plane22 = mat(cv::Rect(0, height+height2/2, width2*2, height2/2));
    cv::Mat planes;
    std::vector<cv::Mat> uv{plane2, plane22};
    cv::merge(uv, planes);
    char *plane2Buffer = (char *)CVPixelBufferGetBaseAddressOfPlane(imageBuffer, 1);
    memcpy(plane2Buffer, planes.data, planes.total()*2);
    
    // unlock buffer again until we need it again
    CVPixelBufferUnlockBaseAddress(imageBuffer, 0);
}

- (void) setBeauty:(EFFECT_BEAUTY)beauty Quality:(float) quality
{
    auto it = beauties.find((int)beauty);
    if (it != beauties.end()) {
        it->second = quality;
    } else {
        beauties.insert(std::make_pair((int)beauty, quality));
    }
}

- (void) removeBeauty:(EFFECT_BEAUTY)beauty
{
    beauties.erase((int)beauty);
}

- (void) resetBeauty
{
    beauties.clear();
}

- (void) updateVideoSize:(FACE_VIDEO_SIZE) vsize {
    self.config.videoSize = vsize;
//    [gpuBeautyFilter removeAllTargets];
//    size = [self getFrameSize];
//    gpuRawDataOutput = [[GPUImageRawDataOutput alloc] initWithImageSize:size resultsInBGRAFormat:YES];
//    [gpuBeautyFilter addTarget:gpuRawDataOutput];
}

- (Effect*) getBeautyEffect:(EFFECT_BEAUTY)index {
    switch (index) {
        case BEAUTY_SCRUB:
            return new ScrubEffect();
            break;
        case BEAUTY_ROSY:
        {
            Effect *e = new RosyEffect();
            effect_config ec;
            e->init(ec);
            return e;
        }
            break;
        case BEAUTY_BIGEYE:
        {
            Effect *e = new BigEyeEffect();
            effect_config ec;
            e->init(ec);
            return e;
        }
            break;
        case BEAUTY_LIGHTEN:
        {
            Effect *e = new LightenEffect();
            effect_config ec;
            e->init(ec);
            return e;
        }
            break;
        case BEAUTY_SOFTEN:
        {
            Effect *e = new SoftenEffect();
            effect_config ec;
            e->init(ec);
            return e;
        }
            break;
        case BEAUTY_VFACE:
        {
            Effect *e = new VFaceEffect();
            effect_config ec;
            e->init(ec);
            return e;
        }
            break;

        default:
            break;
    }
    return NULL;
}

- (Effect*) getStickerEffect:(EFFECT_STICKER)index {
    NSString* bundleRoot = [[NSBundle mainBundle] bundlePath];
    switch (index) {
        case STICKER_MIKE_GLASSES:
        {
            effect_config ec = {[[NSFileManager defaultManager] fileSystemRepresentationWithPath:[bundleRoot stringByAppendingPathComponent:@"glass_mice.png"]], [[NSFileManager defaultManager] fileSystemRepresentationWithPath:[bundleRoot stringByAppendingPathComponent:@"glass_mice.txt"]]};
            Effect *e = new GlassesEffect();
            e->init(ec);
            return e;
        }
            break;
        case STICKER_STAR_GLASSES:
        {
            effect_config ec = {"",
                [[NSFileManager defaultManager] fileSystemRepresentationWithPath:[bundleRoot stringByAppendingPathComponent:@"star.txt"]],
                [[NSFileManager defaultManager] fileSystemRepresentationWithPath:[bundleRoot stringByAppendingPathComponent:@"/"]],
                "Comp 1_*.png"
            };
            Effect *e = new StarEffect();
            e->init(ec);
            return e;
        }
            break;
        case STICKER_MIKE_NOSE:
        {
            
            effect_config ec = {"",
                [[NSFileManager defaultManager] fileSystemRepresentationWithPath:[bundleRoot stringByAppendingPathComponent:@"nose.txt"]],
                [[NSFileManager defaultManager] fileSystemRepresentationWithPath:[bundleRoot stringByAppendingPathComponent:@"/"]],
                "Comp 2_*.png"
            };
            Effect *e = new NoseEffect();
            e->init(ec);
            return e;
        }
            break;
        case STICKER_ANGEL:
        {
            
            effect_config ec = {"",
                [[NSFileManager defaultManager] fileSystemRepresentationWithPath:[bundleRoot stringByAppendingPathComponent:@"angel.txt"]],
                [[NSFileManager defaultManager] fileSystemRepresentationWithPath:[bundleRoot stringByAppendingPathComponent:@"/"]],
                "angel*.png"
            };
            Effect *e = new AngelEffect();
            e->init(ec);
            return e;
        }
            break;
        case STICKER_BEAR:
        {
            
            effect_config ec = {"",
                [[NSFileManager defaultManager] fileSystemRepresentationWithPath:[bundleRoot stringByAppendingPathComponent:@"bear.txt"]],
                [[NSFileManager defaultManager] fileSystemRepresentationWithPath:[bundleRoot stringByAppendingPathComponent:@"/"]],
                "bear*.png"
            };
            Effect *e = new BearEffect();
            e->init(ec);
            return e;
        }
            break;
        case STICKER_CAT:
        {
            
            effect_config ec = {"",
                [[NSFileManager defaultManager] fileSystemRepresentationWithPath:[bundleRoot stringByAppendingPathComponent:@"cat.txt"]],
                [[NSFileManager defaultManager] fileSystemRepresentationWithPath:[bundleRoot stringByAppendingPathComponent:@"/"]],
                "cat 2*.png"
            };
            Effect *e = new CatEffect();
            e->init(ec);
            return e;
        }
            break;
        case STICKER_CAT2:
        {
            
            effect_config ec = {"",
                [[NSFileManager defaultManager] fileSystemRepresentationWithPath:[bundleRoot stringByAppendingPathComponent:@"cat2.txt"]],
                [[NSFileManager defaultManager] fileSystemRepresentationWithPath:[bundleRoot stringByAppendingPathComponent:@"/"]],
                "cat 3*.png"
            };
            Effect *e = new Cat2Effect();
            e->init(ec);
            return e;
        }
            break;
        case STICKER_CORGI:
        {
            
            effect_config ec = {"",
                [[NSFileManager defaultManager] fileSystemRepresentationWithPath:[bundleRoot stringByAppendingPathComponent:@"corgi.txt"]],
                [[NSFileManager defaultManager] fileSystemRepresentationWithPath:[bundleRoot stringByAppendingPathComponent:@"/"]],
                "corgi*.png"
            };
            Effect *e = new CorgiEffect();
            e->init(ec);
            return e;
        }
            break;
        case STICKER_DEVIL:
        {
            
            effect_config ec = {"",
                [[NSFileManager defaultManager] fileSystemRepresentationWithPath:[bundleRoot stringByAppendingPathComponent:@"devil.txt"]],
                [[NSFileManager defaultManager] fileSystemRepresentationWithPath:[bundleRoot stringByAppendingPathComponent:@"/"]],
                "devil*.png"
            };
            Effect *e = new DevilEffect();
            e->init(ec);
            return e;
        }
            break;
        case STICKER_DOG:
        {
            
            effect_config ec = {"",
                [[NSFileManager defaultManager] fileSystemRepresentationWithPath:[bundleRoot stringByAppendingPathComponent:@"dog.txt"]],
                [[NSFileManager defaultManager] fileSystemRepresentationWithPath:[bundleRoot stringByAppendingPathComponent:@"/"]],
                "dog*.png"
            };
            Effect *e = new DogEffect();
            e->init(ec);
            return e;
        }
            break;
        default:
            break;
    }
    return NULL;
}

- (Effect*) getFilterEffect:(EFFECT_FILTER)index {
    NSString* bundleRoot = [[NSBundle mainBundle] bundlePath];
    switch (index) {
        case FILTER_WARN:
        {
            Effect *e = new WarnFilter();
            effect_config ec;
            e->init(ec);
            return e;
        }
            break;
        case FILTER_COOL:
        {
            Effect *e = new CoolFilter();
            effect_config ec;
            e->init(ec);
            return e;
        }
            break;
        case FILTER_ROSE:
        {
            Effect *e = new RoseFilter();
            effect_config ec;
            e->init(ec);
            return e;
        }
            break;
        case FILTER_SKETCH:
        {
            Effect *e = new SketchFilter();
            std::string meta = "352x288";
            if (self.config.videoSize == FACE_VIDEO_640) {
                meta = "640x480";
            } else if (self.config.videoSize == FACE_VIDEO_720) {
                meta = "720x1280";
            } else if (self.config.videoSize == FACE_VIDEO_1280) {
                meta = "1280x720";
            }
            effect_config ec = {[[NSFileManager defaultManager] fileSystemRepresentationWithPath:[bundleRoot stringByAppendingPathComponent:@"pencilsketch_bg.jpg"]], meta};
            e->init(ec);
            return e;
        }
            break;
        case FILTER_AUTUMN:
        {
            Effect *e = new AutumnFilter();
            effect_config ec;
            e->init(ec);
            return e;
        }
            break;
        case FILTER_BONE:
        {
            Effect *e = new BoneFilter();
            effect_config ec;
            e->init(ec);
            return e;
        }
            break;
        case FILTER_FREEZE:
        {
            Effect *e = new FreezeFilter();
            effect_config ec;
            e->init(ec);
            return e;
        }
            break;
        case FILTER_OCEAN:
        {
            Effect *e = new OceanFilter();
            effect_config ec;
            e->init(ec);
            return e;
        }
            break;
        case FILTER_PARULA:
        {
            Effect *e = new ParulaFilter();
            effect_config ec;
            e->init(ec);
            return e;
        }
            break;
        case FILTER_SUNSET:
        {
            Effect *e = new SunsetFilter();
            effect_config ec;
            e->init(ec);
            return e;
        }
            break;
        case FILTER_CARTOON:
        {
            Effect *e = new CartoonFilter();
            effect_config ec;
            e->init(ec);
            return e;
        }
            break;

        default:
            break;
    }
    return NULL;
}

- (Effect*) getEffect:(NSUInteger)index {
    if (index < 1000) {
        if (sticker != NULL) {
            delete sticker;
            sticker = NULL;
        }
        return [self getStickerEffect:(EFFECT_STICKER)index];
    } else if (index < 2000) {
        if (beauty != NULL) {
            delete beauty;
            beauty = NULL;
        }
        return [self getBeautyEffect:(EFFECT_BEAUTY)index];
    } else if (index > 2000) {
        if (filter != NULL) {
            delete filter;
            filter = NULL;
        }
        return [self getFilterEffect:(EFFECT_FILTER)index];
    }
    
    return NULL;
}

- (void) free {
    if (sticker != NULL) {
        delete sticker;
        sticker = NULL;
    }
    if (filter != NULL) {
        delete filter;
        filter = NULL;
    }
    if (beauty != NULL) {
        delete beauty;
        beauty = NULL;
    }
}

- (void) dealloc {
    [self free];
    for (auto it = beautyEffects.begin(); it != beautyEffects.end(); ++it) {
        Effect *e = it->second;
        delete e;
        it++;
    }
    beautyEffects.clear();
}

@end

@implementation EffectConfig

NSString* deviceName()
{
    struct utsname systemInfo;
    uname(&systemInfo);
    
    NSString *str = [NSString stringWithCString:systemInfo.machine encoding:NSUTF8StringEncoding];
    NSArray *arr = [str componentsSeparatedByString:@","];
    return arr[0]; // first model name
}

+ (EffectConfig*) defaultConfig {
    EffectConfig *config = [[EffectConfig alloc] init];
    NSString *model = deviceName();
//    model = @"iPhone10";
    NSNumberFormatter *f = [[NSNumberFormatter alloc] init];
    f.numberStyle = NSNumberFormatterDecimalStyle;
    config.faceScale = 0.2;
    config.detectorType = FACE_DETECTOR_GG_MLKIT;
    config.videoSize = FACE_VIDEO_640;
//    if ([model hasPrefix:@"iPhone"]) {
//        NSNumber *myNumber = [f numberFromString:[model componentsSeparatedByString:@"e"][1]];
//        if (myNumber.integerValue < 10) {    // iphone 7 and earlier
//            config.faceScale = 0.25;
//            config.detectorType = FACE_DETECTOR_GG_MLKIT;
//            config.videoSize = FACE_VIDEO_355;
//        } else {
//            config.faceScale = 0.15;
//            config.detectorType = FACE_DETECTOR_GG_MLKIT;
//            config.videoSize = FACE_VIDEO_640;
//        }
//    } else if ([model hasPrefix:@"iPad"]) {
//        // ipad
//        NSNumber *myNumber = [f numberFromString:[model componentsSeparatedByString:@"d"][1]];
//        if (myNumber.integerValue < 4) {    // ipad 4 and earlier
//            config.faceScale = 0.25;
//            config.detectorType = FACE_DETECTOR_GG_MLKIT;
//            config.videoSize = FACE_VIDEO_355;
//        } else {
//            config.faceScale = 0.25;
//            config.detectorType = FACE_DETECTOR_GG_MLKIT;
//            config.videoSize = FACE_VIDEO_640;
//        }
//    } else {
//        config.faceScale = 0.25;
//        config.detectorType = FACE_DETECTOR_OPENCV_CASCADE;
//        config.videoSize = FACE_VIDEO_355;
//    }
    config.beauty = YES;
    config.facesDetectionByBuffer = NO;

    return config;
}

@end
