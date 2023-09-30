# vssl_m5stack

Roots VSSLロボットのプログラムです。

## Requirements

- Arduino IDE（またはVSCodeのArduino拡張）
- Arduino boards:
  - M5Stack
- Arduino libraries:
  - M5Stack
- Python3
- Python3 libraries:
  - protobuf
  - grpcio-tools

## Installation

### vssl_protocolとnanopbのダウンロード

VSSL通信プロトコルのvssl_protocolと、
Google Protocol Buffersを使うためのnanopbライブラリをダウンロードします

```sh
$ cd vssl_m5stack
$ git submodule update --init --recursive
```

nanopb内のヘッダーファイルとソースファイルをコピーします

```sh
$ cd vssl_m5stack
$ cp nanopb/*.h vssl_main
$ cp nanopb/*.c vssl_main
```

vssl_protocolの.protoファイルをコンパイルします

```sh
# ライブラリをインストール
$ pip3 install protobuf grpcio-tools

# protoファイルをコンパイル
$ cd vssl_m5stack
$ python3 nanopb/generator/nanopb_generator.py -Q quote -L quote -I vssl_protocol/proto vssl_protocol/proto/vssl_robot_control.proto -D vssl_main
```

作業後、vssl_mainディレクトリに下記のファイルが追加されます。

```sh
pb.h         pb_decode.c  pb_encode.h
pb_common.c  pb_decode.h  vssl_robot_control.pb.c
pb_common.h  pb_encode.c  vssl_robot_control.pb.h
```

### Arduinoの設定

1. Arduino環境にM5Stackボード情報を追加します
1. Arduino環境にM5Stackライブラリを追加します

### ビルド

- M5Stack Core向けにビルドする場合は、BoardをM5Stack-Core-ESP32 (esp32)に設定します
- M5StampS3向けにビルドする場合は、BoardをSTAMP-S3 (esp32)に設定します
