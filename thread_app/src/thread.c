#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>

#define STACKSIZE 256

/* priority of each thread */
// #define THREAD0_PRIORITY 4
// #define THREAD1_PRIORITY 5
// #define THREAD2_PRIORITY 6
// #define THREAD3_PRIORITY 7
#define THREAD_PRIORITY 2

#define SLEEP_TIME_MS   1000

/* get devicetree node identifier through alias. */
#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)
#define LED3_NODE DT_ALIAS(led3)
// or  #define LED_NODE DT_NODELABEL(yellow)

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


void thread0(void)
{	
	int ret;
	bool led_state = true;

	if (!gpio_is_ready_dt(&white)) {
		printk("LED WHITE not working ");
	}

	ret = gpio_pin_configure_dt(&white, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		printk("---- LED WHITE configuration failed ---- ");
	}

	while (1) {

          printk("Hello, I am thread0\n");

		  ret = gpio_pin_toggle_dt(&white);

		  if (ret < 0) {

			printk("***** LED WHITE not toggled****");
		}

		led_state = !led_state;
		printk(" 1 - LED WHITE state: %s\n", led_state ? "ON" : "OFF");
		k_msleep(SLEEP_TIME_MS);
	}
}

void thread1(void)
{

	int ret;
	bool led_state = true;

	if (!gpio_is_ready_dt(&green)) {
		
		printk("LED GREEN not working ");
	}

	ret = gpio_pin_configure_dt(&green, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		printk("---- LED GREEN configuration failed ---- ");
	}

	while (1) {
        printk("Hello, I am thread1\n");

 		ret = gpio_pin_toggle_dt(&green);

		  if (ret < 0) {
			printk("***** LED GREEN not toggled****");
		}

		led_state = !led_state;
		printk("2 - LED GREEN state : %s\n", led_state ? "ON" : "OFF");
		k_msleep(SLEEP_TIME_MS*2);
	}
}

void thread2(void)
{
	int ret;
	bool led_state = true;

	if (!gpio_is_ready_dt(&yellow)) {

		printk("LED YELLOW not working ");
	}

	ret = gpio_pin_configure_dt(&yellow, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		printk("---- LED YELLOW configuration failed ---- ");
	}

	while (1) {

          printk("Hello, I am thread2\n");

		  ret = gpio_pin_toggle_dt(&yellow);

		  if (ret < 0) {
			printk("***** LED YELLOW not toggled****");
		}

		led_state = !led_state;
		printk("3 - LED YELLOW state: %s\n", led_state ? "ON" : "OFF");
		k_msleep(SLEEP_TIME_MS*3);
	}
}

void thread3(void)
{
	int ret;
	bool led_state = true;

	if (!gpio_is_ready_dt(&red)) {
		printk(" LED RED not working ");
	}

	ret = gpio_pin_configure_dt(&red, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		printk("---- LED RED configuration failed ---- ");
	}

	while (1) {

          printk("Hello, I am thread3\n");

		  ret = gpio_pin_toggle_dt(&red);

		  if (ret < 0) {
			printk("***** LED RED not toggled****");
		}

		led_state = !led_state;
		printk("4 - LED RED state: %s\n", led_state ? "ON" : "OFF");
		k_msleep(SLEEP_TIME_MS*4);
	}
}

// behavior is the same with different priorities

K_THREAD_DEFINE(thread0_id, STACKSIZE, thread0, NULL, NULL, NULL,
		THREAD_PRIORITY, 0, 0);
K_THREAD_DEFINE(thread1_id, STACKSIZE, thread1, NULL, NULL, NULL,
		THREAD_PRIORITY, 0, 0);
K_THREAD_DEFINE(thread2_id, STACKSIZE, thread2, NULL, NULL, NULL,
		THREAD_PRIORITY, 0, 0);
K_THREAD_DEFINE(thread3_id, STACKSIZE, thread3, NULL, NULL, NULL,
		THREAD_PRIORITY, 0, 0);



