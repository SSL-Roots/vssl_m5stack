// Copyright 2023 Roots
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <M5Unified.h>

#include "wifi_utils.hpp"

namespace wifi_utils {

bool connect_wifi_via_smart_config(
    const IPAddress local_ip, const IPAddress gateway, const IPAddress subnet,
    const int timeout_prev_connection,
    const int timeout_smart_config,
    const int timeout_new_connection) {
  // Ref: http://lang-ship.com/blog/work/esp32-wi-fi-setting/
  const int DELAY_MS = 1000;

  // Use previous connection info
  WiFi.config(local_ip, gateway, subnet);
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
  }

  M5_LOGI("----- Wi-Fi Connected -----");
  M5_LOGI("----- SSID: %s -----", WiFi.SSID().c_str());

  return true;
}

bool disconnect_wifi(void) {
  // Turn off Wi-Fi radio
  // Does not erase the connection info
  return WiFi.disconnect(true, false);
}

}  // namespace wifi_utils
