package com.example.agora_rtc_rawdata_example

import android.content.Context
import android.os.Bundle
import android.util.Log
import androidx.multidex.MultiDex
import io.flutter.embedding.android.FlutterActivity
import io.flutter.embedding.engine.FlutterEngine
import io.flutter.plugins.GeneratedPluginRegistrant
import org.opencv.android.BaseLoaderCallback
import org.opencv.android.InstallCallbackInterface
import vn.nws.liveeffects.EffectWrapper

class MainActivity: FlutterActivity() {
  private var wrapper: EffectWrapper? = null

  override fun attachBaseContext(newBase: Context?) {
    super.attachBaseContext(newBase)
    MultiDex.install(this);
  }

  override fun onCreate(savedInstanceState: Bundle?) {
    super.onCreate(savedInstanceState)
    GeneratedPluginRegistrant.registerWith(FlutterEngine(this));
    try{
      wrapper = EffectWrapper(context);
      initWrapper()
    }catch (e:Exception){
      Log.d("LONGTV","")
    }
  }

  private fun initWrapper() {
    try {
      wrapper!!.InitWrapper(object : BaseLoaderCallback(context) {
        override fun onManagerConnected(status: Int) {
          wrapper!!.Filter(wrapper!!.mWrapper, EffectWrapper.FilterEffect.Cool.value)
          wrapper!!.SetBeauty(
            wrapper!!.mWrapper,
            EffectWrapper.BeautyEffect.BigEye.value,
            100
          )
          wrapper!!.Sticker(wrapper!!.mWrapper, EffectWrapper.StickerEffect.Cat2.value)
          wrapper!!.beautiful = false
        }

        override fun onPackageInstall(operation: Int, callback: InstallCallbackInterface?) {
          super.onPackageInstall(operation, callback)
        }
      })
      wrapper!!.startProcessing()
    } catch (e: Exception) {
      Log.d("LOMGTV", e.message ?: "")
    }
  }

}
