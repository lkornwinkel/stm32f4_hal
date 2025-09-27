#include <stddef.h>
#include <hal/uart.h>
#include <hal/buffer.h>
#include <hal/cmsis.h>
#include <hal/clock.h>

static hal_uart_t m_hal_uart[HAL_UART_COUNT];

static hal_error_t hal_uart_check_index(hal_uart_index_t uart_index) {
    hal_error_t err = eHAL_ERROR_OK;

    if (uart_index >= HAL_UART_COUNT) {
        err = eHAL_ERROR_NOTOK;
    }

    return err;
}

static hal_clock_register_t hal_uart_get_clock_register(USART_TypeDef *uartx) {
    hal_error_t err = eHAL_ERROR_OK;
    hal_clock_register_t reg = {0};

    if (uartx == NULL) {
        err = eHAL_ERROR_NULLPOINTER;
    }

    if (err == eHAL_ERROR_OK) {

        if (uartx == USART1) {
            reg.address = (uint32_t) &(RCC->APB2ENR);
            reg.bit = RCC_APB2ENR_USART1EN;
        } else if (uartx == USART2) {
            reg.address = (uint32_t) &(RCC->APB1ENR);
            reg.bit = RCC_APB1ENR_USART2EN;
        } else if (uartx == USART3) {
            reg.address = (uint32_t) &(RCC->APB1ENR);
            reg.bit = RCC_APB1ENR_USART3EN;
        } else if (uartx == UART4) {
            reg.address = (uint32_t) &(RCC->APB1ENR);
            reg.bit = RCC_APB1ENR_UART4EN;
        } else if (uartx == UART5) {
            reg.address = (uint32_t) &(RCC->APB1ENR);
            reg.bit = RCC_APB1ENR_UART5EN;
        } else if (uartx == USART6) {
            reg.address = (uint32_t) &(RCC->APB2ENR);
            reg.bit = RCC_APB2ENR_USART6EN;
        } else {
            err = eHAL_ERROR_NOTSUPPORTED;
        }
    }

    return reg;
}

hal_error_t hal_uart_init(void) {
    return eHAL_ERROR_OK;
}

hal_error_t hal_uart_enable_clock(USART_TypeDef *uartx) {
    hal_error_t err = eHAL_ERROR_OK;
    hal_clock_register_t clockRegister;


    clockRegister = hal_uart_get_clock_register(uartx);
    if (!hal_clock_register_is_valid(clockRegister)) {
        err = eHAL_ERROR_NOTOK;
    }

    if (err == eHAL_ERROR_OK) {
        hal_clock_enable(clockRegister);
    }

    return err;
}

void hal_uart_enable(USART_TypeDef *uartx) {
    uartx->CR1 |= USART_CR1_UE | USART_CR1_TE;
}

void hal_uart_disable(USART_TypeDef *uartx) {
    uartx->CR1 &= ~(USART_CR1_UE | USART_CR1_TE);
}

bool_t hal_uart_transmission_complete(USART_TypeDef *uartx) {
    return (uartx->SR & USART_SR_TC) != 0;
}

hal_error_t hal_uart_get_index(USART_TypeDef *uart, hal_uart_index_t *indexRet) {
    hal_error_t err = eHAL_ERROR_OK;
    hal_uart_index_t index;

    if (uart == NULL || indexRet == NULL) {
        err = eHAL_ERROR_NULLPOINTER;
    }
    if (err == eHAL_ERROR_OK) {

        if (uart == USART1) {
            index = eHAL_UART1;
        } else if (uart == USART2) {
            index = eHAL_UART2;
        } else if (uart == USART3) {
            index = eHAL_UART3;
        } else if (uart == UART4) {
            index = eHAL_UART4;
        } else if (uart == UART5) {
            index = eHAL_UART5;
        } else if (uart == USART6) {
            index = eHAL_UART6;
        } else {
            err = eHAL_ERROR_NOTSUPPORTED;
        }
    }

    if (err == eHAL_ERROR_OK) {
        *indexRet = index;
    }

    return err;
}


hal_error_t hal_uart_setup(hal_uart_setup_t uart) {
    hal_error_t lErr;
    hal_uart_index_t uart_index;


    lErr = hal_uart_get_index(uart.uart, &uart_index);

    if (lErr == eHAL_ERROR_OK) {

        hal_uart_disable(uart.uart);

        m_hal_uart[uart_index].uart = uart.uart;

        uart.uart->CR1 = uart.cr1;
        uart.uart->CR2 = uart.cr2;
        uart.uart->CR3 = uart.cr3;
        uart.uart->BRR = 0x008b;


        // hal_uart_enable(uart.uart);
    }

    return lErr;
}

