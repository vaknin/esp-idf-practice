#include "portmacro.h"
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <stdio.h>
#include <esp_log.h>

#define LED_PIN GPIO_NUM_3
#define BUTTON_PIN GPIO_NUM_5

SemaphoreHandle_t xMutex = NULL;
int shared_counter = 0;
float delay = 0.01;
static char TAG[] = "Log:";

void sleep_seconds(float seconds) {
    vTaskDelay(seconds * 1000 / portTICK_PERIOD_MS);
}

void signal_taker() {
    while (1) {
        xSemaphoreTake(xMutex, portMAX_DELAY);
        shared_counter++;
        xSemaphoreGive(xMutex);
        ESP_LOGI(TAG, "Incremented to {%d}", shared_counter);
        sleep_seconds(delay);
    }
}

void signal_giver() {
    while (1) {
        xSemaphoreTake(xMutex, portMAX_DELAY);
        shared_counter--;
        xSemaphoreGive(xMutex);
        ESP_LOGI(TAG, "Decremented to {%d}", shared_counter);
        sleep_seconds(delay);
    }
}

void app_main(void) {
    xMutex = xSemaphoreCreateMutex();
    xTaskCreate(signal_giver, "giver", 4096, NULL, 1, NULL);
    xTaskCreate(signal_taker, "taker", 4096, NULL, 1, NULL);
}
