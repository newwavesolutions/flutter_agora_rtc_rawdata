import UIKit
import Flutter
import agora_rtc_rawdata
import FirebaseCore
import Accelerate
import CoreMedia
import VideoToolbox

@UIApplicationMain
@objc class AppDelegate: FlutterAppDelegate, AgoraRawDataProcessorDelegate {
    
    private var effectWrapper: EffectWrapper?
    
    let cv = OpenCVWrapper()
    
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
//        effectWrapper?.iFilter = EFFECT_FILTER(
//            rawValue:UInt(SWIFT_EFFECT_FILTER.FILTER_CARTOON.rawValue)
//        )
        let beautifyEnum = EFFECT_BEAUTY(
            rawValue:UInt(SWIFT_EFFECT_BEAUTY.BEAUTY_VFACE.rawValue)
        )
        effectWrapper?.iBeauty = beautifyEnum
        effectWrapper?.setBeauty(
            beautifyEnum,
            quality: 100
        )
//        effectWrapper?.iSticker = EFFECT_STICKER(
//            rawValue:UInt(SWIFT_EFFECT_STICKER.STICKER_CAT.rawValue)
//        )
        effectWrapper?.startProcessing()

        // Add delete to process data from lib
        AgoraRawDataProcessor.shared.delegate = self
        
        // Flutter function
        return super.application(application, didFinishLaunchingWithOptions: launchOptions)
    }
}

extension AppDelegate {
    func onCapture(_ videoFrame: AgoraVideoFrame) -> Bool {
        let pixelBuffer1: CVPixelBuffer? = cv.convertFrame2CVPixelBuffer(videoFrame)?.takeRetainedValue()
//        let pixelBuffer = createPixelBufferWithVideoFrame(videoFrame)
//        memset(videoFrame.uBuffer, 0, Int(videoFrame.uStride * videoFrame.height) / 2)
//        memset(videoFrame.vBuffer, 0, Int(videoFrame.vStride * videoFrame.height) / 2)
        
        guard let pixelBuffer1 = pixelBuffer1 else { return true }
//
        effectWrapper?.apply(on: nil, on: pixelBuffer1)
        cv.myAssign(pixelBuffer1, to: videoFrame)
        
//        guard let pixelBuffer = pixelBuffer else { return true }
//////
//        if CVPixelBufferLockBaseAddress(pixelBuffer, []) == kCVReturnSuccess  {
//
//            let yPlaneWidth = CVPixelBufferGetWidthOfPlane(pixelBuffer, 0)
//            let yPlaneHeight = CVPixelBufferGetHeightOfPlane(pixelBuffer, 0)
//
//            let uPlaneWidth = CVPixelBufferGetWidthOfPlane(pixelBuffer, 1)
//            let uPlaneHeight = CVPixelBufferGetHeightOfPlane(pixelBuffer, 1)
//
//            let vPlaneWidth = CVPixelBufferGetWidthOfPlane(pixelBuffer, 2)
//            let vPlaneHeight = CVPixelBufferGetHeightOfPlane(pixelBuffer, 2)
//
//
//            memset(videoFrame.yBuffer, 0, Int(videoFrame.yStride * videoFrame.height))
//            memset(videoFrame.uBuffer, 0, Int(videoFrame.uStride * videoFrame.height) / 2)
//            memset(videoFrame.vBuffer, 0, Int(videoFrame.vStride * videoFrame.height) / 2)
//
//            let yBuffer = CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 0)
//            memcpy(videoFrame.yBuffer, yBuffer, yPlaneWidth * yPlaneHeight)
//
//            let uBuffer = CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 1)
//            memcpy(videoFrame.uBuffer, uBuffer, uPlaneWidth * uPlaneHeight)
//
//            let vBuffer = CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 2)
//            memcpy(videoFrame.vBuffer, vBuffer, vPlaneWidth * vPlaneHeight)
//
//            CVPixelBufferUnlockBaseAddress(pixelBuffer, [])
//        }

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
    let _: CVReturn = CVPixelBufferCreate(kCFAllocatorDefault, Int(frame.width), Int(frame.height), kCVPixelFormatType_420YpCbCr8BiPlanarFullRange, attributes as CFDictionary, &pixelBuffer)
    
    guard let pixelBuffer = pixelBuffer,
          CVPixelBufferLockBaseAddress(pixelBuffer, []) == kCVReturnSuccess else {
        return nil
    }
//
    let yPlaneWidth = CVPixelBufferGetWidthOfPlane(pixelBuffer, 0)
    let yPlaneHeight = CVPixelBufferGetHeightOfPlane(pixelBuffer, 0)

    let uPlaneWidth = CVPixelBufferGetWidthOfPlane(pixelBuffer, 1)
    let uPlaneHeight = CVPixelBufferGetHeightOfPlane(pixelBuffer, 1)
//
//    let vPlaneWidth = CVPixelBufferGetWidthOfPlane(pixelBuffer, 2)
//    let vPlaneHeight = CVPixelBufferGetHeightOfPlane(pixelBuffer, 2)
    
    
//    memcpy(yDestination, yBuffer, yPlaneWidth * yPlaneHeight)
//
    let yBuffer = frame.yBuffer
    let uBuffer = frame.uBuffer
    let vBuffer = frame.vBuffer
//    let bytesPointer = UnsafeMutableRawPointer.allocate(byteCount: uPlaneWidth * uPlaneHeight * 2, alignment: 1)
//    strcat(bytesPointer, uBuffer)
//    strcat(bytesPointer, vBuffer)
    let yDestination = CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 0)
    memcpy(yDestination, yBuffer,  yPlaneWidth * yPlaneHeight)
    
