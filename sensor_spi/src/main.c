#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

#define SPI_SENSOR DT_NODELABEL(mysensor) 
const struct device *const bme680 = DEVICE_DT_GET(SPI_SENSOR);


#define SLEEP_TIME_MS 1000 // Sample every 1 second

int main(void)
{
    
    // Check if the BME680 device was found and is ready.
    if (!device_is_ready(bme680)) {
        printk("Error: BME680 device is not ready\n");
        return 0;
    }

    printk("BME680 sensor found and ready.\n");
    printk("Fetching sensor data every %d ms...\n", SLEEP_TIME_MS);

    while (1) {
        struct sensor_value temp, press, humidity;
        int rc;

        // Trigger a sensor measurement fetch
        rc = sensor_sample_fetch(bme680);
        if (rc != 0) {
            printk("Sensor sample fetch failed (error %d)\n", rc);
            k_sleep(K_MSEC(SLEEP_TIME_MS)); // Wait before trying again
            continue;
        }

        // Get individual sensor readings
        rc = sensor_channel_get(bme680, SENSOR_CHAN_AMBIENT_TEMP, &temp);
        if (rc == 0) {
            rc = sensor_channel_get(bme680, SENSOR_CHAN_PRESS, &press);
        }
        if (rc == 0) {
            rc = sensor_channel_get(bme680, SENSOR_CHAN_HUMIDITY, &humidity);
        }

        if (rc != 0) {
            printk("Failed to get sensor data (error %d)\n", rc);
        } else {
            // Print the sensor values.
            // sensor_value.val1 contains the integer part, sensor_value.val2 contains the fractional part (in micro units).
            // For temperature, val2 is micro-degrees Celsius (multiply by 1e-6)
            // For pressure, val2 is micro-Pascals (multiply by 1e-6)
            // For humidity, val2 is micro-percent (multiply by 1e-6)

            printk("Temperature: %d.%06d C\n", temp.val1, temp.val2);
            printk("Pressure: %d.%06d Pa\n", press.val1, press.val2);
            printk("Humidity: %d.%06d %%RH\n", humidity.val1, humidity.val2);
        }

        // Wait for the next sample
        k_sleep(K_MSEC(SLEEP_TIME_MS));
    }

    return 0;
}
