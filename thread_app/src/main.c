#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

#define STACKSIZE 256


#define SLEEP_TIME_MS   1000

/* get devicetree node identifier through alias. */
#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)
#define LED3_NODE DT_ALIAS(led3)
// or alternatively #define LEDx_NODE DT_NODELABEL(color)

#if !DT_NODE_HAS_STATUS_OKAY(LED0_NODE) || \
	!DT_NODE_HAS_STATUS_OKAY(LED1_NODE) || \
	!DT_NODE_HAS_STATUS_OKAY(LED2_NODE) || \
	!DT_NODE_HAS_STATUS_OKAY(LED3_NODE)
#error "Unsupported board: led devicetree alias is not defined"
#endif

static const struct gpio_dt_spec white = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec green = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
static const struct gpio_dt_spec yellow = GPIO_DT_SPEC_GET(LED2_NODE, gpios);
static const struct gpio_dt_spec red = GPIO_DT_SPEC_GET(LED3_NODE, gpios);


int main(void)
{
	int ret;
	bool led_state = true;

	/* configure GPIO pins for LEDs */

	if (!gpio_is_ready_dt(&yellow)) {

		printk("LED YELLOW not working ");

		return 0;
	}

	if (!gpio_is_ready_dt(&green)) {

		printk("LED GREEN not working ");

		return 0;
	}

	if (!gpio_is_ready_dt(&red)) {

		printk("LED RED not working ");

		return 0;
	}

	if (!gpio_is_ready_dt(&white)) {
		printk("LED WHITE not working ");

		return 0;
	}

	/* Set GPIO pins as outputs */

	ret = gpio_pin_configure_dt(&yellow, GPIO_OUTPUT_ACTIVE); // GPIO_OUTPUT_INACTIVE
	if (ret < 0) {

		printk("---- LED YELLOW configuration failed ---- ");
		return 0;
	}

	ret = gpio_pin_configure_dt(&green, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		printk("---- LED GREEN configuration failed ---- ");

		return 0;
	}

	ret = gpio_pin_configure_dt(&red, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		printk("---- LED RED configuration failed ---- ");
		return 0;
	}

	ret = gpio_pin_configure_dt(&white, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {

		printk("---- LED WHITE configuration failed ---- ");

		return 0;
	}


	while (1) {
		ret = gpio_pin_toggle_dt(&yellow);
		if (ret < 0) {

			printk("***** LED YELLOW not toggled****");
			return 0;
		}
		k_msleep(SLEEP_TIME_MS); // blink every 1 second

		ret = gpio_pin_toggle_dt(&green);
		if (ret < 0) {

			printk("***** LED GREEN not toggled****");
			return 0;
		}
		k_msleep(2 * SLEEP_TIME_MS); // blink every 2 seconds

		ret = gpio_pin_toggle_dt(&red);
		if (ret < 0) {

			printk("***** LED RED not toggled****");
			return 0;
		}
		k_msleep(3 * SLEEP_TIME_MS); // blink every 3 seconds

		ret = gpio_pin_toggle_dt(&white);
		if (ret < 0) {

			printk("***** LED WHITE not toggled****");
			return 0;
		}

		led_state = !led_state;
		printk("LED state: %s\n", led_state ? "ON" : "OFF");
		k_msleep(4 * SLEEP_TIME_MS); // blink every 4 seconds
	}

	return 0;
}