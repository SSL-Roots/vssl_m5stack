#include <M5Unified.h>

#include "pb_encode.h"
#include "pb_decode.h"
#include "vssl_robot_control.pb.h"

void setup() {
  M5.begin();

  // ログをカラー表示する
  M5.Log.setLogLevel(m5::log_target_serial, ESP_LOG_VERBOSE);
  M5.Log.setEnableColor(m5::log_target_serial, true);

  M5_LOGI("Hello, world!");
}

void loop() {
  M5.delay(1);
  M5.update();

  if (M5.BtnA.wasHold()) {
    M5_LOGI("Button A was Hold!");
    RobotControl control = RobotControl_init_zero;
    control.move_velocity.forward = 0.5;
    control.move_velocity.left = 0.0;
    control.move_velocity.angular = 0.0;
    control.kick_speed = 0.0;
    control.dribbler_speed = 0.0;
    uint8_t buffer[256];
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
    pb_encode(&stream, RobotControl_fields, &control);
  }

  if (M5.BtnA.wasPressed()) {
    M5_LOGI("Button A was pressed!");
  }

  if (M5.BtnA.isHolding()) {
    M5_LOGI("Button A is holding!"); 
  }

}
