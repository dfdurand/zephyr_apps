#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/spi.h>
#include <string.h>
#include <stdio.h>


LOG_MODULE_REGISTER(spi_freq);

#if defined(CONFIG_BOARD_NUCLEO_G071RB)
#define SPI_NODE DT_NODELABEL(spi1)
#elif defined(CONFIG_BOARD_NRF52840DK)
#define SPI_NODE DT_NODELABEL(spi3)
#else
#error "Unsupported board configuration, check your usart peripheral in devicetree"
#endif


#define SPI_FREQ DT_PROP(SPI_NODE, clock_frequency)    /* SPI frequency in Hz */
#define SPI_SLAVE 0                 /* Slave ID, usually 0 */



static struct spi_config spi_cfg = {
	.operation = SPI_WORD_SET(8) | SPI_TRANSFER_MSB | SPI_OP_MODE_MASTER | SPI_LINES_SINGLE |  SPI_HOLD_ON_CS,
	.frequency = SPI_FREQ,
	.slave = SPI_SLAVE,
	.cs = NULL,
};


int main(void)
{
    const struct device *spi_dev = DEVICE_DT_GET(SPI_NODE);

    if (!device_is_ready(spi_dev)) {
        LOG_ERR("SPI device not ready");
        // return;
    }

    LOG_INF("SPI device ready, %s", spi_dev->name);

    /* random data */
    uint8_t tx_data[] = {0xAA, 0xBB, 0xCC};
    uint8_t rx_data[3] = {0};
    struct spi_buf tx_buf = {
        .buf = tx_data,
        .len = sizeof(tx_data),
    };
    struct spi_buf rx_buf = {
        .buf = rx_data,
        .len = sizeof(rx_data),
    };

    struct spi_buf_set tx_bufs = {
        .buffers = &tx_buf,
        .count = 1,
    };
    struct spi_buf_set rx_bufs = {
        .buffers = &rx_buf,
        .count = 1,
    };
 
    int ret = spi_transceive(spi_dev, &spi_cfg, &tx_bufs, &rx_bufs);
    if (ret < 0) {
        LOG_ERR("SPI transceive failed: %d", ret);
    } else {
        LOG_INF("SPI transceive successful");
        LOG_INF("Received: 0x%02X 0x%02X 0x%02X", rx_data[0], rx_data[1], rx_data[2]);
        LOG_HEXDUMP_INF(rx_data, sizeof(rx_data), "Received data:");
    }

}

