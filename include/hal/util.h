#ifndef HAL_STM32F4_UTIL_H
#define HAL_STM32F4_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"
#include "error.h"

extern hal_error_t hal_util_wait_set(volatile uint32_t *reg, uint32_t mask, uint32_t tries);
extern hal_error_t hal_util_wait_clear(volatile uint32_t *reg, uint32_t mask, uint32_t tries);

extern void hal_util_systick_enable(void);
extern void hal_util_systick_disable(void);
extern uint64_t hal_util_get_ms(void);
extern void hal_util_set_ms(uint64_t new_ms);
extern void hal_util_inc_ms(void);
extern void hal_util_wait_ms(uint64_t ms);

#ifdef __cplusplus
}
#endif

#endif
