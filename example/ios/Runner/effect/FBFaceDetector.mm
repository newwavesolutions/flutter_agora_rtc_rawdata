//
//  FBFaceDetector.m
//  live_effect
//
//  Created by trung on 1/23/19.
//  Copyright © 2019 trung. All rights reserved.
//

#import "FBFaceDetector.h"
#import <MLKitFaceDetection/MLKFace.h>
#import <MLKitFaceDetection/MLKFaceDetector.h>
#import <MLKitFaceDetection/MLKFaceDetectorOptions.h>
#import <MLKitVision/MLKitVision.h>
#import <UIKit/UIKit.h>

@interface FBFaceDetector ()

@property (nonatomic, strong) MLKFaceDetector *faceDetector;
@property (nonatomic, strong) NSArray<MLKFace *> *lastFaces;
@property (nonatomic, assign) int detectCount;

@end

@implementation FBFaceDetector

- (id)initWith:(double)scale {
    self = [super init];
    if (self != nil) {
        // Real-time contour detection of multiple faces
        MLKFaceDetectorOptions *options = [[MLKFaceDetectorOptions alloc] init];
        options.contourMode = MLKFaceDetectorContourModeAll;
//        options.contourMode = FIRVisionFaceDetectorContourModeNone;
        self.faceDetector = [MLKFaceDetector faceDetectorWithOptions:options];
        self.scale = scale;
        self.detectCount = 0;
        self.lastFaces = nil;
    }
    return self;
}

- (nullable NSArray<MLKFace *> *) detectFaces:(CVImageBufferRef)imageBuffer rotation:(int)rotate {
    int64_t t = (int64_t) (NSDate.timeIntervalSinceReferenceDate * 1000);
    if (self.detectCount >= 0) {
        // buffer to ciimage
        CIImage *cimage = [[CIImage alloc] initWithCVImageBuffer:imageBuffer];
        int rotation = 1;
        if (rotate >= 0) {
            if (rotate == 0) {
                rotation = kCGImagePropertyOrientationRight;
            } else if (rotate == 1) {
                rotation = kCGImagePropertyOrientationDown;
            } else if (rotate == 2) {
                rotation = kCGImagePropertyOrientationLeft;
            }
            cimage = [cimage imageByApplyingOrientation:rotation];
        }
        cimage = [cimage imageByApplyingOrientation:rotate];
        CIFilter *filter = [CIFilter filterWithName:@"CILanczosScaleTransform"];
        [filter setValue:cimage forKey:kCIInputImageKey];
        [filter setValue:@(self.scale) forKey:kCIInputScaleKey];
        [filter setValue:@(1.0) forKey:kCIInputAspectRatioKey];
        CIImage *ciimage = [filter valueForKey:kCIOutputImageKey];
        
        CIContext *temporaryContext = [CIContext contextWithOptions:nil];
        CGImageRef videoImage = [temporaryContext
                                 createCGImage:ciimage
                                 fromRect:ciimage.extent];
        
        UIImage *uiImage = [UIImage imageWithCGImage:videoImage];
        CGImageRelease(videoImage);
        // create firebase vision image with metadata
        MLKVisionImage *image = [[MLKVisionImage alloc] initWithImage:uiImage];
        image.orientation = UIImageOrientationUp;
//        FIRVisionImageMetadata *metadata = [[FIRVisionImageMetadata alloc] init];
//        image.metadata = metadata;
        NSError *err;
        NSArray<MLKFace *> *faces = [self.faceDetector resultsInImage:image error:&err];
        self.lastFaces = faces;
    }
    self.detectCount -= 1;
    if (self.detectCount < -2) {
        self.detectCount = 0;
    }
    t = (int64_t) (NSDate.timeIntervalSinceReferenceDate * 1000) - t;
//    NSLog(@"detectFaces: %d", (int)t);
    return self.lastFaces;
}

- (nullable NSArray<MLKFace *> *) detectFacesWith:(CMSampleBufferRef)sampleBuffer rotation:(int)rotate
{
    CVImageBufferRef imageBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
    return [self detectFaces:imageBuffer rotation:rotate];
}
@end