    let uvDestination = CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 1)
//    uvDestination?.load(fromByteOffset: <#T##Int#>, as: Int.self)
//    vBuffer.

//    let vBuffer = frame.vBuffer
//    let vDestination = CVPixelBufferGetBaseAddressOfPlane(pixelBuffer, 2)
//    memcpy(vDestination, vBuffer, vPlaneWidth * vPlaneHeight)
    
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


enum SWIFT_EFFECT_BEAUTY: Int, CaseIterable {
    case BEAUTY_NONE = 1000
    case BEAUTY_SCRUB = 1001
    case BEAUTY_BIGEYE = 1002
    case BEAUTY_LIGHTEN = 1003
    case BEAUTY_ROSY = 1004
    case BEAUTY_SOFTEN = 1005
    case BEAUTY_VFACE = 1006
    
    var name: String {
        switch self {
        case .BEAUTY_NONE:
            return "Common.Beautify"
        case .BEAUTY_SCRUB:
            return "Beautify.Scrub"
        case .BEAUTY_BIGEYE:
            return "Beautify.BigEyes"
        case .BEAUTY_LIGHTEN:
            return "Beautify.Lighten"
        case .BEAUTY_ROSY:
            return "Beautify.Rosy"
        case .BEAUTY_SOFTEN:
            return "Beautify.Soften"
        case .BEAUTY_VFACE:
            return "Beautify.VLine"
            
        }
    }
    
    var key: String {
        switch self {
        case .BEAUTY_NONE:
            return "BEAUTY_NONE"
        case .BEAUTY_SCRUB:
            return "BEAUTY_SCRUB"
        case .BEAUTY_BIGEYE:
            return "BEAUTY_BIGEYE"
        case .BEAUTY_LIGHTEN:
            return "BEAUTY_LIGHTEN"
        case .BEAUTY_ROSY:
            return "BEAUTY_ROSY"
        case .BEAUTY_SOFTEN:
            return "BEAUTY_SOFTEN"
        case .BEAUTY_VFACE:
            return "BEAUTY_VFACE"
            
        }
    }
    
    var thumb: String {
        switch self {
        case .BEAUTY_NONE:
            return "beautify_ic"
        case .BEAUTY_SCRUB:
            return "scrub"
        case .BEAUTY_BIGEYE:
            return "big_eyes_ic"
        case .BEAUTY_LIGHTEN:
            return "lighten_ic"
        case .BEAUTY_ROSY:
            return "rosy_ic"
        case .BEAUTY_SOFTEN:
            return "soften_ic"
        case .BEAUTY_VFACE:
            return "v_line_ic"
        }
    }
    
