#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/sys/printk.h> 
#include <zephyr/devicetree.h>

#define UART_NODE DT_CHOSEN(zephyr_console)

/* or use the following  configuration */
// #if defined(CONFIG_BOARD_NUCLEO_G071RB)
// #define UART_NODE DT_NODELABEL(usart2)
// #elif defined(CONFIG_BOARD_NRF52840DK)
// #define UART_NODE DT_NODELABEL(uart0)
// #else
// #error "Unsupported board configuration, check your usart peripheral in devicetree"
// #endif

#define RX_BUF_SIZE 32

const struct device *uart_dev = DEVICE_DT_GET(UART_NODE);

void print_string(const struct device *dev, const char *str)
{
    while (*str) {
        uart_poll_out(dev, *str++);
    }
}

int main(void)
{
    if (!device_is_ready(uart_dev)) {
        printk("UART device not ready.\n");
        return -1;
    }

    uint8_t rx_buf[RX_BUF_SIZE];
    size_t rx_index = 0;

    print_string(uart_dev, "\nUART Polling Mode with RX Buffer\n");
    print_string(uart_dev, "Type characters. Max buffer: 32 bytes\n");

    while (1) {
        uint8_t c;
        int ret = uart_poll_in(uart_dev, &c);
        if (ret == 0) {
            // Echo back the character
            uart_poll_out(uart_dev, c);

            // Store in buffer only if there is space
            if (rx_index < RX_BUF_SIZE) {
                rx_buf[rx_index++] = c;
            } else {
                print_string(uart_dev, "\r\nBuffer overflow\r\n");
                rx_index = 0;  // Reset buffer
            }

            // Handle Enter key to manually reset buffer
            if (c == '\r' || c == '\n') {
                uart_poll_out(uart_dev, '\n');
                rx_index = 0;
            }
        } else {
            // No data, sleep a bit
            k_msleep(10);
        }
    }
    // This line will never be reached
}
