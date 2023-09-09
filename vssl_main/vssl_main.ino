#include <M5Unified.h>
#include "include/hoge.h"

void setup() {
  M5.begin();

  // ログをカラー表示する
  M5.Log.setLogLevel(m5::log_target_serial, ESP_LOG_VERBOSE);
  M5.Log.setEnableColor(m5::log_target_serial, true);

  M5_LOGI("Start! %d", get_number_five());
}

void loop() {
  M5.delay(1);
  M5.update();

  if (M5.BtnA.wasPressed()) {
    M5_LOGI("Button A was pressed!");
  }
}
