#include <zephyr/devicetree.h>
#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/logging/log.h>
#include <stdint.h>

LOG_MODULE_REGISTER(uart_it_demo, LOG_LEVEL_INF);

#define UART_DEVICE_NODE DT_CHOSEN(zephyr_console)

// #if defined(CONFIG_BOARD_NUCLEO_G071RB)     
// #define UART_DEVICE_NODE DT_NODELABEL(usart2)
// #elif defined(CONFIG_BOARD_NRF52840DK)
// #define UART_DEVICE_NODE DT_NODELABEL(uart0)
// #else
// #error "Unsupported board configuration, check your usart peripheral in devicetree"
// #endif

/* max buffer length */
#define RX_BUF_MAX_LEN 10

static const struct device *uart_dev = DEVICE_DT_GET(UART_DEVICE_NODE);

static uint8_t rx_buffer[RX_BUF_MAX_LEN];
static size_t rx_buf_pos = 0;

static void uart_cb(const struct device *dev, void *user_data)
{
    ARG_UNUSED(user_data);

    uint8_t recv_char;

    while (uart_irq_update(dev) && uart_irq_is_pending(dev)) {
        if (uart_irq_rx_ready(dev)) {
            while (uart_fifo_read(dev, &recv_char, 1)) {
                if (rx_buf_pos < RX_BUF_MAX_LEN) {
                    rx_buffer[rx_buf_pos++] = recv_char;
                    // optionally echo back received character immediately -> comment code in main() while loop
                    // uart_fifo_fill(dev, &recv_char, 1);
                } else {
                    // Buffer overflow
                    LOG_WRN("RX buffer overflow! Discarding data.");
                }
            }
        }
    }
}

int main(void)
{
    if (!uart_dev) {
        LOG_INF("Cannot find %s!\n", uart_dev->name);
        return -1;
    }

    LOG_INF("UART Interrupt-driven example started with RX buffer max length %d", RX_BUF_MAX_LEN);

    uart_irq_callback_set(uart_dev, uart_cb); // Set the callback function for UART interrupts
    uart_irq_rx_enable(uart_dev);

    while (1) {
        k_sleep(K_MSEC(10));

        if (rx_buf_pos > 0) {
            rx_buffer[rx_buf_pos] = '\0'; // Null-terminate for safe printing
            LOG_INF("Received data: %s", (char *)rx_buffer); //important
            rx_buf_pos = 0; // Reset buffer position after processing
        }
    }
}