    func idOf(index: Int) -> Int {
        switch index {
        case 0:
            return SWIFT_EFFECT_BEAUTY.BEAUTY_NONE.rawValue
        case 1:
            return SWIFT_EFFECT_BEAUTY.BEAUTY_SCRUB.rawValue
        case 2:
            return SWIFT_EFFECT_BEAUTY.BEAUTY_BIGEYE.rawValue
        case 3:
            return SWIFT_EFFECT_BEAUTY.BEAUTY_LIGHTEN.rawValue
        case 4:
            return SWIFT_EFFECT_BEAUTY.BEAUTY_ROSY.rawValue
        case 5:
            return SWIFT_EFFECT_BEAUTY.BEAUTY_SOFTEN.rawValue
        case 6:
            return SWIFT_EFFECT_BEAUTY.BEAUTY_VFACE.rawValue
        default:
            return SWIFT_EFFECT_BEAUTY.BEAUTY_NONE.rawValue
        }
    }

}

enum SWIFT_EFFECT_STICKER: Int, CaseIterable {
    case STICKER_NONE = 0
    case STICKER_MIKE_GLASSES = 1
    case STICKER_STAR_GLASS = 2
    case STICKER_MIKE_NOSE = 3
    
    case STICKER_ANGEL = 4
    case STICKER_BEAR = 5
    case STICKER_CAT = 6
    case STICKER_CAT2 = 7
    case STICKER_CORGI = 8
    case STICKER_DEVIL = 9
    case STICKER_DOG = 10
    
    var name: String {
        switch self {
        case .STICKER_NONE:
            return "Sticker.None"
        case .STICKER_MIKE_GLASSES:
            return "Sticker.MikeGlass"
        case .STICKER_STAR_GLASS:
            return "Sticker.StarGlass"
        case .STICKER_MIKE_NOSE:
            return "Sticker.MikeNose"
        case .STICKER_ANGEL:
            return "Sticker.Angel"
        case .STICKER_BEAR:
            return "Sticker.Bear"
        case .STICKER_CAT:
            return "Sticker.Cat"
        case .STICKER_CAT2:
            return "Sticker.Cat2"
        case .STICKER_CORGI:
            return "Sticker.Corgi"
        case .STICKER_DEVIL:
            return "Sticker.Devil"
        case .STICKER_DOG:
            return "Sticker.Dog"
        }
    }
    
    var thumb: String {
        switch self {
        case .STICKER_NONE:
            return "none"
        case .STICKER_MIKE_GLASSES:
            return "mike_glass"
        case .STICKER_STAR_GLASS:
            return "star_glass"
        case .STICKER_MIKE_NOSE:
            return "mike_nose"
            
        case .STICKER_ANGEL:
            return "angel"
        case .STICKER_BEAR:
            return "bear"
        case .STICKER_CAT:
            return "cat"
        case .STICKER_CAT2:
            return "cat2"
        case .STICKER_CORGI:
            return "corgi"
        case .STICKER_DEVIL:
            return "devil"
        case .STICKER_DOG:
            return "dog"
        }
    }
    
    func idOf(index: Int) -> Int {
        switch index {
        case 0:
            return SWIFT_EFFECT_STICKER.STICKER_NONE.rawValue
        case 1:
            return SWIFT_EFFECT_STICKER.STICKER_MIKE_GLASSES.rawValue
        case 2:
            return SWIFT_EFFECT_STICKER.STICKER_STAR_GLASS.rawValue
        case 3:
            return SWIFT_EFFECT_STICKER.STICKER_MIKE_NOSE.rawValue
        case 4:
            return SWIFT_EFFECT_STICKER.STICKER_ANGEL.rawValue
        case 5:
            return SWIFT_EFFECT_STICKER.STICKER_BEAR.rawValue
        case 6:
            return SWIFT_EFFECT_STICKER.STICKER_CAT.rawValue
        case 7:
            return SWIFT_EFFECT_STICKER.STICKER_CAT2.rawValue
        case 8:
            return SWIFT_EFFECT_STICKER.STICKER_CORGI.rawValue
        case 9:
            return SWIFT_EFFECT_STICKER.STICKER_DEVIL.rawValue
        case 10:
            return SWIFT_EFFECT_STICKER.STICKER_DOG.rawValue
        default:
            return SWIFT_EFFECT_STICKER.STICKER_NONE.rawValue
        }
    }

}
