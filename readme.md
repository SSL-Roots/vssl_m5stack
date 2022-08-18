# vssl_m5stack

Roots VSSLロボットのm5stackプログラムです。

## 開発環境のセットアップ方法
### 前提条件
OS: Ubuntu 20.04  or higher

### 手順
1. ROS2 foxyをインストールする
2. VSCodeをインストールする
3. VSCodeにPlatformIO拡張をインストールする
4. VSCodeで「PIO Home」->「Open Project」としてこのリポジトリを開く


## 現状のプログラムの動作確認方法

1. ESP32 BasicをPCに接続する
2. `include/secrets.h` を作成し、以下の内容を書き込む
    ```
    #ifndef __SECRETS_H__
    #define __SECRETS_H__

    #define SSID "<your 2.4GHz SSID>"
    #define PSK "<your SSID Password>"

    #endif
    ```

3. PlatformIOでビルド・書き込みを行う
4. Linuxのターミナルで以下のコマンドを使って micro-ROS agentを立ち上げる
    `docker run -it --rm -v /dev:/dev -v /dev/shm:/dev/shm --privileged --net=host microros/micro-ros-agent:$ROS_DISTRO udp4 --port 8888 -v6`
5. 別のターミナルで `ros2 topic echo micro_ros_platformio_node_publisher` を実行し、PublishされたメッセージをSubscribeする