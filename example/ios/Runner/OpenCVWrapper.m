//
//  OpenCVWrapper.m
//  Runner
//
//  Created by Lê Thọ Sơn on 23/12/2022.
//

#import "OpenCVWrapper.h"
#import <UIKit/UIKit.h>
#import <agora_rtc_rawdata/agora_rtc_rawdata.h>

@implementation OpenCVWrapper

-  (nullable CVPixelBufferRef) convertFrame2CVPixelBuffer: (AgoraVideoFrame *) frame {
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

- (void)myAssign: (CVPixelBufferRef)pixelBuffer to:(AgoraVideoFrame *)frame {
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

//- (CVPixelBufferRef)convert2PlaneTo3:(CVPixelBufferRef) oldData {
//    CVPixelBufferLockBaseAddress(oldData, 0);
//    unsigned char *ySrcPlane = (unsigned char *)CVPixelBufferGetBaseAddressOfPlane(oldData, 0);
//    unsigned char *uvDestPlane = (unsigned char *)CVPixelBufferGetBaseAddressOfPlane(oldData, 1);
//
//    unsigned long yPlaneWidth = CVPixelBufferGetWidthOfPlane(oldData, 0);
//    unsigned long yPlaneHeight = CVPixelBufferGetHeightOfPlane(oldData, 0);
//
//    unsigned char *uData = (unsigned char *)calloc(yPlaneWidth * yPlaneHeight / 4, sizeof(unsigned char));
//    unsigned char *vData = (unsigned char *)calloc(yPlaneWidth * yPlaneHeight / 4, sizeof(unsigned char));
//
//    for (int i = 0; i < yPlaneWidth * yPlaneHeight / 2; i ++) {
//        if(i % 2 == 0) {
//            uData[i / 2] = uvDestPlane[i];
//        } else {
//            vData[(i - 1) / 2] = uvDestPlane[i];
//        }
//    }
//
//
//    NSDictionary *pixelAttributes = @{(NSString *)kCVPixelBufferIOSurfacePropertiesKey:@{}};
//    CVPixelBufferRef pixelBuffer = NULL;
//    CVReturn result = CVPixelBufferCreate(kCFAllocatorDefault,
//                                          yPlaneWidth,
//                                          yPlaneHeight,
//                                          kCVPixelFormatType_420YpCbCr8PlanarFullRange,   //  NV12
//                                            (__bridge CFDictionaryRef)(pixelAttributes),
//                                            &pixelBuffer);
//
//
//    unsigned char *yDestPlane = (unsigned char *)CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 0);
//    unsigned char *uDestPlane = (unsigned char *)CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 1);
//    unsigned char *vDestPlane = (unsigned char *)CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 2);
//
//    memcpy(yDestPlane, ySrcPlane, yPlaneWidth * yPlaneHeight);
//    memcpy(uDestPlane, uData, yPlaneWidth * yPlaneHeight / 4);
//    memcpy(vDestPlane, vData, yPlaneWidth * yPlaneHeight / 4);
//    CVPixelBufferUnlockBaseAddress(oldData, 0);
//    return pixelBuffer;
//}

@end
