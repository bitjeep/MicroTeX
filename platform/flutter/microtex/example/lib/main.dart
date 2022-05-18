import 'package:flutter/material.dart';
import 'package:flutter/rendering.dart';
import 'dart:async';

import 'package:flutter/services.dart';
import 'package:microtex/microtex.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await MicroTeX.instance.initialize(
    clmAsset: 'assets/XITSMath-Regular.clm2',
    textLayoutSerif: 'Noto Serif',
    textLayoutSansSerif: 'Noto Sans',
    textLayoutMonospace: 'Noto Mono',
  );
  debugMicroTeX = true;
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text("MicroTeX example app"),
        ),
        body: Center(
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              Row(
                mainAxisSize: MainAxisSize.min,
                children: const [
                  SizedBox(
                    width: 150,
                    child: Text(
                      'render by text:',
                      style: TextStyle(fontSize: 20),
                    ),
                  ),
                  Text(
                    'Hello World!',
                    style: TextStyle(
                      fontSize: 100,
                      fontFamily: 'XITS Math',
                    ),
                  ),
                ],
              ),
              Row(
                mainAxisSize: MainAxisSize.min,
                children: const [
                  SizedBox(
                    width: 150,
                    child: Text(
                      'render by path:',
                      style: TextStyle(fontSize: 20),
                    ),
                  ),
                  LaTeX(
                    latex: "\\text{Hello World!}",
                    textSize: 100,
                    style: TeXStyle.text,
                    color: Colors.black,
                  ),
                ],
              ),
              const SizedBox(height: 50),
              _buildRichText(),
            ],
          ),
        ),
      ),
    );
  }

  Widget _buildRichText() => RichText(
        text: TextSpan(
          children: [
            const TextSpan(
              text: "Hello!",
              style: TextStyle(
                color: Colors.black,
                fontSize: 50,
                fontFamily: 'XITS Math',
              ),
            ),
            WidgetSpan(
              alignment: PlaceholderAlignment.baseline,
              baseline: TextBaseline.alphabetic,
              child: Container(
                color: Colors.red[100],
                child: const LaTeX(
                  latex: "F(\\omega)=\\int_{-\\infty}^{+\\infty}f(t)e^{-i\\omega_nt}dt",
                  textSize: 50,
                  style: TeXStyle.text,
                  color: Colors.black,
                ),
              ),
            )
          ],
        ),
      );
}
