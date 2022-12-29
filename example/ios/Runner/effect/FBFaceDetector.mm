//
//  FBFaceDetector.m
//  live_effect
//
//  Created by trung on 1/23/19.
//  Copyright © 2019 trung. All rights reserved.
//

#import "FBFaceDetector.h"

@interface FBFaceDetector ()

@property (nonatomic, strong) FIRVisionFaceDetector *faceDetector;

@end

@implementation FBFaceDetector

- (id)initWith:(double)scale {
    self = [super init];
    if (self != nil) {
        // Real-time contour detection of multiple faces
        FIRVisionFaceDetectorOptions *options = [[FIRVisionFaceDetectorOptions alloc] init];
        options.contourMode = FIRVisionFaceDetectorContourModeNone;
        FIRVision *vision = [FIRVision vision];
        self.faceDetector = [vision faceDetectorWithOptions:options];
        self.scale = scale;
    }
    return self;
}

- (nullable NSArray<FIRVisionFace *> *) detectFaces:(CVImageBufferRef)imageBuffer rotation:(int)rotate {
    int64_t t = (int64_t) (NSDate.timeIntervalSinceReferenceDate * 1000);
    // buffer to ciimage
    CIImage *cimage = [[CIImage alloc] initWithCVImageBuffer:imageBuffer];
    int rotation = 0;
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
    
    // scale down
    CIFilter *filter = [CIFilter filterWithName:@"CILanczosScaleTransform"];
    [filter setValue:cimage forKey:kCIInputImageKey];
    [filter setValue:@(self.scale) forKey:kCIInputScaleKey];
    [filter setValue:@(1.0) forKey:kCIInputAspectRatioKey];
    CIImage *ciimage = [filter valueForKey:kCIOutputImageKey];
    CIContext *context = [CIContext contextWithOptions:nil];
    CGImageRef ref = [context createCGImage:ciimage fromRect:ciimage.extent];
    // convert to uiimage
    UIImage *tmp = [UIImage imageWithCGImage:ref];
    CFRelease(ref);
    // create firebase vision image with metadata
    FIRVisionImage *image = [[FIRVisionImage alloc] initWithImage:tmp];
    FIRVisionImageMetadata *metadata = [[FIRVisionImageMetadata alloc] init];
    image.metadata = metadata;
    NSError *err;
    NSArray<FIRVisionFace *> *faces = [self.faceDetector resultsInImage:image error:&err];
    t = (int64_t) (NSDate.timeIntervalSinceReferenceDate * 1000) - t;
    //NSLog(@"detectFaces: %d", (int)t);
    return faces;
}

- (nullable NSArray<FIRVisionFace *> *) detectFacesWith:(CMSampleBufferRef)sampleBuffer rotation:(int)rotate
{
    CVImageBufferRef imageBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
    return [self detectFaces:imageBuffer rotation:rotate];
}
@end
