#include <stdio.h>
#include "blynk_integration.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_rom_gpio.h" // Necesario para esp_rom_gpio_pad_select_gpio

// Define el pin del LED (puedes cambiarlo al que necesites)
#define LED_GPIO 2

static const char *TAG = "BLYNK";

// Simulación de una función que se llamaría cuando se recibe un comando Blynk
void blynk_virtual_write_handler(int pin, int value) {
    ESP_LOGI(TAG, "Pin virtual %d recibido con valor: %d", pin, value);

    if (pin == 0) {
        gpio_set_level(LED_GPIO, value);  // Enciende o apaga el LED
        ESP_LOGI(TAG, "LED %s", value ? "ENCENDIDO" : "APAGADO");
    }
}

// Inicializa el hardware controlado por Blynk
void blynk_start(void) {
    // Selecciona el pin GPIO para uso (función moderna)
    esp_rom_gpio_pad_select_gpio(LED_GPIO);

    // Configura el GPIO como salida
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);

    // Estado inicial del LED (apagado)
    gpio_set_level(LED_GPIO, 0);

    ESP_LOGI(TAG, "Inicialización de Blynk terminada. LED conectado al GPIO %d", LED_GPIO);
}
