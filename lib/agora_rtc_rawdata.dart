import 'dart:async';

import 'package:flutter/services.dart';

class AgoraRtcRawdata {
  static const MethodChannel _channel =
      const MethodChannel('agora_rtc_rawdata');

  static Future<void> registerAudioFrameObserver(int engineHandle) {
    return _channel.invokeMethod('registerAudioFrameObserver', engineHandle);
  }

  static Future<void> unregisterAudioFrameObserver() {
    return _channel.invokeMethod('unregisterAudioFrameObserver');
  }

  static Future<void> registerVideoFrameObserver(int engineHandle) {
    return _channel.invokeMethod('registerVideoFrameObserver', engineHandle);
  }

  static Future<void> unregisterVideoFrameObserver() {
    return _channel.invokeMethod('unregisterVideoFrameObserver');
  }

  static Future<void> setBeautyEffect({BeautyEffect? type, double value = 100}) {
    return _channel.invokeMethod(
      'setBeautyEffect',
      {
        "beauty_type": type?.value,
        "value": value,
      },
    );
  }
}

enum BeautyEffect {
  BigEye(1),
  Lighten(2),
  Rosy(3),
  Scrub(4),
  Soften(5),
  VFace(6);

  const BeautyEffect(this.value);

  final int value;
}
