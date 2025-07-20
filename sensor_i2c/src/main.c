#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/i2c.h>

/* Define the label of the sensor (should match your overlay or board definition) */
#define BME680_NODE DT_NODELABEL(mysensor)

/* Create a struct for device and address from DTS */
static const struct i2c_dt_spec bme680 = I2C_DT_SPEC_GET(BME680_NODE);

int main(void)
{
    printk("BME680 raw I2C example\n");

    if (!device_is_ready(bme680.bus)) {
        printk("I2C bus not ready!\n");
        return 0;
    }

    /* BME680 register addresses (from datasheet) */
    const uint8_t BME680_REG_ID = 0xD0;   /* Chip ID Register */
    const uint8_t BME680_EXPECTED_ID = 0x61;

    uint8_t chip_id;
    int ret;

    /* Read chip ID */
    ret = i2c_write_read_dt(&bme680, &BME680_REG_ID, sizeof(BME680_REG_ID),
                            &chip_id, sizeof(chip_id));
    if (ret != 0) {
        printk("Failed to read chip ID: %d\n", ret);
        return 0;
    }

    printk("Chip ID: 0x%02X\n", chip_id);
    if (chip_id != BME680_EXPECTED_ID) {
        printk("Unexpected chip ID! Is the sensor connected?\n");
        return 0;
    }

    printk("BME680 detected!\n");


    /* Read temperature registers (0x22, 0x23, 0x24) */

    /* ========  GET RAW TEMPERATURE :  adc value =============*/

    uint8_t buf[3];  // Buffer to hold temp_msb, temp_lsb, temp_xlsb
    uint8_t temp_reg = 0x22;  // Temperature MSB register address
    
    ret = i2c_write_read_dt(&bme680, &temp_reg, sizeof(temp_reg), buf, sizeof(buf));
    if (ret != 0) {
        printk("Failed to read temperature registers: %d\n", ret);
        return 0;
    }
    
    /* buf[0] = temp_msb
        * buf[1] = temp_lsb
        * buf[2] = temp_xlsb
        */
    uint32_t temp_adc = ((uint32_t)buf[0] << 12) | ((uint32_t)buf[1] << 4) | ((uint32_t)buf[2] >> 4);
    
    printk("Raw temperature ADC value: %d\n", temp_adc);


    /* ========  CALIBRATION PARAMETERS =============*/

    uint8_t calib_data[3]; // For par_t2 (2 bytes) + par_t3 (1 byte)

    uint8_t reg_addr;

    // Read par_t1
    reg_addr = 0xE9;
    uint8_t par_t1_lsb_msb[2];
    i2c_write_read_dt(&bme680, &reg_addr, sizeof(reg_addr), par_t1_lsb_msb, sizeof(par_t1_lsb_msb));
    uint16_t par_t1 = ((uint16_t)par_t1_lsb_msb[1] << 8) | par_t1_lsb_msb[0];

    // Read par_t2 and par_t3
    reg_addr = 0x8A;
    i2c_write_read_dt(&bme680, &reg_addr, sizeof(reg_addr), calib_data, sizeof(calib_data));
    int16_t par_t2 = ((int16_t)calib_data[1] << 8) | calib_data[0]; // careful: signed
    int8_t par_t3 = (int8_t)calib_data[2];

    printk("Calibration params:\npar_t1 = %u\npar_t2 = %d\npar_t3 = %d\n", par_t1, par_t2, par_t3);


    /* ========  TEMPERATURE VALUE  =============*/
    float var1, var2;
    float t_fine, compensated_temp;
    /* Calculate temperature */
    /* Formulas from BME680 datasheet : bst-bme680-ds001.pdf*/
    var1 = (((float)temp_adc) / 16384.0f - ((float)par_t1) / 1024.0f) * ((float)par_t2);
    var2 = ((((float)temp_adc) / 131072.0f - ((float)par_t1) / 8192.0f) * 
            (((float)temp_adc) / 131072.0f - ((float)par_t1) / 8192.0f)) * ((float)par_t3 * 16.0f);

    t_fine = var1 + var2;
    compensated_temp = t_fine / 5120.0f;

    printk("Compensated temperature: %.2f °C\n", compensated_temp);

   return 0;
}
