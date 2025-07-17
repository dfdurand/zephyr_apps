/*
 * Zephyr OS Sample Application: Custom binding application
 * Target board: NUCLEO-G071RB - nrf52840
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>

// #define LED0_NODE DT_NODELABEL(green_led_0)
#define LED0_NODE DT_ALIAS(led0)
#if !DT_NODE_HAS_STATUS(LED0_NODE, okay)
#error "Unsupported board: led0 devicetree alias is not defined"
#endif

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const int blink_delay = DT_PROP(LED0_NODE, blink_delay_ms);

/* Delay time is defined in dts */
#define DELAY_TIME_MS  blink_delay

int main(void)
{
    int ret;

    ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        printk("Error configuring LED pin: %d\n", ret);
        return -1;
    } else {
        printk("LED configured successfully.\n");
        printk("target board used:  %s\n", CONFIG_BOARD_TARGET);
        printk("Device name: %s\n", led.port->name);
        printk("LED pin: %d, active state: %d\n", led.pin, led.dt_flags & GPIO_ACTIVE_LOW ? 0 : 1);
		
    }

    while (1) {
        gpio_pin_toggle_dt(&led);
        k_msleep(DELAY_TIME_MS);
        printk("LED toggled.\n");
    }

    return 0;
}
