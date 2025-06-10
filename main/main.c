#include <esp_log.h>
#include <nvs_flash.h>
#include <esp_event.h>
#include <esp_wifi.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

static const char *TAG = "WIFI_SCAN_NO_GOTO"; // Nueva etiqueta para los logs

void wifi_scan_task(void *pvParameters) {
    esp_err_t ret; // Declaramos la variable 'ret' para los códigos de error

    // Inicializar NVS (Non-Volatile Storage) - Necesario para Wi-Fi
    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Inicializar la pila de red TCP/IP
    ESP_ERROR_CHECK(esp_netif_init());

    // Crear el bucle de eventos por defecto (para eventos Wi-Fi)
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Inicializar el módulo Wi-Fi con configuración por defecto
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // Configurar el modo Wi-Fi a Estación (Station)
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    // Desconectar (si estaba conectado) y escanear
    ESP_ERROR_CHECK(esp_wifi_disconnect());
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "Iniciando escaneo de redes Wi-Fi...");

    uint16_t number = 0;
    wifi_ap_record_t *ap_info = NULL;

    wifi_scan_config_t scan_config = {
        .ssid = 0,
        .bssid = 0,
        .channel = 0,
        .show_hidden = true
    };

    ESP_ERROR_CHECK(esp_wifi_scan_start(&scan_config, true));
    ESP_LOGI(TAG, "Escaneo completado.");

    // Primero, obtenemos solo el número de APs encontrados.
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&number, NULL));

    if (number == 0) {
        ESP_LOGI(TAG, "No se encontraron redes Wi-Fi.");
    } else {
        ESP_LOGI(TAG, "Se encontraron %d redes.", number);

        // Asignar memoria dinámicamente para almacenar los registros de los APs
        ap_info = (wifi_ap_record_t *)pvPortMalloc(sizeof(wifi_ap_record_t) * number);

        if (ap_info == NULL) {
            ESP_LOGE(TAG, "Error: No se pudo asignar memoria para los registros AP. Continuando con la limpieza.");
            // No hay 'goto', simplemente el flujo seguirá a la limpieza.
        } else {
            // Si la asignación fue exitosa, obtener la información completa y mostrarla
            ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&number, ap_info));

            for (int i = 0; i < number; i++) {
                ESP_LOGI(TAG, "%2d: SSID: %s, RSSI: %d dBm, BSSID: %02x:%02x:%02x:%02x:%02x:%02x, Canal: %d, Cifrado: %s",
                         i + 1,
                         ap_info[i].ssid,
                         ap_info[i].rssi,
                         ap_info[i].bssid[0], ap_info[i].bssid[1], ap_info[i].bssid[2],
                         ap_info[i].bssid[3], ap_info[i].bssid[4], ap_info[i].bssid[5],
                         ap_info[i].primary,
                         (ap_info[i].authmode == WIFI_AUTH_OPEN) ? "Abierta" : "Segura"
                        );
            }
        }
    }

    // Sección de limpieza. Se ejecutará siempre, haya habido un error de malloc o no.
    if (ap_info != NULL) {
        vPortFree(ap_info);
        ESP_LOGI(TAG, "Memoria de registros AP liberada.");
    }

    // Estas operaciones de limpieza son críticas y deben ocurrir independientemente de los resultados del escaneo/malloc
    ESP_ERROR_CHECK(esp_wifi_stop());
    ESP_ERROR_CHECK(esp_wifi_deinit());
    ESP_ERROR_CHECK(esp_event_loop_delete_default());
    esp_netif_deinit();
    nvs_flash_deinit();

    vTaskDelete(NULL);
}

void app_main(void) {
    xTaskCreate(&wifi_scan_task, "wifi_scan_task", 4096, NULL, 5, NULL);
}