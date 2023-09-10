#include <M5Unified.h>

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
  }

  if (M5.BtnA.wasPressed()) {
    M5_LOGI("Button A was pressed!");
  }

  if (M5.BtnA.isHolding()) {
    M5_LOGI("Button A is holding!"); 
  }

}
