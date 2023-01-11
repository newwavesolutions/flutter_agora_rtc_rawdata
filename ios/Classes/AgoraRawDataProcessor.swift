//
//  RawDataProcessor.swift
//  agora_rtc_rawdata
//
//  Created by Lê Thọ Sơn on 23/12/2022.
//

import Foundation

public protocol AgoraRawDataProcessorDelegate {
    func onCapture(_ videoFrame: AgoraVideoFrame) -> Bool
    func didChangeBeauty(type: Int, value: Double)
}

open class AgoraRawDataProcessor {
    public static let shared = AgoraRawDataProcessor()
    
    public var delegate: AgoraRawDataProcessorDelegate? = nil
    
    func processRawData(_ videoFrame: AgoraVideoFrame)  -> Bool {
        return delegate?.onCapture(videoFrame) ?? false
    }
    
    func didChangeBeauty(type: Int, value: Double) {
        delegate?.didChangeBeauty(type: type, value: value)
    }
}
