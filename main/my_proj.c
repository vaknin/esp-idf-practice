#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>

#define LED_PIN GPIO_NUM_3
#define BUTTON_PIN GPIO_NUM_5

SemaphoreHandle_t xSemaphore = NULL;

static void gpio_isr_handler() {
    if (gpio_get_level(BUTTON_PIN))
        xSemaphoreGiveFromISR(xSemaphore, NULL);
}

static void configure() {

    // Button
    gpio_install_isr_service(0);
    gpio_reset_pin(BUTTON_PIN);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLDOWN_ONLY);
    gpio_set_intr_type(BUTTON_PIN, GPIO_INTR_ANYEDGE);
    gpio_isr_handler_add(BUTTON_PIN, gpio_isr_handler, NULL);

    // LED
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
}


void sleep_seconds(float seconds) {
    vTaskDelay(seconds * 1000 / portTICK_PERIOD_MS);
}

void led_task()
{
    while (1) {
        printf("not semaphored!\n");
        if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
            printf("semaphored!\n");
            gpio_set_level(LED_PIN, 1);
            while (gpio_get_level(BUTTON_PIN)) {
                sleep_seconds(.1f);
            }
            gpio_set_level(LED_PIN, 0);
        }
    }
}

void app_main(void) {
    configure();
    xSemaphore = xSemaphoreCreateBinary();
    xTaskCreate(led_task, "LED", 1024, NULL, 1, NULL);
}
