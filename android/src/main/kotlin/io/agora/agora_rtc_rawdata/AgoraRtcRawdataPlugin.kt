package io.agora.agora_rtc_rawdata

import android.app.Activity
import android.content.Context
import android.graphics.Bitmap
import android.graphics.Matrix
import androidx.annotation.NonNull
import io.agora.rtc.rawdata.base.AudioFrame
import io.agora.rtc.rawdata.base.IAudioFrameObserver
import io.agora.rtc.rawdata.base.IVideoFrameObserver
import io.agora.rtc.rawdata.base.VideoFrame
import io.flutter.embedding.engine.plugins.FlutterPlugin
import io.flutter.plugin.common.MethodCall
import io.flutter.plugin.common.MethodChannel
import io.flutter.plugin.common.MethodChannel.MethodCallHandler
import io.flutter.plugin.common.MethodChannel.Result
import android.util.Log
import java.util.*
import io.github.crow_misia.libyuv.I420Buffer
import org.opencv.android.OpenCVLoader
import org.opencv.android.Utils
import org.opencv.core.Mat
import org.opencv.imgproc.Imgproc
import vn.nws.liveeffects.EffectWrapper
import java.nio.ByteBuffer
import io.flutter.embedding.engine.plugins.activity.ActivityAware
import it.thoson.flutter_agora_demo.YUVUtils
import org.opencv.android.BaseLoaderCallback
import org.opencv.android.InstallCallbackInterface

/** AgoraRtcRawdataPlugin */
class AgoraRtcRawdataPlugin : FlutterPlugin, MethodCallHandler {
  /// The MethodChannel that will the communication between Flutter and native Android
  ///
  /// This local reference serves to register the plugin with the Flutter Engine and unregister it
  /// when the Flutter Engine is detached from the Activity
  private lateinit var channel: MethodChannel

  private var audioObserver: IAudioFrameObserver? = null
  private var videoObserver: IVideoFrameObserver? = null

  private lateinit var context: Context
  private lateinit var activity: Activity

  private var beautyEffectIndex: Int = 1

  var originBitmap: Bitmap? = null
  var originI420: I420Buffer? = null
  var newI420: ByteArray? = null
  var newBitmap: Bitmap? = null

  var mWrapper: EffectWrapper? = null

  override fun onAttachedToEngine(@NonNull flutterPluginBinding: FlutterPlugin.FlutterPluginBinding) {
    channel = MethodChannel(flutterPluginBinding.binaryMessenger, "agora_rtc_rawdata")
    channel.setMethodCallHandler(this)
    context = flutterPluginBinding.applicationContext
    OpenCVLoader.initDebug()
    if (mWrapper == null) {
      Log.d("SonLT1", "init fail")
    } else {
      Log.d("SonLT1", "init success")
    }
    mWrapper = EffectWrapper(context)
    initView()
    if (mWrapper == null) {
      Log.d("SonLT2", "init fail")
    } else {
      Log.d("SonLT2", "init success")
    }
//    mWrapper?.SetBeauty(
//      mWrapper!!.mWrapper,
//      2,
//      1
//    )
    if (mWrapper == null) {
      Log.d("SonLT", "init fail")
    } else {
      Log.d("SonLT", "init success")
    }
  }

  private fun initView() {
    mWrapper?.InitWrapper(object : BaseLoaderCallback(context) {
      override fun onManagerConnected(status: Int) {
        // Todo: change effect here
//                wrapper.startProcessing()
//                wrapper.Sticker(wrapper.mWrapper, EffectWrapper.StickerEffect.Angel.value)
//                wrapper.Quality(wrapper.mWrapper, 10)
//                wrapper.Beauty(wrapper.mWrapper, EffectWrapper.BeautyEffect.BigEye.value)
//        mWrapper!!.Filter(mWrapper!!.mWrapper, EffectWrapper.FilterEffect.Cool.value)
        mWrapper!!.SetBeauty(mWrapper!!.mWrapper, EffectWrapper.BeautyEffect.BigEye.value, 100)
        mWrapper!!.Sticker(mWrapper!!.mWrapper, EffectWrapper.StickerEffect.Cat.value)
        mWrapper!!.beautiful = false
//                Handler().postDelayed({
//                    if (sGoCoderSDK != null) {
//                        // Create a broadcaster instance
//                        mWZBroadcast = WOWZBroadcast()
//                        mWZBroadcast?.setLogLevel(WOWZLog.LOG_LEVEL_DEBUG)
//
//                        configWowzBroadcast()
//                        val configError = startBroadcast()
//                        if (configError != null) {
//                            Toast.makeText(context, "Start LiveStream Fail", Toast.LENGTH_SHORT).show()
//                        }
//                    }
//                }, 5000)
      }

      override fun onPackageInstall(operation: Int, callback: InstallCallbackInterface?) {
        super.onPackageInstall(operation, callback)
      }
    })
//    mWrapper?.Filter(
//      mWrapper!!.mWrapper,
//      EffectWrapper.FilterEffect.Cool.value
//    )
//
//    mWrapper?.Filter(
//      mWrapper!!.mWrapper,
//      EffectWrapper.FilterEffect.Cool.value
//    )
    mWrapper?.startProcessing()
  }

