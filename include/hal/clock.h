#ifndef HAL_STM32F4_CLOCK_H
#define HAL_STM32F4_CLOCK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"
#include "error.h"

struct hal_clock_register
{
    uint32_t address;
    uint32_t bit;
};

typedef struct hal_clock_register hal_clock_register_t;

static inline bool_t hal_clock_register_is_valid(hal_clock_register_t reg)
{
    return reg.address != 0 && reg.bit != 0;
}

extern void hal_clock_enable(hal_clock_register_t reg);
extern void hal_clock_disable(hal_clock_register_t reg);

extern hal_error_t hal_system_clock_init(void);

#ifdef __cplusplus
}
#endif

#endif
