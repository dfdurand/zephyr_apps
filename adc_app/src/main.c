#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/adc.h>

static const struct adc_dt_spec adc_channel = ADC_DT_SPEC_GET(DT_PATH(zephyr_user));

#define LM35DZ_MV_PER_DEGREE_C 10.0f

int main(void)
{
    printk(" === ADC Application Sample === \n");

    int16_t buffer;
    struct adc_sequence sequence = {
        .buffer = &buffer,
        .buffer_size = sizeof(buffer),
    };

    if (!adc_is_ready_dt(&adc_channel)) {
		printk("ADC device %s not ready", adc_channel.dev->name);
		return -1;
	}

    int err = adc_channel_setup_dt(&adc_channel);
    if (err < 0) {
        printk("ADC channel setup failed: %d\n", err);
        return -1;
    }

    err = adc_sequence_init_dt(&adc_channel, &sequence);
    if (err < 0) {
        printk("ADC sequence init failed: %d\n", err);
        return -1;
    }

    while (1) {
        int err = adc_read(adc_channel.dev, &sequence);

        if (err < 0) {
            printk("ADC read failed: %d\n", err);
        } else {
            int32_t millivolts = buffer;
            err = adc_raw_to_millivolts_dt(&adc_channel, &millivolts);
            if (err < 0) {
                printk("Conversion to mV failed: %d\n", err);
            } else {
                float temperature_c = millivolts / LM35DZ_MV_PER_DEGREE_C;
                printk("Temperature: %.2f °C\n", (double)temperature_c);  
            }
        }

        k_sleep(K_SECONDS(1));
    }

    return 0;
}
