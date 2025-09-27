#ifndef HAL_STM32F4_UART_H
#define HAL_STM32F4_UART_H

#ifdef __cplusplus
extern "C" {



#endif

#include "types.h"
#include "error.h"
#include "cmsis.h"
#include "buffer.h"
#include "dma.h"

#define HAL_UART_COUNT              eHAL_UART_COUNT
#define HAL_UART_BUFFER_SIZE        256

typedef enum hal_uart_index {
    eHAL_UART1 = 0,
    eHAL_UART2,
    eHAL_UART3,
    eHAL_UART4,
    eHAL_UART5,
    eHAL_UART6,
    eHAL_UART_COUNT
} hal_uart_index_t;

typedef struct hal_uart_setup {
    USART_TypeDef *uart;
    uint32_t cr1;
    uint32_t cr2;
    uint32_t cr3;

} hal_uart_setup_t;

typedef struct hal_uart {
    USART_TypeDef *uart;

    hal_dma_t *dma_rx;
    hal_dma_t *dma_tx;

} hal_uart_t;

extern hal_error_t hal_uart_init(void);
extern hal_error_t hal_uart_enable_clock(USART_TypeDef *uartx);
extern void hal_uart_enable(USART_TypeDef *uartx);
extern void hal_uart_disable(USART_TypeDef *uartx);
extern hal_error_t hal_uart_setup(hal_uart_setup_t uart);
extern hal_error_t hal_uart_setup_dma(hal_uart_index_t uart_index, hal_dma_t *dma_rx, hal_dma_t *dma_tx);
extern hal_error_t hal_uart_snd_dma(hal_uart_index_t uart_index, uint16_t length);
extern hal_error_t hal_uart_rcv_dma(hal_uart_index_t uart_index, uint8_t *buffer, uint16_t length);
extern hal_error_t hal_uart_snd_polling(hal_uart_index_t uart_index, const uint8_t *buffer, uint16_t length);
extern hal_error_t hal_uart_rcv_polling(hal_uart_index_t uart_index, uint8_t *buffer, uint16_t length);

// extern bool_t hal_uart_transmission_complete(USART_TypeDef *uartx);
extern hal_error_t hal_uart_wait_for_transfer_complete(USART_TypeDef *uartx);
extern hal_error_t hal_uart_start_tx_dma(hal_uart_index_t uart_index, const void *buf, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif
