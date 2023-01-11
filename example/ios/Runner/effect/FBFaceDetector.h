//
//  FBFaceDetector.h
//  live_effect
//
//  Created by trung on 1/23/19.
//  Copyright © 2019 trung. All rights reserved.
//

#include <opencv2/opencv.hpp>
#import <Foundation/Foundation.h>
#import <CoreMedia/CoreMedia.h>
#import <MLKitFaceDetection/MLKFace.h>

NS_ASSUME_NONNULL_BEGIN

@interface FBFaceDetector : NSObject

@property (nonatomic, assign) double scale;

- (id) initWith:(double)scale;
- (nullable NSArray<MLKFace *> *) detectFaces:(CVImageBufferRef)imageBuffer rotation:(int)rotate;
- (nullable NSArray<MLKFace *> *) detectFacesWith:(CMSampleBufferRef)sampleBuffer rotation:(int)rotate;

@end

NS_ASSUME_NONNULL_END
