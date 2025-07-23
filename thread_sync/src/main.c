/* ========== 1 THREAD SYNCHRONIZATION AT RUNTIME ============= */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#define LED0_NODE DT_ALIAS(led0)
#define STACK_SIZE 512
#define PRIORITY 5


static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

K_THREAD_STACK_DEFINE(blink_stack, STACK_SIZE);
K_THREAD_STACK_DEFINE(blink_stack2, STACK_SIZE);

// Declare thread data structs
static struct k_thread blink_thread, blink_thread2;

// Blink thread entry point
void blink_thread_start(void *arg_1, void *arg_2, void *arg_3)
{
    const char *name = (const char *)arg_1;
    while (1) {
        gpio_pin_toggle_dt(&led);
        printk("%s is toggling the LED\n", name);
        k_msleep(1000); // Blink every 1000 ms
    }
}

int main(void)
{
    int ret;
    k_tid_t tid_blink, tid_blink2;

    if (!gpio_is_ready_dt(&led)) {
        printk(" GPIO pin not ready\r\n");
        return 0;
    }

    ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT);
    if (ret < 0) {
        printk("Could not configure GPIO\r\n");
        return 0;
    }

    // Start first blink thread
    tid_blink = k_thread_create(&blink_thread,          // Thread struct
                                blink_stack,            // Stack size
                                K_THREAD_STACK_SIZEOF(blink_stack),
                                blink_thread_start,     // Thread entry point
                                (void *)"Thread A",     // p1
                                NULL,                   // p2
                                NULL,                   // p3
                                PRIORITY,               // Prio
                                0,                      // Options
                                K_NO_WAIT);             // delay

    // k_thread_name_set(tid_blink, "Thread A"); // Set thread name
    printk("Blink thread started with ID: %p\r\n", tid_blink);

    // Start a second blink thread
    tid_blink2 = k_thread_create(&blink_thread2,         // Thread struct
                                  blink_stack2,            // Stack size
                                  K_THREAD_STACK_SIZEOF(blink_stack2),
                                  blink_thread_start,     // Thread entry point
                                  (void *)"Thread B",     // p1
                                  NULL,                   // p2
                                  NULL,                   // p3
                                  PRIORITY,               // Prio
                                  0,                      // Options
                                  K_NO_WAIT);             // delay
    printk("Second blink thread started with ID: %p\r\n", tid_blink2);

    while (1) {
        k_msleep(1000);
    }

    return 0;
}

/* ========================= 2 USING MUTEX =============*/

// #include <zephyr/kernel.h>
// #include <zephyr/drivers/gpio.h>

// #define LED0_NODE DT_ALIAS(led0)
// #define STACK_SIZE 512
// #define PRIORITY 5


// static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

// K_THREAD_STACK_DEFINE(blink_stack, STACK_SIZE);
// K_THREAD_STACK_DEFINE(blink_stack2, STACK_SIZE);

// // Declare thread data structs
// static struct k_thread blink_thread, blink_thread2;

// static struct k_mutex led_mutex;

// void led_thread(void *p1, void *p2, void *p3) {
//     const char *name = (const char *)p1;
//     while (1) {
//         k_mutex_lock(&led_mutex, K_FOREVER);
//         printk("%s has the mutex and is toggling the LED\n", name);
//         gpio_pin_toggle_dt(&led);
//         k_msleep(1000);
//         k_mutex_unlock(&led_mutex);
//     }
// }

// int main(void)
// {
//     int ret;
//     k_mutex_init(&led_mutex);
//    /* Create two threads that will toggle the LED
//       Each thread will try to acquire the mutex before toggling the LED
//       This ensures that only one thread can toggle the LED at a time
//       This prevents race conditions 
//     */

//     k_tid_t tid_blink, tid_blink2;

//     if (!gpio_is_ready_dt(&led)) {
//         printk(" GPIO pin not ready\r\n");
//         return 0;
//     }

//     ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT);
//     if (ret < 0) {
//         printk("Could not configure GPIO\r\n");
//         return 0;
//     }

//     // Start first blink thread
//     tid_blink = k_thread_create(&blink_thread,          // Thread struct
//                                 blink_stack,            // Stack size
//                                 K_THREAD_STACK_SIZEOF(blink_stack),
//                                 led_thread,     // Thread entry point
//                                 (void *)"Thread A",     // p1
//                                 NULL,                   // p2
//                                 NULL,                   // p3
//                                 PRIORITY,               // Prio
//                                 0,                      // Options
//                                 K_NO_WAIT);             // delay

//     // k_thread_name_set(tid_blink, "Thread A"); // Set thread name
//     printk("Blink thread started with ID: %p\r\n", tid_blink);

//     // Start a second blink thread
//     tid_blink2 = k_thread_create(&blink_thread2,         // Thread struct
//                                   blink_stack2,            // Stack size
//                                   K_THREAD_STACK_SIZEOF(blink_stack2),
//                                   led_thread,     // Thread entry point
//                                   (void *)"Thread B",     // p1
//                                   NULL,                   // p2
//                                   NULL,                   // p3
//                                   PRIORITY,               // Prio
//                                   0,                      // Options
//                                   K_NO_WAIT);             // delay
//     printk("Second blink thread started with ID: %p\r\n", tid_blink2);

//     while (1) {
//         k_msleep(1000);
//     }

//     return 0;
// }

/* ========================= 3 USING SEMAPHORES =============*/

// #include <zephyr/kernel.h>
// #include <zephyr/drivers/gpio.h>

// #define LED0_NODE DT_ALIAS(led0)
// #define STACK_SIZE 512
// #define PRIORITY 5

// static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
// static struct k_sem led_sem;

