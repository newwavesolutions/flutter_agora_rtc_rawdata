import UIKit
import Flutter
import agora_rtc_rawdata

@UIApplicationMain
@objc class AppDelegate: FlutterAppDelegate, AgoraRawDataProcessorDelegate {
    override func application(
        _ application: UIApplication,
        didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey: Any]?
    ) -> Bool {
        GeneratedPluginRegistrant.register(with: self)

        AgoraRawDataProcessor.shared.delegate = self

        return super.application(application, didFinishLaunchingWithOptions: launchOptions)
    }
}

extension AppDelegate {
    func onCapture(_ videoFrame: AgoraVideoFrame) -> Bool {
        let pixelBuffer = createPixelBufferWithVideoFrame(videoFrame)
//        memset(videoFrame.uBuffer, 0, Int(videoFrame.uStride * videoFrame.height) / 2)
//        memset(videoFrame.vBuffer, 0, Int(videoFrame.vStride * videoFrame.height) / 2)
        
        
        if let pixelBuffer = pixelBuffer,
              CVPixelBufferLockBaseAddress(pixelBuffer, []) == kCVReturnSuccess  {
            
            let yPlaneWidth = CVPixelBufferGetWidthOfPlane(pixelBuffer, 0)
            let yPlaneHeight = CVPixelBufferGetHeightOfPlane(pixelBuffer, 0)
            
            let uPlaneWidth = CVPixelBufferGetWidthOfPlane(pixelBuffer, 1)
            let uPlaneHeight = CVPixelBufferGetHeightOfPlane(pixelBuffer, 1)
            
            let vPlaneWidth = CVPixelBufferGetWidthOfPlane(pixelBuffer, 2)
            let vPlaneHeight = CVPixelBufferGetHeightOfPlane(pixelBuffer, 2)
            
            
            memset(videoFrame.yBuffer, 0, Int(videoFrame.yStride * videoFrame.height))
            memset(videoFrame.uBuffer, 0, Int(videoFrame.uStride * videoFrame.height) / 2)
            memset(videoFrame.vBuffer, 0, Int(videoFrame.vStride * videoFrame.height) / 2)
            
            let yBuffer = CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 0)
            memcpy(videoFrame.yBuffer, yBuffer, yPlaneWidth * yPlaneHeight)

            let uBuffer = CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 1)
            memcpy(videoFrame.uBuffer, uBuffer, uPlaneWidth * uPlaneHeight)
            
            let vBuffer = CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 2)
            memcpy(videoFrame.vBuffer, vBuffer, vPlaneWidth * vPlaneHeight)
            
            CVPixelBufferUnlockBaseAddress(pixelBuffer, [])
        }
        
        return true
    }
}

//kCVPixelFormatType_420YpCbCr8Planar => ok
//kCVPixelFormatType_420YpCbCr8PlanarFullRange => ok
//kCVPixelFormatType_420YpCbCr8BiPlanarVideoRange => NG
//kCVPixelFormatType_420YpCbCr8BiPlanarFullRange => NG

//kCVPixelFormatType_420YpCbCr10BiPlanarVideoRange => NG
//kCVPixelFormatType_420YpCbCr10BiPlanarFullRange => NG
//kCVPixelFormatType_420YpCbCr8VideoRange_8A_TriPlanar => NG

func createPixelBufferWithVideoFrame(_ frame: AgoraVideoFrame) -> CVPixelBuffer? {
    let attributes: [String: Any] = [
        String(kCVPixelBufferMetalCompatibilityKey): true,
        String(kCVPixelBufferOpenGLCompatibilityKey): true,
        String(kCVPixelBufferIOSurfacePropertiesKey): [
            String(kCVPixelBufferIOSurfaceOpenGLESTextureCompatibilityKey): true,
            String(kCVPixelBufferIOSurfaceOpenGLESFBOCompatibilityKey): true,
            String(kCVPixelBufferIOSurfaceCoreAnimationCompatibilityKey): true
        ]
    ]
    
    var pixelBuffer: CVPixelBuffer? = nil
    let _: CVReturn = CVPixelBufferCreate(kCFAllocatorDefault, Int(frame.width), Int(frame.height), kCVPixelFormatType_420YpCbCr8PlanarFullRange, attributes as CFDictionary, &pixelBuffer)
    
    guard let pixelBuffer = pixelBuffer,
          CVPixelBufferLockBaseAddress(pixelBuffer, []) == kCVReturnSuccess else {
        return nil
    }
    
    let yPlaneWidth = CVPixelBufferGetWidthOfPlane(pixelBuffer, 0)
    let yPlaneHeight = CVPixelBufferGetHeightOfPlane(pixelBuffer, 0)
    
    let uPlaneWidth = CVPixelBufferGetWidthOfPlane(pixelBuffer, 1)
    let uPlaneHeight = CVPixelBufferGetHeightOfPlane(pixelBuffer, 1)
    
    let vPlaneWidth = CVPixelBufferGetWidthOfPlane(pixelBuffer, 2)
    let vPlaneHeight = CVPixelBufferGetHeightOfPlane(pixelBuffer, 2)
    
    let yBuffer = frame.yBuffer
    let yDestination = CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 0)
    memcpy(yDestination, yBuffer, yPlaneWidth * yPlaneHeight)
    
    let uBuffer = frame.uBuffer
    let uDestination = CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 1)
    memcpy(uDestination, uBuffer, uPlaneWidth * uPlaneHeight)

    let vBuffer = frame.vBuffer
    let vDestination = CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 2)
    memcpy(vDestination, vBuffer, vPlaneWidth * vPlaneHeight)
    
    CVPixelBufferUnlockBaseAddress(pixelBuffer, [])
    
    return pixelBuffer
}
