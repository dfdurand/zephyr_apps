#include <zephyr/ztest.h>
#include <string.h>

// Define mock types to replicate gpio_dt_spec
struct device_mock {
    const char *name;
    // const void *config;
    // const void *api;
    // void *data;
};

struct gpio_dt_spec {
    struct device_mock *port;
    uint8_t pin;
    uint16_t dt_flags;
};

// Initialize mock device and LED specification
static struct device_mock mock_device = {
    .name = "gpio@50000000"
};

static struct gpio_dt_spec led = {
    .port = &mock_device,
    .pin = 5,
    .dt_flags = 0
};

// Mock LED state
static int mock_led_state = 0;

// Simulate GPIO functions
static int gpio_pin_configure_dt(const struct gpio_dt_spec *spec, int state) {
    mock_led_state = state;
    return 0;
}

static int gpio_pin_get_dt(const struct gpio_dt_spec *spec) {
    return mock_led_state;
}

static int gpio_pin_toggle_dt(const struct gpio_dt_spec *spec) {
    mock_led_state = !mock_led_state;
    return 0;
}

static int gpio_pin_set_dt(const struct gpio_dt_spec *spec, int state) {
    mock_led_state = state;
    return 0;
}

// Test suite setup
static void *led_suite_setup(void) {
    int ret = gpio_pin_configure_dt(&led, 0); // LED off
    zassert_equal(ret, 0, "Failed to configure mock LED");
    return NULL;
}

static void led_suite_before(void *fixture) {
    int ret = gpio_pin_configure_dt(&led, 1); // LED on
    zassert_equal(ret, 0, "Failed to turn on mock LED");
}

ZTEST_SUITE(led_suite, NULL, led_suite_setup, led_suite_before, NULL, NULL);

// Toggle test
ZTEST(led_suite, test_led_toggle) {
    int last_state = gpio_pin_get_dt(&led);
    zassert_equal(last_state, 1, "LED should be on before toggling");

    for (int i = 0; i < 5; i++) {
        gpio_pin_toggle_dt(&led);
        int new_state = gpio_pin_get_dt(&led);

        zassert_not_equal(last_state, new_state,
            "LED did not toggle on iteration %d", i);
        last_state = new_state;
    }
}

// Turn off test
ZTEST(led_suite, test_led_off) {
    int ret = gpio_pin_set_dt(&led, 0);
    zassert_equal(ret, 0, "Failed to turn off mock LED");
    zassert_equal(gpio_pin_get_dt(&led), 0, "LED should be off");
}

// Turn on test
ZTEST(led_suite, test_led_on) {
    int ret = gpio_pin_set_dt(&led, 1);
    zassert_equal(ret, 0, "Failed to turn on mock LED");
    zassert_equal(gpio_pin_get_dt(&led), 1, "LED should be on");
}

// LED info test
ZTEST(led_suite, test_led_info) {
    zassert_not_null(led.port->name, "LED port name should not be NULL");
    zassert_true(strcmp(led.port->name, "gpio@50000000") == 0,
                 "Unexpected port name");

    zassert_equal(led.pin, 5, "LED pin should be 5");
    zassert_equal(led.dt_flags, 0, "LED dt_flags should be zero");

    printk("LED Info: Pin %d, Port %s, Flags %d\n",
           led.pin, led.port->name, led.dt_flags);
}
