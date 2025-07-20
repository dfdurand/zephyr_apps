#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/sys/ring_buffer.h>

LOG_MODULE_REGISTER(ring_usart_app);

#define UART_DEVICE DT_CHOSEN(zephyr_console)

// #if defined(CONFIG_BOARD_NUCLEO_G071RB)     
// #define UART_DEVICE DT_NODELABEL(usart2)
// #elif defined(CONFIG_BOARD_NRF52840DK)
// #define UART_DEVICE DT_NODELABEL(uart0)
// #else
// #error "Unsupported board configuration, check your usart peripheral in devicetree"
// #endif

#define RING_BUF_SIZE 10

RING_BUF_DECLARE(rx_ringbuf, RING_BUF_SIZE);

const struct device *uart_dev = DEVICE_DT_GET(UART_DEVICE);

void print_uart(char *buf, int len)
{
	for (int i = 0; i < len; i++) {
		uart_poll_out(uart_dev, buf[i]);
	}
}

static void uart_cb(const struct device *dev, void *user_data)
{
    while (uart_irq_update(dev) &&  uart_irq_is_pending(dev)) {
        int len;
        int rd_len = 0;

        uint8_t *data;
        len = ring_buf_put_claim(&rx_ringbuf, &data, rx_ringbuf.size);

        if (len > 0) {
            rd_len = uart_fifo_read(dev, data, len);
            ring_buf_put_finish(&rx_ringbuf, rd_len);
        }
        else {
            LOG_WRN("rx buf full");
            uint8_t dummy;
            rd_len = uart_fifo_read(dev, &dummy, 1);
        }
    }
}


int main(void)
{
    int ret, len;
    uint8_t *data;

    if (!uart_dev) {
        LOG_ERR("Cannot get UART device");
        return -1;
    }

    ret = uart_irq_callback_user_data_set(uart_dev, uart_cb, NULL);

    if( ret < 0){
        LOG_ERR("failed to set uart callback \n");
    }

    uart_irq_rx_enable(uart_dev);
    
    LOG_INF("UART Ring Buffer UART Interrupt example Started, send  a message to the UART device");
    while (1) {

            len = ring_buf_get_claim(&rx_ringbuf, (uint8_t **)&data, rx_ringbuf.size);
            if (len > 0) {
                print_uart(data, len);
            }

            k_msleep(10); // Add a small delay to reduce CPU usage
    }
    
    return 0;
}