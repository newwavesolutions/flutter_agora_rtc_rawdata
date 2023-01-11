//
//  OpenCVWrapper.h
//  Runner
//
//  Created by Lê Thọ Sơn on 23/12/2022.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <agora_rtc_rawdata/agora_rtc_rawdata.h>

NS_ASSUME_NONNULL_BEGIN

@interface LiveEffectHelper : NSObject

- (nullable CVPixelBufferRef) convertVideoFrame2PixelBuffer: (AgoraVideoFrame *) frame;

- (void) convertPixelBuffer2VideoFrame: (CVPixelBufferRef) pixelBuffer to: (AgoraVideoFrame *) frame;

@end

NS_ASSUME_NONNULL_END
