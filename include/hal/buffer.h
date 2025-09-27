#ifndef HAL_STM32F4_BUFFER_H
#define HAL_STM32F4_BUFFER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

struct hal_buffer
{
    uint8_t *data;
    uint32_t length;
};

typedef struct hal_buffer hal_buffer_t;

struct hal_rx_tx_buffer
{
    hal_buffer_t rx, tx;
};

typedef struct hal_rx_tx_buffer hal_rx_tx_buffer_t;


extern bool_t hal_buffer_check(hal_buffer_t buffer);
extern bool_t hal_rx_tx_buffer_check(hal_rx_tx_buffer_t buffer);

#ifdef __cplusplus
}
#endif

#endif
