#ifndef MYWIFI_H
#define MYWIFI_H

#include <stdint.h>

#define WIFI_MAC_MAXLEN				6

void wifi_init(void);
int wifi_connect(const char *ssid, const char *pass,
		const uint8_t bssid[WIFI_MAC_MAXLEN]);

#endif
