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

#ifndef WIFI_UTILS_H
#define WIFI_UTILS_H

#include <WiFi.h>

bool connect_wifi_via_smart_config(
    const IPAddress local_ip, const IPAddress gateway, const IPAddress subnet,
    const int timeout_prev_connection=10000,
    const int timeout_smart_config=30000,
    const int timeout_new_connection=30000);

#endif  // WIFI_UTILS_H