// K_THREAD_STACK_DEFINE(blink_stack, STACK_SIZE);
// K_THREAD_STACK_DEFINE(blink_stack2, STACK_SIZE);

// // Declare thread data structs
// static struct k_thread blink_thread, blink_thread2;

// void led_thread(void *p1, void *p2, void *p3) {
//     const char *name = (const char *)p1;
//     while (1) {
//         k_sem_take(&led_sem, K_FOREVER);
//         printk("%s took the semaphore and is toggling the LED\n", name);
//         gpio_pin_toggle_dt(&led);
//         k_msleep(1000);
//         k_sem_give(&led_sem);
//     }
// }


// int main(void)
// {
//     int ret;
//     k_sem_init(&led_sem, 1, 1);  // Binary semaphore
//     k_tid_t tid_blink, tid_blink2;

//     if (!gpio_is_ready_dt(&led)) {
//         printk(" GPIO pin not ready\r\n");
//         return 0;
//     }

//     ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT);
//     if (ret < 0) {
//         printk("Could not configure GPIO\r\n");
//         return 0;
//     }

//     // Start first blink thread
//     tid_blink = k_thread_create(&blink_thread,          // Thread struct
//                                 blink_stack,            // Stack size
//                                 K_THREAD_STACK_SIZEOF(blink_stack),
//                                 led_thread,     // Thread entry point
//                                 (void *)"Thread A",     // p1
//                                 NULL,                   // p2
//                                 NULL,                   // p3
//                                 PRIORITY,               // Prio
//                                 0,                      // Options
//                                 K_NO_WAIT);             // delay

//     // k_thread_name_set(tid_blink, "Thread A"); // Set thread name
//     printk("Blink thread started with ID: %p\r\n", tid_blink);

//     // Start a second blink thread
//     tid_blink2 = k_thread_create(&blink_thread2,         // Thread struct
//                                   blink_stack2,            // Stack size
//                                   K_THREAD_STACK_SIZEOF(blink_stack2),
//                                   led_thread,     // Thread entry point
//                                   (void *)"Thread B",     // p1
//                                   NULL,                   // p2
//                                   NULL,                   // p3
//                                   PRIORITY,               // Prio
//                                   0,                      // Options
//                                   K_NO_WAIT);             // delay
//     printk("Second blink thread started with ID: %p\r\n", tid_blink2);

//     while (1) {
//         k_msleep(1000);
//     }

//     return 0;
// }





/* ========== 0 THREAD SYNCHRONIZATION AT COMPILE TIME ============= */

// #include <zephyr/kernel.h>
// #include <zephyr/drivers/gpio.h>
// #define LED0_NODE DT_ALIAS(led0)
// #define STACK_SIZE 512
// #define PRIORITY 5

// static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

// void led_thread(void *p1, void *p2, void *p3) {
//     const char *name = (const char *)p1;
//     while (1) {
//         printk("%s is toggling the LED\n", name);
//         gpio_pin_toggle_dt(&led);
//         k_msleep(1000);
//     }
// }

// K_THREAD_DEFINE(thread_a, STACK_SIZE, led_thread, "Thread A", NULL, NULL, PRIORITY, 0, 0);
// K_THREAD_DEFINE(thread_b, STACK_SIZE, led_thread, "Thread B", NULL, NULL, PRIORITY, 0, 0);

// int main(void) {
//     gpio_pin_configure_dt(&led, GPIO_OUTPUT);
//     return 0;
// }


/* Mutex and semaphore example for LED control  at compile time */

/* ========================= 0-1 USING MUTEX =============*/


// #include <zephyr/kernel.h>
// #include <zephyr/drivers/gpio.h>

// #define LED0_NODE DT_ALIAS(led0)
// #define STACK_SIZE 512
// #define PRIORITY 5

// static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
// static struct k_mutex led_mutex;

// void led_thread(void *p1, void *p2, void *p3) {
//     const char *name = (const char *)p1;
//     while (1) {
//         k_mutex_lock(&led_mutex, K_FOREVER);
//         printk("%s has the mutex and is toggling the LED\n", name);
//         gpio_pin_toggle_dt(&led);
//         k_msleep(1000);
//         k_mutex_unlock(&led_mutex);
//     }
// }

// K_THREAD_DEFINE(thread_a, STACK_SIZE, led_thread, "Thread A", NULL, NULL, PRIORITY, 0, 0);
// K_THREAD_DEFINE(thread_b, STACK_SIZE, led_thread, "Thread B", NULL, NULL, PRIORITY, 0, 0);

// int main(void) {
//     gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
//     k_mutex_init(&led_mutex);
//     return 0;
// }



/* ========================= 0-2 USING SEMAPHORES =============*/

// #include <zephyr/kernel.h>
// #include <zephyr/drivers/gpio.h>

// #define LED0_NODE DT_ALIAS(led0)
// #define STACK_SIZE 512
// #define PRIORITY 5

// static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
// static struct k_sem led_sem;

// void led_thread(void *p1, void *p2, void *p3) {
//     const char *name = (const char *)p1;
//     while (1) {
//         k_sem_take(&led_sem, K_FOREVER);
//         printk("%s took the semaphore and is toggling the LED\n", name);
//         gpio_pin_toggle_dt(&led);
//         k_msleep(1000);
//         k_sem_give(&led_sem);
//     }
// }

// K_THREAD_DEFINE(thread_a, STACK_SIZE, led_thread, "Thread A", NULL, NULL, PRIORITY, 0, 0);
// K_THREAD_DEFINE(thread_b, STACK_SIZE, led_thread, "Thread B", NULL, NULL, PRIORITY, 0, 0);


// int main(void) {
//     gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
//     k_sem_init(&led_sem, 1, 1);  // Binary semaphore
//     return 0;
// }
