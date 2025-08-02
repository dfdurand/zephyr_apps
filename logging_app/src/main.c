#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/printk.h>
#include <zephyr/random/random.h>

LOG_MODULE_REGISTER(temp_monitor, LOG_LEVEL_INF); // Change level if needed

#define TEMP_READ_INTERVAL_MS 2000
#define CRITICAL_TEMP_C 35

static int simulate_temperature_reading(void) {
    // Simulate temperature between 20°C and 40°C
    return 20 + (sys_rand32_get() % 21);
}

int main(void) {
    printk(" ===== Starting the monitor temperature ===== \n");

    while (1) {
        int temp = simulate_temperature_reading();

        //  printk version
        printk("printk >> measured temperature : %d°C\n", temp);

        //  Logging Subsystem version
        LOG_INF("LOG_INF >> measured temperature : %d°C", temp);

        if (temp > CRITICAL_TEMP_C - 3 && temp < CRITICAL_TEMP_C)  {
            LOG_WRN(" !!! almost reached critical temp (%d°C)", temp);
        }

        if (temp > CRITICAL_TEMP_C) {
            LOG_ERR("critical temperature! (%d°C)", temp);
        }

        k_msleep(TEMP_READ_INTERVAL_MS);
    }

}
