#include <string.h>
#include <esp_log.h>
#include <nvs_flash.h>
#include <esp_event.h>
#include <esp_wifi.h>
#include <esp_netif.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

static const char *TAG = "WIFI_SCAN";

void wifi_scan_task(void *pvParameters) {
    esp_err_t ret;

    // Inicialización de NVS
    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Inicialización de interfaces de red y eventos
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_netif_create_default_wifi_sta();  // Crea interfaz Wi-Fi STA

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_disconnect());

    ESP_LOGI(TAG, "Iniciando escaneo de redes Wi-Fi...");

    wifi_scan_config_t scan_config = {
        .ssid = 0,
        .bssid = 0,
        .channel = 0,
        .show_hidden = true,
        .scan_type = WIFI_SCAN_TYPE_ACTIVE,
        .scan_time = {
            .active = {
                .min = 100,
                .max = 300
            }
        }
    };

    ESP_ERROR_CHECK(esp_wifi_scan_start(&scan_config, true));
    ESP_LOGI(TAG, "Escaneo completado.");

    uint16_t number = 0;
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&number));

    if (number == 0) {
        ESP_LOGI(TAG, "No se encontraron redes Wi-Fi.");
    } else {
        wifi_ap_record_t *ap_info = (wifi_ap_record_t *)pvPortMalloc(sizeof(wifi_ap_record_t) * number);
        if (ap_info == NULL) {
            ESP_LOGE(TAG, "Error: No se pudo asignar memoria para los registros AP.");
        } else {
            ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&number, ap_info));

            for (int i = 0; i < number; i++) {
                ESP_LOGI(TAG, "%2d: SSID: %s, RSSI: %d dBm, BSSID: %02x:%02x:%02x:%02x:%02x:%02x, Canal: %d, Cifrado: %s",
                         i + 1,
                         (char *)ap_info[i].ssid,
                         ap_info[i].rssi,
                         ap_info[i].bssid[0], ap_info[i].bssid[1], ap_info[i].bssid[2],
                         ap_info[i].bssid[3], ap_info[i].bssid[4], ap_info[i].bssid[5],
                         ap_info[i].primary,
                         (ap_info[i].authmode == WIFI_AUTH_OPEN) ? "Abierta" : "Segura");
            }
            vPortFree(ap_info);
        }
    }

    // Limpieza
    ESP_ERROR_CHECK(esp_wifi_stop());
    ESP_ERROR_CHECK(esp_wifi_deinit());
    ESP_ERROR_CHECK(esp_event_loop_delete_default());
    //ESP_ERROR_CHECK(esp_netif_deinit());
    ESP_ERROR_CHECK(nvs_flash_deinit());

    vTaskDelete(NULL);
}

void app_main(void) {
    xTaskCreate(wifi_scan_task, "wifi_scan_task", 4096 * 2, NULL, 5, NULL);
}
