#ifndef HAL_STM32F4_DELAY_H
#define HAL_STM32F4_DELAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

extern void hal_delay_us(uint32_t us);
extern void hal_delay_ms(uint32_t ms);

#ifdef __cplusplus
}
#endif

#endif
