#include <zephyr/ztest.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <string.h>

#define LED_NODE DT_ALIAS(led0)

#if !DT_NODE_HAS_STATUS(LED_NODE, okay)
#error "Unsupported board: led0 alias is not defined"
#endif

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

static void *led_suite_setup(void)
{
   int ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
   zassert_equal(ret, 0, "Failed to configure LED");
}

static void led_suite_before(void *fixture)
{
    int ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    zassert_equal(ret, 0, "Failed to turn on LED");
}

ZTEST_SUITE(led_suite, NULL, led_suite_setup, led_suite_before, NULL, NULL);

ZTEST(led_suite, test_led_toggle)
{

    int last_state = gpio_pin_get_dt(&led);
        printk("last_state: %d\n", last_state);
    zassert_equal(last_state, 1, "Led should be on before toggling");
    for (int i = 0; i < 5; i++) {
        gpio_pin_toggle_dt(&led);
        k_sleep(K_MSEC(500)); 
        int new_state = gpio_pin_get_dt(&led);

        zassert_not_equal(last_state, new_state,
                          "GPIO pin state did not toggle on iteration %d", i);
        printk("new_state: %d, last_state: %d\n", new_state, last_state);
        last_state = new_state;
    }
}
ZTEST(led_suite, test_led_off)
{

    int state = gpio_pin_set_dt(&led, GPIO_ACTIVE_LOW);
    zassert_equal(state, 0, "LED should be off");
}
ZTEST(led_suite, test_led_on)
{

    int state = gpio_pin_get_dt(&led);
    zassert_equal(state, 1, "LED should be on");            
}

ZTEST(led_suite, test_led_info)
{

    zassert_not_null(led.port->name, "LED port should not be NULL");
    // zassert_true(led.port->name != NULL, "LED port name should not be NULL");
    zassert_true(strcmp(led.port->name, "gpio@50000000") == 0, "Strings should be equal");
    zassert_true(led.pin == 13, "LED pin should be valid");
    zassert_false(led.dt_flags != 0, "LED dt_flags should be equal to zero");
    zassert_true(led.dt_flags == 0, "LED dt_flags should not be zero");
    printk("LED Info: Pin %d, Port %s, Flags %d\n",
          led.pin, led.port->name, led.dt_flags);
} 

/* not functionnal:  just to demonstrate how to use */
// void test_main(void)
// {
// #if defined(CONFIG_UART_ASYNC_API)
//     printk(" ==== UART async API is enabled for this test session ====\n");
// #else
//     printk(" ==== UART async API is not enabled ====\n");
// #endif

// #if defined(CONFIG_I2C)
//     printk(" ==== I2C is enabled for this test session ====\n");
// #else
//     printk(" ==== I2C is not enabled ====\n");
// #endif

// }