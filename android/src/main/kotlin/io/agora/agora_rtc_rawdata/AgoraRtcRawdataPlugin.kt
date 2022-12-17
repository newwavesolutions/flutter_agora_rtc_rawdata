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
import org.opencv.android.OpenCVLoader
import org.opencv.android.Utils
import org.opencv.core.Mat
import vn.nws.liveeffects.EffectWrapper
import java.nio.ByteBuffer
import it.thoson.flutter_agora_demo.YUVUtils
import org.opencv.android.BaseLoaderCallback
import org.opencv.android.InstallCallbackInterface
import org.opencv.imgproc.Imgproc

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

  var originBitmap: Bitmap? = null
  var newI420: ByteArray? = null
  var mWrapper: EffectWrapper? = null

  override fun onAttachedToEngine(@NonNull flutterPluginBinding: FlutterPlugin.FlutterPluginBinding) {
    channel = MethodChannel(flutterPluginBinding.binaryMessenger, "agora_rtc_rawdata")
    channel.setMethodCallHandler(this)
    context = flutterPluginBinding.applicationContext
//    OpenCVLoader.initDebug()
    mWrapper = EffectWrapper(context)
    initView()
  }

  private fun initView() {
    mWrapper?.InitWrapper(object : BaseLoaderCallback(context) {
      override fun onManagerConnected(status: Int) {
//        mWrapper!!.Filter(mWrapper!!.mWrapper, EffectWrapper.FilterEffect.Cool.value)
//        mWrapper!!.SetBeauty(mWrapper!!.mWrapper, EffectWrapper.BeautyEffect.BigEye.value, 100)
//        mWrapper!!.Sticker(mWrapper!!.mWrapper, EffectWrapper.StickerEffect.Cat.value)
//        mWrapper!!.beautiful = false
      }

      override fun onPackageInstall(operation: Int, callback: InstallCallbackInterface?) {
        super.onPackageInstall(operation, callback)
      }
    })
    mWrapper?.startProcessing()
  }

  private fun removeBeauty(){
    mWrapper!!.RemoveBeauty(
      mWrapper!!.mWrapper,
      EffectWrapper.BeautyEffect.BigEye.value,
    )

    mWrapper!!.RemoveBeauty(
      mWrapper!!.mWrapper,
      EffectWrapper.BeautyEffect.Lighten.value,
    )

    mWrapper!!.RemoveBeauty(
      mWrapper!!.mWrapper,
      EffectWrapper.BeautyEffect.Rosy.value,
    )

    mWrapper!!.RemoveBeauty(
      mWrapper!!.mWrapper,
      EffectWrapper.BeautyEffect.Scrub.value,
    )

    mWrapper!!.RemoveBeauty(
      mWrapper!!.mWrapper,
      EffectWrapper.BeautyEffect.Soften.value,
    )

    mWrapper!!.RemoveBeauty(
      mWrapper!!.mWrapper,
      EffectWrapper.BeautyEffect.VFace.value,
    )
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
              val rotateMatrix = Matrix()
              rotateMatrix.setRotate(270f)
              originBitmap = Bitmap.createBitmap(
                originBitmap!!,
                0,
                0,
                videoFrame.width,
                videoFrame.height,
                rotateMatrix,
                false
              )

              //Step 2: Process data
              val originMat = Mat()
              Utils.bitmapToMat(originBitmap, originMat)
              val matRGB = Mat()
              Imgproc.cvtColor(originMat, matRGB, Imgproc.COLOR_BGRA2BGR)
              mWrapper?.Apply(mWrapper!!.mWrapper, matRGB.nativeObjAddr)
              Utils.matToBitmap(matRGB, originBitmap)

              val revertMatrix = Matrix()
              revertMatrix.setRotate(-270f)
              originBitmap = Bitmap.createBitmap(
                originBitmap!!,
                0,
                0,
                videoFrame.height,
                videoFrame.width,
                revertMatrix,
                false
              )

              //Step 3: Convert Bitmap to i420 buffer
              newI420 = YUVUtils.bitmapToI420(videoFrame.width, videoFrame.height, originBitmap!!)
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
//              Arrays.fill(videoFrame.getuBuffer(), -1)
//              Arrays.fill(videoFrame.getvBuffer(), -1)
              return false
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
        var baseValue: Double? = (call.arguments as Map<*, *>)["value"] as Double?
        var value: Int? = baseValue?.toInt()
        var type: Int? = (call.arguments as Map<*, *>)["beauty_type"] as Int?
        Log.d("setBeautyEffect", "{beauty_type=$type, value=$value}")

        if (value == null || type == null) {
          return
        }
        removeBeauty()
        when (type) {
          1 -> mWrapper!!.SetBeauty(
            mWrapper!!.mWrapper,
            EffectWrapper.BeautyEffect.BigEye.value,
            value
          )
          2 -> mWrapper!!.SetBeauty(
            mWrapper!!.mWrapper,
            EffectWrapper.BeautyEffect.Lighten.value,
            value
          )
          3 -> mWrapper!!.SetBeauty(
            mWrapper!!.mWrapper,
            EffectWrapper.BeautyEffect.Rosy.value,
            value
          )
          4 -> mWrapper!!.SetBeauty(
            mWrapper!!.mWrapper,
            EffectWrapper.BeautyEffect.Scrub.value,
            value
          )
          5 -> mWrapper!!.SetBeauty(
            mWrapper!!.mWrapper,
            EffectWrapper.BeautyEffect.Soften.value,
            value
          )
          6 -> mWrapper!!.SetBeauty(
            mWrapper!!.mWrapper,
            EffectWrapper.BeautyEffect.VFace.value,
            value
          )
        }

        result.success("")
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
