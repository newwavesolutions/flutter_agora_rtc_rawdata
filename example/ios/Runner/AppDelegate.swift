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
        memset(videoFrame.uBuffer, 0, Int(videoFrame.uStride * videoFrame.height) / 2)
        memset(videoFrame.vBuffer, 0, Int(videoFrame.vStride * videoFrame.height) / 2)
        return true
    }
}
