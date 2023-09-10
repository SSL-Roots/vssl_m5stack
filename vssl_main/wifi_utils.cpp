
#include <M5Unified.h>
#include <WiFi.h>

#include "wifi_utils.h"

bool connect_wifi_via_smart_config(
    const int timeout_prev_connection,
    const int timeout_smart_config,
    const int timeout_new_connection) {
  // Ref: http://lang-ship.com/blog/work/esp32-wi-fi-setting/
  const int DELAY_MS = 1000;

  // Use previous connection info
  WiFi.begin();
  while (WiFi.status() != WL_CONNECTED) {
    M5_LOGI("Try to use a previous Wi-Fi connection.");
    delay(DELAY_MS);
    if ( timeout_prev_connection < millis() ) {
      break;
    }
  }

  // If not connected, wait for SmartConfig
  if ( WiFi.status() != WL_CONNECTED ) {
    WiFi.mode(WIFI_STA);
    WiFi.beginSmartConfig();
    while (!WiFi.smartConfigDone()) {
      delay(DELAY_MS);
      M5_LOGI("Waiting for SmartConfig");
      if ( timeout_smart_config < millis() ) {
        M5_LOGW("Failed to detect SmartConfig packet.");
        return false;
      }
    }

    // Try to connect to new Wi-fi
    while (WiFi.status() != WL_CONNECTED) {
      delay(DELAY_MS);
      M5_LOGI("Waiting for Wi-fi");
      // 60秒以上接続できなかったら抜ける
      if ( timeout_new_connection < millis() ) {
        M5_LOGW("Failed to connect Wi-Fi. SSID: %s", WiFi.SSID().c_str());
        return false;
      }
    }
    M5_LOGI("----- Wi-Fi Connected -----");
    M5_LOGI("----- SSID: %s -----", WiFi.SSID().c_str());
  }

  return true;
}