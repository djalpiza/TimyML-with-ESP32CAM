#ifndef __CONNECT_WLAN_H
#define __CONNECT_WLAN_H
#include <string>

void Init_NVS();
void wifi_init_sta(const char *_ssid, const char *_password);
std::string GetIPAdresse();

#endif