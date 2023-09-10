
#ifndef WIFI_UTILS_H
#define WIFI_UTILS_H

bool connect_wifi_via_smart_config(
    const int timeout_prev_connection=10000,
    const int timeout_smart_config=30000,
    const int timeout_new_connection=30000);

#endif  // WIFI_UTILS_H