hal_error_t hal_uart_setup_dma(hal_uart_index_t uart_index, hal_dma_t *dma_rx, hal_dma_t *dma_tx) {
    hal_error_t err = eHAL_ERROR_OK;

    err = hal_uart_check_index(uart_index);

    if (err == eHAL_ERROR_OK) {
        m_hal_uart[uart_index].dma_rx = dma_rx;
        m_hal_uart[uart_index].dma_tx = dma_tx;
    }

    return err;
}

static hal_error_t hal_uart_snd_byte(uint8_t byte) {
    hal_error_t lErr = eHAL_ERROR_OK;
    USART_TypeDef *uartx = m_hal_uart[eHAL_UART1].uart;

    // Auf TXE (Datenregister leer) warten
    while ((uartx->SR & USART_SR_TXE) == 0) {
        /* warten */
    }

    uartx->DR = byte;

    return lErr;
}

hal_error_t hal_uart_snd_polling(hal_uart_index_t uart_index, const uint8_t *buffer, uint16_t length) {
    hal_error_t lErr = eHAL_ERROR_OK;

    USART_TypeDef *uartx = m_hal_uart[uart_index].uart;

    uartx->CR1 |= (USART_CR1_TE | USART_CR1_UE);

    for (uint16_t i = 0; i < length; i++) {
        lErr = hal_uart_snd_byte(buffer[i]);

        if (lErr != eHAL_ERROR_OK) {
            break;
        }
    }

    // Am Ende auf vollständige Übertragung warten (TC)
    while ((uartx->SR & USART_SR_TC) == 0) {
    }


    return lErr;
}

hal_error_t hal_uart_snd_dma(hal_uart_index_t uart_index, uint16_t length) {
    hal_error_t lErr = eHAL_ERROR_OK;
    USART_TypeDef *uartx = m_hal_uart[uart_index].uart;
    DMA_TypeDef *dmax = m_hal_uart[uart_index].dma_tx->dma;
    DMA_Stream_TypeDef *streamx = m_hal_uart[uart_index].dma_tx->stream;

    // hal_dma_disable_clock(dmax);

    streamx->NDTR = length;

    uartx->CR1 |= (USART_CR1_TE | USART_CR1_UE);
    uartx->CR3 |= USART_CR3_DMAT;

    // hal_dma_enable_clock(dmax);
}


hal_error_t hal_uart_start_tx_dma(hal_uart_index_t uart_index, const void *buf, uint16_t len) {
    hal_error_t lErr = eHAL_ERROR_OK;
    // hal_error_t err = hal_uart_check_index(uart_index);
    // if (err != eHAL_ERROR_OK) return err;

    DMA_Stream_TypeDef *stream = m_hal_uart[uart_index].dma_tx ? m_hal_uart[uart_index].dma_tx->stream : NULL;
    USART_TypeDef *uart = m_hal_uart[uart_index].uart;
    if (!stream || !uart) {
        return eHAL_ERROR_PARAM;
    }

    // Stream deaktivieren und Flags löschen
    stream->CR &= ~DMA_SxCR_EN;
    while (stream->CR & DMA_SxCR_EN) {
    } // warten bis aus
    // Flags je nach Stream löschen (hier Beispiel für generisches Clear; implementiere passend)
    // hal_dma_clear_flags(stream);

    // Quelle/Ziel/Länge setzen
    stream->PAR = (uint32_t) &uart->DR;
    stream->M0AR = (uint32_t) buf;
    stream->NDTR = len;

    // UART-DMA TX sicher aktiv

    hal_uart_enable(uart);

    // Stream starten
    stream->CR |= DMA_SxCR_EN;
    uart->CR3 |= USART_CR3_DMAT;
    return eHAL_ERROR_OK;
}


hal_error_t hal_uart_wait_for_transfer_complete(USART_TypeDef *uartx) {
    hal_error_t lErr = eHAL_ERROR_OK;

    if (uartx == NULL) {
        lErr = eHAL_ERROR_NULLPOINTER;
    }
    if (lErr == eHAL_ERROR_OK) {
        while ((USART1->SR & USART_SR_TC) == 0) {
            /* wait */
        }
    }

    return lErr;
}

