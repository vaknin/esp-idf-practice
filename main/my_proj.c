#include "portmacro.h"
#include "./rgb_led.c"
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <stdio.h>
#include "esp_log.h"

#define LED_PIN GPIO_NUM_3
#define BUTTON_PIN GPIO_NUM_5

static const char* TAG = "TASKS";

void sleep_seconds(float seconds) {
    vTaskDelay(seconds * 1000 / portTICK_PERIOD_MS);
}

void task_2()
{
    while (1) {
        ESP_LOGI(TAG, "hello from task 2!");
        sleep_seconds(3.0f);
    }
}
void task_1()
{
    while (1) {
        ESP_LOGI(TAG, "hello from task 1!");
        sleep_seconds(1.5f);

        // The following three lines are great for
        // checking the needed stack size:

        // int stack_left = uxTaskGetStackHighWaterMark(NULL);
        // int stack_used = (int)stack_size - stack_left;
        // ESP_LOGI(TAG, "%d, %d, %d", (int)stack_size, stack_left, stack_used);
    }
}

void app_main(void) {
    TaskHandle_t task_1_handle = NULL;
    xTaskCreate(task_1, "task1", 4096, NULL, 1, &task_1_handle);
    xTaskCreate(task_2, "task2", 4096, NULL, 1, NULL);
    vTaskDelete(task_1_handle);
}
