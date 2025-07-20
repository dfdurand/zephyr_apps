#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/sys/printk.h>
#include <zephyr/drivers/uart.h>

#define UART_NODE DT_CHOSEN(zephyr_console)

/*Define the size of the receive buffer */
#define RECEIVE_BUFF_SIZE 30

/* Define the receiving timeout period (µs) */
#define RECEIVE_TIMEOUT 10

/* Get uart/usart device */
const struct device *uart = DEVICE_DT_GET(UART_NODE); 

const struct uart_config uart_cfg = {
		.baudrate = 115200,
		.parity = UART_CFG_PARITY_NONE,
		.stop_bits = UART_CFG_STOP_BITS_1,
		.data_bits = UART_CFG_DATA_BITS_8,
		.flow_ctrl = UART_CFG_FLOW_CTRL_NONE
	};

/* Define the receive buffer */
static uint8_t rx_buf[RECEIVE_BUFF_SIZE] = {0};

/* Define the transmit buffer */
static uint8_t tx_buf[] =  {" ====== Uart async api application ======== \n\r"};

int len;

uint8_t length, offset;

/* callback function */
static void uart_cb(const struct device *dev, struct uart_event *evt, void *user_data)
{
    ARG_UNUSED(user_data);

    switch (evt->type) {
    
    case UART_TX_DONE:
		printk("===== TX done ======\r\n");
        break;        
	case UART_RX_RDY:
   
        // printk("all messages: \r\n");
        printk("-> all messages: ");
        printk("%s", evt->data.rx.buf);
        // LOG_INF("%s", evt->data.rx.buf + evt->data.rx.offset);

        length = evt->data.rx.len ;
        offset = evt->data.rx.offset ;

      
        printk("\n-> offset :  %d\r\n",offset);
        printk("-> length :  %d\r\n",length);

        len = evt->data.rx.len + evt->data.rx.offset;

        // printk("current msg: \r\n");
        printk("current msg: ");
        for (int i = offset; i < len; i++) {
            uart_poll_out(uart, evt->data.rx.buf[i]);
        }

	     break;
    case UART_RX_DISABLED:
        printk("===== RX is disabled ======\r\n");
        uart_rx_enable(dev, rx_buf, sizeof rx_buf, RECEIVE_TIMEOUT); // enable for continuous reception
    
        break;
    case UART_RX_BUF_RELEASED:
        printk("===== Buffer is no longer used by UART driver. ======\r\n");

        break;		
    case UART_RX_BUF_REQUEST:
        printk("===== Request next buffer when overflow ======\r\n");
        break;

    case UART_TX_ABORTED:
        printk("===== TX aborted : TIMEOUT ======\r\n");

        break;

    case UART_RX_STOPPED:
        printk("===== RX has stopped due to external event. ======\r\n");
        printk("Reason: %d\r\n", evt->data.rx_stop.reason);

        break;
	default:
	     break;
	}
}


int main(void)
{
	printk("===== Uart async api application ======\r\n");
    int ret;

	/* check if uart device is ready */
    if (!device_is_ready(uart)){
        printk("Device %s is not ready.\r\n", uart->name);
        printk("Please check your device tree configuration.\r\n");
		return 1 ;
	}
    
    /* setup uart communication */
    int err = uart_configure(uart, &uart_cfg);

    if (err < 0) {
        printk("uart_configure failed with error %d\r\n", err);
        return err;
    }

    /* Register the UART callback function */
    ret = uart_callback_set(uart, uart_cb, NULL);

    if (ret < 0) {
        printk("uart_callback_set failed with error %d\r\n", ret);
        return ret;
    }

    /* Start receiving by calling uart_rx_enable() and pass it the address of the receive  buffer */
    ret = uart_rx_enable(uart, rx_buf, sizeof rx_buf, RECEIVE_TIMEOUT);

    if (ret) {
        printk("fail to enable RX with error %d\r\n", ret);
        return ret;
    }

    /* Start transmitting by calling uart_tx() and pass it the address of the transmit buffer */
    ret = uart_tx(uart, tx_buf, sizeof(tx_buf), SYS_FOREVER_MS);

	while (1) {
        // k_sleep(K_MSEC(100));
		}

	return 0;

}
