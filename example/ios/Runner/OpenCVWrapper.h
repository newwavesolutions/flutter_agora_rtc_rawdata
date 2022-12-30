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

@interface OpenCVWrapper : NSObject

- (nullable CVPixelBufferRef) convertFrame2CVPixelBuffer: (AgoraVideoFrame *) frame;

- (void) myAssign: (CVPixelBufferRef) pixelBuffer to: (AgoraVideoFrame *) frame;

@end

NS_ASSUME_NONNULL_END