  override fun onMethodCall(@NonNull call: MethodCall, @NonNull result: Result) {
    when (call.method) {
      "registerAudioFrameObserver" -> {
        if (audioObserver == null) {
          audioObserver = object : IAudioFrameObserver((call.arguments as Number).toLong()) {
            override fun onRecordAudioFrame(audioFrame: AudioFrame): Boolean {
              return true
            }

            override fun onPlaybackAudioFrame(audioFrame: AudioFrame): Boolean {
              return true
            }

            override fun onMixedAudioFrame(audioFrame: AudioFrame): Boolean {
              return true
            }

            override fun onPlaybackAudioFrameBeforeMixing(
              uid: Int,
              audioFrame: AudioFrame
            ): Boolean {
              return true
            }
          }
        }
        audioObserver?.registerAudioFrameObserver()
        result.success(null)
      }
      "unregisterAudioFrameObserver" -> {
        audioObserver?.let {
          it.unregisterAudioFrameObserver()
          audioObserver = null
        }
        result.success(null)
      }
      "registerVideoFrameObserver" -> {
        if (videoObserver == null) {
          videoObserver = object : IVideoFrameObserver((call.arguments as Number).toLong()) {
            override fun onCaptureVideoFrame(videoFrame: VideoFrame): Boolean {
//              Arrays.fill(videoFrame.getuBuffer(), 0)
//              Arrays.fill(videoFrame.getvBuffer(), 0)

              //Step 1: Convert i420 to Bitmap
              val i420: ByteArray = YUVUtils.toWrappedI420(
                ByteBuffer.wrap(videoFrame.getyBuffer()),
                ByteBuffer.wrap(videoFrame.getuBuffer()),
                ByteBuffer.wrap(videoFrame.getvBuffer()),
                videoFrame.width,
                videoFrame.height
              )
              originBitmap = YUVUtils.NV21ToBitmap(
                context,
                YUVUtils.I420ToNV21(i420, videoFrame.width, videoFrame.height),
                videoFrame.width,
                videoFrame.height
              )

              // Oriented image
              val matrix = Matrix()
              matrix.setRotate(270f)
              // 围绕原地进行旋转
              // 围绕原地进行旋转
              originBitmap = Bitmap.createBitmap(originBitmap!!, 0, 0, videoFrame.width, videoFrame.height, matrix, false)

              //Step 2: Process data
              val originMat = Mat()
//              val newMat = Mat()
              Utils.bitmapToMat(originBitmap, originMat)

//              Imgproc.cvtColor(originMat, newMat, Imgproc.COLOR_RGB2GRAY);
              mWrapper?.Apply(mWrapper!!.mWrapper, originMat.nativeObjAddr)

              newBitmap =
                Bitmap.createBitmap(originMat.cols(), originMat.rows(), Bitmap.Config.ARGB_8888)
              Utils.matToBitmap(originMat, newBitmap!!)


              val matrix2 = Matrix()
              matrix2.setRotate(-270f)
              // 围绕原地进行旋转
              // 围绕原地进行旋转
              newBitmap = Bitmap.createBitmap(newBitmap!!, 0, 0, videoFrame.height, videoFrame.width, matrix2, false)

              //Step 3: Convert Bitmap to i420 buffer
              newI420 = YUVUtils.bitmapToI420(videoFrame.width, videoFrame.height, newBitmap!!)
//              var yBuffer = ByteArray(newI420!!.bufferY.capacity())
//              var uBuffer = ByteArray(newI420!!.bufferU.capacity())
//              var vBuffer = ByteArray(newI420!!.bufferV.capacity())
              for (i in videoFrame.getyBuffer().indices) {
                videoFrame.getyBuffer()[i] = newI420!![i]
              }
              for (i in videoFrame.getuBuffer().indices) {
                videoFrame.getuBuffer()[i] = newI420!![videoFrame.getyBuffer().size + i]
              }
              for (i in videoFrame.getvBuffer().indices) {
                videoFrame.getvBuffer()[i] =
                  newI420!![videoFrame.getyBuffer().size + videoFrame.getuBuffer().size + i]
              }
              return true
            }

            override fun onRenderVideoFrame(uid: Int, videoFrame: VideoFrame): Boolean {
              // unsigned char value 255
              Arrays.fill(videoFrame.getuBuffer(), -1)
              Arrays.fill(videoFrame.getvBuffer(), -1)
              return true
            }
          }
        }
        videoObserver?.registerVideoFrameObserver()
        result.success(null)
      }
      "unregisterVideoFrameObserver" -> {
        videoObserver?.let {
          it.unregisterVideoFrameObserver()
          videoObserver = null
        }
        result.success(null)
      }
      "setBeautyEffect" -> {
        Log.d("setBeautyEffect", call.arguments.toString())
      }
      else -> result.notImplemented()
    }
  }

  override fun onDetachedFromEngine(@NonNull binding: FlutterPlugin.FlutterPluginBinding) {
    channel.setMethodCallHandler(null)
  }

  companion object {
    // Used to load the 'native-lib' library on application startup.
    init {
      System.loadLibrary("cpp")
    }
  }
}
