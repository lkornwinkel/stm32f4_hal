#include <stddef.h>
#include <hal/init.h>
#include <hal/clock.h>
#include <hal/cmsis.h>

void hal_init(void) {
    hal_clock_register_t lRegisterConfig[] = {
            {
                    .address = (uint32_t) &RCC->APB1ENR,
                    .bit = RCC_APB1ENR_PWREN
            },
            {
                    .address = (uint32_t) &RCC->APB2ENR,
                    .bit = RCC_APB2ENR_USART1EN
            }
    };

    for (size_t i = 0; i < sizeof(lRegisterConfig) / sizeof(hal_clock_register_t); i++) {
        hal_clock_enable(lRegisterConfig[i]);
    }
}
