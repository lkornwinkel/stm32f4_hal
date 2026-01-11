#include <hal/clock.h>
#include <hal/dma.h>
#include <hal/error.h>
#include <hal/gpio.h>
#include <hal/init.h>
#include <hal/uart.h>
#include <hal/util.h>
#include <stddef.h>
#include <string.h>

hal_gpio_setup_def_t uart_pins[] = {
    {
        .gpio = GPIOB,
        .pin = 6,
        .mode = eHAL_GPIO_ALTERNATE,
        .output = eHAL_GPIO_PUSHPULL,
        .af = eHAL_GPIO_AF7,
        .pull = eHAL_GPIO_PULLUP,
        .speed = eHAL_GPIO_SPEED_HIGH,
    }
};

hal_uart_setup_t uart_setup = {
    .uart = USART1,
    .cr1 = 0x00000000,
    .cr2 = 0x00000000,
    .cr3 = 0x00000000,
};

uint8_t buffer[512] = "Hello World!\r\n";

hal_dma_setup_def_t dma_setup_tx = {
    .channel = 4,
    .dir = eHAL_DMA_DIR_MEM_TO_PER,
    .dma = DMA2,
    .stream = DMA2_Stream7,
    .fifo = 0,
    .flow_control = false,
    .mem_addr = (uint32_t)buffer,
    .per_addr = (uint32_t)&USART1->DR,
    .prio = 0
};

hal_dma_t dma_tx = {
    .dma = DMA2,
    .stream = DMA2_Stream7,
};


hal_dma_setup_def_t dma_setup_rx = {
    .channel = 4,
    .dir = eHAL_DMA_DIR_PER_TO_MEM,
    .dma = DMA2,
    .stream = DMA2_Stream6,
    .fifo = 0,
    .flow_control = false,
    .mem_addr = (uint32_t)buffer,
    .per_addr = (uint32_t)&USART1->DR,
    .prio = 0
};

hal_dma_t dma_rx = {
    .dma = DMA2,
    .stream = DMA2_Stream6,
};



void USART1_IRQHandler(void) {
    // Prüfen, ob es ein IDLE Line Interrupt war
    if (USART1->SR & USART_SR_IDLE) {
        volatile uint32_t tmp;
        // IDLE Flag löschen: Sequenz ist Lesen von SR gefolgt von Lesen von DR
        tmp = USART1->SR;
        tmp = USART1->DR;
        (void)tmp; // Warnung für ungenutzte Variable verhindern

        // Hier: DMA stoppen, Anzahl empfangener Bytes berechnen, User-Callback aufrufen
        hal_dma_stop(dma_tx.stream, 1000);
    }
}

int main()
{
    hal_error_t lErr = eHAL_ERROR_OK;

    hal_init();
    hal_dma_enable_clock(dma_tx.dma);
    hal_uart_enable_clock(USART1);
    hal_uart_init();

    if (lErr == eHAL_ERROR_OK)
    {
        hal_gpio_setup(uart_pins, 0, sizeof(uart_pins) / sizeof(uart_pins[0]));

        lErr = hal_uart_setup(uart_setup);
    }

    if (lErr == eHAL_ERROR_OK)
    {
        lErr = hal_dma_setup(dma_setup_tx);
    }

    if (lErr == eHAL_ERROR_OK)
    {
        lErr = hal_uart_setup_dma(eHAL_UART1, 0, &dma_tx);
    }

    while (1)
    {
        hal_uart_start_rx_dma_idle(eHAL_UART1, buffer, sizeof(buffer) - 1);
        hal_uart_start_tx_dma(eHAL_UART1, buffer, strlen(buffer) - 1);
        hal_dma_wait_for_transfer_complete(dma_tx.dma, dma_tx.stream);
        hal_dma_clear_flags(dma_tx.dma, dma_tx.stream);
        hal_uart_wait_for_transfer_complete(uart_setup.uart);

    }
}