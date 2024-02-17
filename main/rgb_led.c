#include "led_strip.h"
#include <stdint.h>
#define BLINK_GPIO GPIO_NUM_8

static led_strip_handle_t led_strip;

void blink_rgb_led(int s_led_state)
{
    if (s_led_state) {
        led_strip_set_pixel(led_strip, 0, 24, 0, 0);
        led_strip_refresh(led_strip);
    } else {
        led_strip_clear(led_strip);
    }
}

void configure_rgb_led()
{
    led_strip_config_t strip_config = {
        .strip_gpio_num = BLINK_GPIO,
        .max_leds = 1
    };
    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000,
        .flags.with_dma = false,
    };
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
    led_strip_clear(led_strip);
}
