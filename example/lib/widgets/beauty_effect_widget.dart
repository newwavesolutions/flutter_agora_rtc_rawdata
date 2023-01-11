import 'package:agora_rtc_rawdata/agora_rtc_rawdata.dart';
import 'package:flutter/material.dart';

class BeautyEffectWidget extends StatelessWidget {
  final Function({BeautyEffect type, double value}) onBeautyEffectChanged;

  BeautyEffectWidget({Key? key, required this.onBeautyEffectChanged})
      : super(key: key);

  final _beautyEffect = ValueNotifier<BeautyEffect>(BeautyEffect.None);
  final _beautyValue = ValueNotifier<double>(0);

  @override
  Widget build(BuildContext context) {
    return Container(
      color: Colors.blueGrey,
      child: Column(
        children: [
          SingleChildScrollView(
            scrollDirection: Axis.horizontal,
            child: ValueListenableBuilder(
              valueListenable: _beautyEffect,
              builder: (context, effect, widget) {
                final list = BeautyEffect.values.map((e) {
                  return TextButton(
                    child: Text(
                      e.name,
                      style: TextStyle(
                        color: e == _beautyEffect.value
                            ? Colors.green
                            : Colors.black,
                      ),
                    ),
                    onPressed: () {
                      _beautyEffect.value = e;
                      _beautyValue.value = 50;
                      onBeautyEffectChanged.call(type: e, value: 50);
                    },
                  );
                }).toList();
                // list.insert(
                //     0,
                //     TextButton(
                //       child: Text("None",
                //           style: TextStyle(
                //             color: null == _beautyEffect.value
                //                 ? Colors.green
                //                 : Colors.black,
                //           )),
                //       onPressed: () {
                //         _beautyEffect.value = BeautyEffect.None;
                //         _beautyValue.value = 0;
                //         onBeautyEffectChanged.call(
                //             type: BeautyEffect.None, value: 0);
                //       },
                //     ));
                return Row(
                  children: list,
                );
              },
            ),
          ),
          Container(
            height: 40,
            child: ValueListenableBuilder<double>(
              valueListenable: _beautyValue,
              builder: (context, value, widget) {
                return Slider(
                  value: value / 100,
                  onChanged: (value) {
                    _beautyValue.value = value * 100;
                    onBeautyEffectChanged.call(
                        type: _beautyEffect.value, value: _beautyValue.value);
                  },
                );
              },
            ),
          )
        ],
      ),
    );
  }
}
