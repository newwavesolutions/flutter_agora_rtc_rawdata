import Flutter
import UIKit

public class SwiftAgoraRtcRawdataPlugin: NSObject, FlutterPlugin, AgoraAudioFrameDelegate, AgoraVideoFrameDelegate {
    public static func register(with registrar: FlutterPluginRegistrar) {
        let channel = FlutterMethodChannel(name: "agora_rtc_rawdata", binaryMessenger: registrar.messenger())
        let instance = SwiftAgoraRtcRawdataPlugin()
        registrar.addMethodCallDelegate(instance, channel: channel)
    }

    private var audioObserver: AgoraAudioFrameObserver?
    private var videoObserver: AgoraVideoFrameObserver?

    public func handle(_ call: FlutterMethodCall, result: @escaping FlutterResult) {
        switch call.method {
        case "registerAudioFrameObserver":
            if audioObserver == nil {
                audioObserver = AgoraAudioFrameObserver(engineHandle: call.arguments as! UInt)
            }
            audioObserver?.delegate = self
            audioObserver?.register()
            result(nil)
        case "unregisterAudioFrameObserver":
            if audioObserver != nil {
                audioObserver?.delegate = nil
                audioObserver?.unregisterAudioFrameObserver()
                audioObserver = nil
            }
            result(nil)
        case "registerVideoFrameObserver":
            if videoObserver == nil {
                videoObserver = AgoraVideoFrameObserver(engineHandle: call.arguments as! UInt)
            }
            videoObserver?.delegate = self
            videoObserver?.register()
            result(nil)
        case "unregisterVideoFrameObserver":
            if videoObserver != nil {
                videoObserver?.delegate = nil
                videoObserver?.unregisterVideoFrameObserver()
                videoObserver = nil
            }
            result(nil)
        case "setBeautyEffect":
            guard let args = call.arguments as? [String : Any] else {return}
            let value = args["value"] as? Double
            let type = args["beauty_type"] as? Int
            if let value = value, let type = type {
                AgoraRawDataProcessor.shared.didChangeBeauty(type: type, value: value)
            }
            result(nil)
        default:
            result(FlutterMethodNotImplemented)
        }
    }

    public func onRecord(_: AgoraAudioFrame) -> Bool {
        return true
    }

    public func onPlaybackAudioFrame(_: AgoraAudioFrame) -> Bool {
        return true
    }

    public func onMixedAudioFrame(_: AgoraAudioFrame) -> Bool {
        return true
    }

    public func onPlaybackAudioFrame(beforeMixing _: AgoraAudioFrame, uid _: UInt) -> Bool {
        return true
    }

    public func onCapture(_ videoFrame: AgoraVideoFrame) -> Bool {
        return AgoraRawDataProcessor.shared.processRawData(videoFrame)
    }

    public func onRenderVideoFrame(_ videoFrame: AgoraVideoFrame, uid _: UInt) -> Bool {
        return false
    }
}
