import 'package:agora_rtc_rawdata/agora_rtc_rawdata.dart';
import 'package:flutter/material.dart';

class BeautyEffectWidget extends StatelessWidget {
  final ValueChanged<BeautyEffect?>? onBeautyEffectChanged;

  BeautyEffectWidget({Key? key, this.onBeautyEffectChanged}) : super(key: key);

  final _beautyEffect = ValueNotifier<BeautyEffect?>(null);

  @override
  Widget build(BuildContext context) {
    return Container(
      color: Colors.blueGrey,
      child: SingleChildScrollView(
        scrollDirection: Axis.horizontal,
        child: ValueListenableBuilder(
          valueListenable: _beautyEffect,
          builder: (context, effect, widget) {
            final list = BeautyEffect.values.map((e) {
              return TextButton(
                child: Text(
                  e.name,
                  style: TextStyle(
                    color:
                        e == _beautyEffect.value ? Colors.green : Colors.black,
                  ),
                ),
                onPressed: () {
                  _beautyEffect.value = e;
                  onBeautyEffectChanged?.call(e);
                },
              );
            }).toList();
            list.insert(
                0,
                TextButton(
                  child: Text("None",
                      style: TextStyle(
                        color: null == _beautyEffect.value
                            ? Colors.green
                            : Colors.black,
                      )),
                  onPressed: () {
                    _beautyEffect.value = null;
                    onBeautyEffectChanged?.call(null);
                  },
                ));
            return Row(
              children: list,
            );
          },
        ),
      ),
    );
  }
}
