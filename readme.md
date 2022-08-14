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
2. PlatformIOでビルド・書き込みを行う
3. Linuxのターミナルで以下のコマンドを使って micro-ROS agentを立ち上げる
    `docker run -it --rm -v /dev:/dev -v /dev/shm:/dev/shm --privileged --net=host microros/micro-ros-agent:$ROS_DISTRO serial --dev <device port> -v6`  
    device port は適宜書き換える
4. 別のターミナルで `ros2 topic echo micro_ros_platformio_node_publisher` を実行し、PublishされたメッセージをSubscribeする