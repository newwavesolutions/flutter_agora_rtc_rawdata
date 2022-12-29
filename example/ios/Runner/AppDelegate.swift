import UIKit
import Flutter
import agora_rtc_rawdata
import FirebaseCore

@UIApplicationMain
@objc class AppDelegate: FlutterAppDelegate, AgoraRawDataProcessorDelegate {
    
    private var effectWrapper: EffectWrapper?
    
    override func application(
        _ application: UIApplication,
        didFinishLaunchingWithOptions launchOptions: [UIApplication.LaunchOptionsKey: Any]?
    ) -> Bool {
        GeneratedPluginRegistrant.register(with: self)
        
        //Init Firebase
        FirebaseApp.configure()
        
        //Init filter wrapper
        effectWrapper = EffectWrapper.init()
        let effectConfig = EffectConfig.default()
        effectWrapper?.prepare(effectConfig)
        effectWrapper?.iFilter = EFFECT_FILTER(
            rawValue:UInt(SWIFT_EFFECT_FILTER.FILTER_COOL.rawValue)
        )
        effectWrapper?.startProcessing()

        // Add delete to process data from lib
        AgoraRawDataProcessor.shared.delegate = self
        
        // Flutter function
        return super.application(application, didFinishLaunchingWithOptions: launchOptions)
    }
}

extension AppDelegate {
    func onCapture(_ videoFrame: AgoraVideoFrame) -> Bool {
        let pixelBuffer = createPixelBufferWithVideoFrame(videoFrame)
//        memset(videoFrame.uBuffer, 0, Int(videoFrame.uStride * videoFrame.height) / 2)
//        memset(videoFrame.vBuffer, 0, Int(videoFrame.vStride * videoFrame.height) / 2)
        
        guard let pixelBuffer = pixelBuffer else { return true }
        
        effectWrapper?.apply(on: nil, on: pixelBuffer)
        
        if CVPixelBufferLockBaseAddress(pixelBuffer, []) == kCVReturnSuccess  {
            
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


enum SWIFT_EFFECT_FILTER: Int, CaseIterable {
    case FILTER_NONE = 2000
    case FILTER_WARM = 2001
    case FILTER_COOL = 2002
    case FILTER_ROSE = 2003
    case FILTER_SKETCH = 2004
    case FILTER_AUTUMN = 2005
    case FILTER_BONE = 2006
    case FILTER_CARTOON = 2007
    case FILTER_FREEZE = 2008
    case FILTER_OCEAN = 2009
    case FILTER_PARULA = 2010
    case FILTER_SUNSET = 2011
    
    func idOf(index: Int) -> Int {
        switch index {
        case 0:
            return SWIFT_EFFECT_FILTER.FILTER_NONE.rawValue
        case 1:
            return SWIFT_EFFECT_FILTER.FILTER_WARM.rawValue
        case 2:
            return SWIFT_EFFECT_FILTER.FILTER_COOL.rawValue
        case 3:
            return SWIFT_EFFECT_FILTER.FILTER_ROSE.rawValue
        case 4:
            return SWIFT_EFFECT_FILTER.FILTER_SKETCH.rawValue
        case 5:
            return SWIFT_EFFECT_FILTER.FILTER_AUTUMN.rawValue
        case 6:
            return SWIFT_EFFECT_FILTER.FILTER_BONE.rawValue
        case 7:
            return SWIFT_EFFECT_FILTER.FILTER_CARTOON.rawValue
        case 8:
            return SWIFT_EFFECT_FILTER.FILTER_FREEZE.rawValue
        case 9:
            return SWIFT_EFFECT_FILTER.FILTER_OCEAN.rawValue
        case 10:
            return SWIFT_EFFECT_FILTER.FILTER_PARULA.rawValue
        case 11:
            return SWIFT_EFFECT_FILTER.FILTER_SUNSET.rawValue
        default:
            return SWIFT_EFFECT_FILTER.FILTER_NONE.rawValue
        }
    }
}
