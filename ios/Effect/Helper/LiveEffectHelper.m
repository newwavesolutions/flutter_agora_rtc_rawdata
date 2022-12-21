//
//  OpenCVWrapper.m
//  Runner
//
//  Created by Lê Thọ Sơn on 23/12/2022.
//

#import "LiveEffectHelper.h"
#import <UIKit/UIKit.h>
#import <agora_rtc_rawdata/agora_rtc_rawdata.h>

@implementation LiveEffectHelper

-  (nullable CVPixelBufferRef) convertVideoFrame2PixelBuffer: (AgoraVideoFrame *) frame {
    int width = frame.width;
    int height = frame.height;
    unsigned char *yData = frame.yBuffer;
    unsigned char *uData = frame.uBuffer;
    unsigned char *vData = frame.vBuffer;
    int yStride = frame.yStride;
    int uStride = frame.uStride;
    int vStride = frame.vStride;
    
    // create pixelbuffer from raw video data
    NSDictionary *pixelAttributes = @{(NSString *)kCVPixelBufferIOSurfacePropertiesKey:@{}};
    CVPixelBufferRef pixelBuffer = NULL;
    CVReturn result = CVPixelBufferCreate(kCFAllocatorDefault,
                                            width,
                                            height,
                                            kCVPixelFormatType_420YpCbCr8BiPlanarFullRange,   //  NV12
                                            (__bridge CFDictionaryRef)(pixelAttributes),
                                            &pixelBuffer);
    if (result != kCVReturnSuccess) {
        NSLog(@"Unable to create cvpixelbuffer %d", result);
    }
    CVPixelBufferLockBaseAddress(pixelBuffer, 0);
    unsigned char *yDestPlane = (unsigned char *)CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 0);
    for (int i = 0, k = 0; i < height; i ++) {
        for (int j = 0; j < width; j ++) {
            yDestPlane[k++] = yData[j + i * yStride];
        }
    }
    unsigned char *uvDestPlane = (unsigned char *)CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 1);
    for (int i = 0, k = 0; i < height / 2; i ++) {
        for (int j = 0; j < width / 2; j ++) {
            uvDestPlane[k++] = uData[j + i * uStride];
            uvDestPlane[k++] = vData[j + i * vStride];
        }
    }
    CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
    return pixelBuffer;
}

- (void)convertPixelBuffer2VideoFrame: (CVPixelBufferRef)pixelBuffer to:(AgoraVideoFrame *)frame {
    unsigned char *yData = frame.yBuffer;
    unsigned char *uData = frame.uBuffer;
    unsigned char *vData = frame.vBuffer;
    CVPixelBufferLockBaseAddress(pixelBuffer, 0);
    unsigned char *ySrcPlane = (unsigned char *)CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 0);
    unsigned char *uvSrcPlane = (unsigned char *)CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 1);
    
    int width = frame.width;
    int height = frame.height;
    
    for (int i = 0; i < width * height / 2; i ++) {
        if(i % 2 == 0) {
            uData[i / 2] = uvSrcPlane[i];
        } else {
            vData[(i - 1) / 2] = uvSrcPlane[i];
        }
    }
    
    memcpy(yData, ySrcPlane, width * height);
    
    CVPixelBufferUnlockBaseAddress(pixelBuffer, 0);
}
@end
