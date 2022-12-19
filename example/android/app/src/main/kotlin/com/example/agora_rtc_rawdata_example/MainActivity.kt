package com.example.agora_rtc_rawdata_example

import android.content.Context
import android.os.Bundle
import android.util.Log
import androidx.multidex.MultiDex
import io.flutter.embedding.android.FlutterActivity
import io.flutter.embedding.engine.FlutterEngine
import io.flutter.plugin.common.MethodChannel
import io.flutter.plugins.GeneratedPluginRegistrant
import org.opencv.android.BaseLoaderCallback
import org.opencv.android.InstallCallbackInterface
import vn.nws.liveeffects.EffectWrapper

class MainActivity : FlutterActivity() {
  companion object {
    private val CHANNEL = "flutter.native/helper"
  }

  private var mWrapper: EffectWrapper? = null

  override fun attachBaseContext(newBase: Context?) {
    super.attachBaseContext(newBase)
    MultiDex.install(this);
  }

  override fun onCreate(savedInstanceState: Bundle?) {
    super.onCreate(savedInstanceState)
    GeneratedPluginRegistrant.registerWith(FlutterEngine(this));
    try {
      mWrapper = EffectWrapper(context);
      initWrapper()
    } catch (e: Exception) {
      Log.d("LONGTV", e.message ?: "")
    }
    flutterEngine?.dartExecutor?.binaryMessenger?.let {
      MethodChannel(it, CHANNEL).setMethodCallHandler { call, result ->
        if (call.method == "setBeautyEffect") {
          var baseValue: Double? = (call.arguments as Map<*, *>)["value"] as Double?
          var value: Int? = baseValue?.toInt()
          var type: Int? = (call.arguments as Map<*, *>)["beauty_type"] as Int?
          if (value != null && type != null) {
            // reset beauty
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
        }
      }
    }
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

  private fun initWrapper() {
    try {
      mWrapper!!.InitWrapper(object : BaseLoaderCallback(context) {
        override fun onManagerConnected(status: Int) {
//          mWrapper!!.Filter(mWrapper!!.mWrapper, EffectWrapper.FilterEffect.Cool.value)
//          mWrapper!!.SetBeauty(
//            mWrapper!!.mWrapper,
//            EffectWrapper.BeautyEffect.BigEye.value,
//            100
//          )
//          mWrapper!!.Sticker(mWrapper!!.mWrapper, EffectWrapper.StickerEffect.Cat2.value)
//          mWrapper!!.beautiful = false
        }

        override fun onPackageInstall(operation: Int, callback: InstallCallbackInterface?) {
          super.onPackageInstall(operation, callback)
        }
      })
      mWrapper!!.startProcessing()
    } catch (e: Exception) {
      Log.d("LOMGTV", e.message ?: "")
    }
  }

}
