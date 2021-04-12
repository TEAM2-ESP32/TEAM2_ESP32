#include "wifi.h"

#include <string.h>

#include "esp_event.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_wifi_default.h"

#include "nvs_flash.h"

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

enum {
	EVENT_CONNECTED_BIT		= BIT0,
	EVENT_DISCONNECTED_BIT		= BIT1,
	EVENT_STA_STARTED_BIT		= BIT2,
	EVENT_STA_STOPPED_BIT		= BIT3,
	EVENT_AP_STARTED_BIT		= BIT4,
	EVENT_AP_STOPPED_BIT		= BIT5,
};

static esp_netif_t *netif_sta;
static esp_event_handler_instance_t event_wifi_any_id;
static esp_event_handler_instance_t event_ip_allocated;
static EventGroupHandle_t event_group;

static void network_interface_init(void)
{
	esp_netif_init();
	netif_sta = esp_netif_create_default_wifi_sta();
}

static int network_set_hostname(const char *hostname)
{
	return esp_netif_set_hostname(netif_sta, hostname);
}

static void event_handler(void *arg, esp_event_base_t event_base,
		int32_t event_id, void *event_data)
{
        wifi_event_sta_disconnected_t *disconnected =
		(wifi_event_sta_disconnected_t *)event_data;
	ip_event_got_ip_t *ip = (ip_event_got_ip_t *)event_data;
	wifi_ap_record_t ap;

	switch (event_id) {
	case WIFI_EVENT_STA_START:
		xEventGroupClearBits(event_group, EVENT_STA_STOPPED_BIT);
		xEventGroupSetBits(event_group, EVENT_STA_STARTED_BIT);
		printf("WiFi station started\r\n");
		break;
	case WIFI_EVENT_STA_STOP:
		xEventGroupClearBits(event_group, EVENT_STA_STARTED_BIT);
		xEventGroupSetBits(event_group, EVENT_STA_STOPPED_BIT);
		printf("WiFi station stopped\r\n");
		break;
	case WIFI_EVENT_STA_CONNECTED:
		esp_wifi_sta_get_ap_info(&ap);
		printf("WiFi station connected(RSSI: %d)\r\n", ap.rssi);
		break;
	case IP_EVENT_STA_GOT_IP:
		xEventGroupClearBits(event_group, EVENT_DISCONNECTED_BIT);
		xEventGroupSetBits(event_group, EVENT_CONNECTED_BIT);
		printf("IP allocated %d.%d.%d.%d\r\n",
				((uint8_t *)&ip->ip_info.ip.addr)[0],
				((uint8_t *)&ip->ip_info.ip.addr)[1],
				((uint8_t *)&ip->ip_info.ip.addr)[2],
				((uint8_t *)&ip->ip_info.ip.addr)[3]);
		break;
	case WIFI_EVENT_STA_DISCONNECTED:
		xEventGroupClearBits(event_group, EVENT_CONNECTED_BIT);
		xEventGroupSetBits(event_group, EVENT_DISCONNECTED_BIT);
		printf("WiFi station disconnected(reason: %d)\r\n", disconnected->reason);
		break;
	case WIFI_EVENT_SCAN_DONE:
		break;
	case WIFI_EVENT_AP_START:
		xEventGroupClearBits(event_group, EVENT_AP_STOPPED_BIT);
		xEventGroupSetBits(event_group, EVENT_AP_STARTED_BIT);
		printf("WiFi AP start\r\n");
		break;
	case WIFI_EVENT_AP_STOP:
		xEventGroupClearBits(event_group, EVENT_AP_STARTED_BIT);
		xEventGroupSetBits(event_group, EVENT_AP_STOPPED_BIT);
		printf("WiFi AP stop\r\n");
		break;
	case WIFI_EVENT_AP_STACONNECTED:
		printf("WiFi AP connected\r\n");
		break;
	case WIFI_EVENT_AP_STADISCONNECTED:
		printf("WiFi AP disconnected\r\n");
		break;
	case WIFI_EVENT_AP_PROBEREQRECVED:
		printf("WiFi AP probe\r\n");
		break;
	default:
		printf("WiFi event %d\r\n", event_id);
		break;
	}
}

void wifi_init(void)
{
	esp_event_loop_create_default();
	nvs_flash_init();

	network_interface_init();
	network_set_hostname("YOURHOST");

	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	esp_wifi_init(&cfg);
	esp_wifi_set_storage(WIFI_STORAGE_RAM);

	if (event_wifi_any_id == NULL) {
		esp_event_handler_instance_register(WIFI_EVENT,
				ESP_EVENT_ANY_ID,
				&event_handler,
				NULL,
				&event_wifi_any_id);
	}
	if (event_ip_allocated == NULL) {
		esp_event_handler_instance_register(IP_EVENT,
				IP_EVENT_STA_GOT_IP,
				&event_handler,
				NULL,
				&event_ip_allocated);
	}

	if (event_group == NULL) {
		event_group = xEventGroupCreate();
	}
}

int wifi_connect(const char *ssid, const char *pass,
		const uint8_t bssid[WIFI_MAC_MAXLEN])
{
	printf("connecting to %s with password \"%s\"\r\n", ssid, pass);

	wifi_config_t conf = { 0, };
	memcpy(conf.sta.ssid, ssid, strlen(ssid) + 1);
	memcpy(conf.sta.password, pass, strlen(pass) + 1);
	if (bssid != NULL) {
		conf.sta.bssid_set = true;
		memcpy(conf.sta.bssid, bssid, sizeof(conf.sta.bssid));
	}

	esp_wifi_set_mode(WIFI_MODE_STA);
	esp_wifi_start();
	xEventGroupWaitBits(event_group, EVENT_STA_STARTED_BIT, pdTRUE,
			pdFALSE, portMAX_DELAY);
	esp_wifi_set_config(ESP_IF_WIFI_STA, &conf);
	xEventGroupClearBits(event_group, xEventGroupGetBits(event_group) &
			(EVENT_CONNECTED_BIT | EVENT_DISCONNECTED_BIT));
	esp_wifi_connect();
	xEventGroupWaitBits(event_group, EVENT_CONNECTED_BIT | EVENT_DISCONNECTED_BIT,
			pdTRUE, pdFALSE, portMAX_DELAY);

	return 0;
}
