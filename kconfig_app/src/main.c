/*
 * Zephyr OS Sample Application: Kconfig Demonstration
 * Target board: NUCLEO-G071RBv1 and NRF52840DK
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>

/* Configurable LED pin and delay via Kconfig */
#define LED0_NODE DT_ALIAS(led0)
#if !DT_NODE_HAS_STATUS(LED0_NODE, okay)
#error "Unsupported board: led0 devicetree alias is not defined"
#endif

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

/* Delay time is defined in Kconfig */
#define DELAY_TIME_MS CONFIG_BLINK_DELAY

int main(void)
{
    int ret;

    ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        return -1;
    } else {
        printk("LED configured successfully.\n");
        printf("target board used:  %s\n", CONFIG_BOARD_TARGET);
		
    }
#if CONFIG_APP_BLINK_LED
    while (1) {
        gpio_pin_toggle_dt(&led);
        k_msleep(DELAY_TIME_MS);
    }
#else
    printk("LED blinking is disabled\n");
#endif
    return 0;
}
