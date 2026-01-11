#include "thingy53_led_specs.h"

#include <zephyr/drivers/gpio.h>

#define LED_RED DT_ALIAS(led0)
#define LED_GREEN DT_ALIAS(led1)
#define LED_BLUE DT_ALIAS(led2)

static const struct gpio_dt_spec red = GPIO_DT_SPEC_GET(LED_RED, gpios);
static const struct gpio_dt_spec green = GPIO_DT_SPEC_GET(LED_GREEN, gpios);
static const struct gpio_dt_spec blue = GPIO_DT_SPEC_GET(LED_BLUE, gpios);

const struct gpio_dt_spec *thingy53_led_get_red(void) { return &red; }

const struct gpio_dt_spec *thingy53_led_get_green(void) { return &green; }

const struct gpio_dt_spec *thingy53_led_get_blue(void) { return &blue; }
