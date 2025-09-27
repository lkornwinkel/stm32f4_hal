#include <hal/gpio.h>

void hal_gpio_setup(hal_gpio_setup_def_t *gpio_data, uint8_t firstIndex, uint8_t count) {
    for (uint8_t i = 0; i < count; i++) {
        uint8_t index = i + firstIndex;
        hal_gpio_enable(gpio_data->gpio);

        // if (gpio_data->mode == eHAL_GPIO_OUTPUT) {
        hal_gpio_set_output_type(gpio_data->gpio, gpio_data->pin, gpio_data->output);
        hal_gpio_set_speed(gpio_data->gpio, gpio_data->pin, gpio_data->speed);
        hal_gpio_set_pull(gpio_data->gpio, gpio_data->pin, gpio_data->pull);
        // }

        hal_gpio_set_alternate(gpio_data->gpio, gpio_data->pin, gpio_data->af);

        hal_gpio_set_mode(gpio_data->gpio, gpio_data->pin, gpio_data->mode);
    }
}

void hal_gpio_set_mode(GPIO_TypeDef *GPIOx, uint8_t pinPos, hal_gpio_mode_t mode) {
    uint32_t shift = (pinPos & 0x0f) * 2;
    uint32_t mask = 0x03 << shift;
    GPIOx->MODER = (GPIOx->MODER & ~mask) | ((uint32_t) mode) << shift;
}

void hal_gpio_set_output_type(GPIO_TypeDef *GPIOx, uint8_t pinPos, hal_gpio_output_t output) {
    uint32_t shift = (pinPos & 0x0f);
    uint32_t mask = 1u << shift;
    GPIOx->OTYPER = (GPIOx->OTYPER & ~mask) | ((uint32_t) output) << shift;
}

void hal_gpio_set_speed(GPIO_TypeDef *GPIOx, uint8_t pinPos, hal_gpio_speed_t speed) {
    uint32_t shift = (pinPos & 0x0f) * 2;
    uint32_t mask = 0x03 << shift;
    GPIOx->OSPEEDR = (GPIOx->OSPEEDR & ~mask) | ((uint32_t) speed) << shift;
}

void hal_gpio_set_pull(GPIO_TypeDef *GPIOx, uint8_t pinPos, hal_gpio_pull_t pull) {
    uint32_t shift = (pinPos & 0x0f) * 2;
    uint32_t mask = 0x03 << shift;
    GPIOx->PUPDR = (GPIOx->PUPDR & ~mask) | ((uint32_t) pull) << shift;
}

void hal_gpio_set_alternate(GPIO_TypeDef *GPIOx, uint8_t pinPos, hal_gpio_af_t af) {
    uint32_t shift = (pinPos & 0x0f) * 4;
    uint32_t mask = 0x03 << shift;
    if (pinPos < 8) {
        GPIOx->AFR[0] = (GPIOx->AFR[0] & ~mask) | ((uint32_t) af) << shift;
    } else {
        GPIOx->AFR[1] = (GPIOx->AFR[1] & ~mask) | ((uint32_t) af) << shift;
    }
}

void hal_gpio_enable(GPIO_TypeDef *GPIOx) {
    if (GPIOx == GPIOA) {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    }
    if (GPIOx == GPIOB) {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    }
    if (GPIOx == GPIOC) {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    }
    if (GPIOx == GPIOD) {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    }
    if (GPIOx == GPIOE) {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
    }
    if (GPIOx == GPIOF) {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;
    }
    if (GPIOx == GPIOG) {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
    }
}

void hal_gpio_disable(GPIO_TypeDef *GPIOx) {
    if (GPIOx == GPIOA) {
        RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOAEN;
    }
    if (GPIOx == GPIOB) {
        RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOBEN;
    }
    if (GPIOx == GPIOC) {
        RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOCEN;
    }
    if (GPIOx == GPIOD) {
        RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIODEN;
    }
    if (GPIOx == GPIOE) {
        RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOEEN;
    }
    if (GPIOx == GPIOF) {
        RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOFEN;
    }
    if (GPIOx == GPIOG) {
        RCC->AHB1ENR &= ~RCC_AHB1ENR_GPIOGEN;
    }
}

/**
 * Sets pin to SET
 *
 * @param GPIOx HW GPIO to use
 * @param pinPos the pin position [0..15]
 */
void hal_gpio_set(GPIO_TypeDef *GPIOx, uint8_t pinPos) {
    GPIOx->BSRR |= (1 << pinPos);
}

void hal_gpio_reset(GPIO_TypeDef *GPIOx, uint8_t pinPos) {
    GPIOx->BSRR |= (1 << pinPos) << 16;
}

void hal_gpio_toggle(GPIO_TypeDef *GPIOx, uint8_t pinPos) {
    if (hal_gpio_output_is_set(GPIOx, pinPos)) {
        hal_gpio_reset(GPIOx, pinPos);
    } else {
        hal_gpio_set(GPIOx, pinPos);
    }
}

bool_t hal_gpio_output_is_set(GPIO_TypeDef *gpio, uint8_t pin) {
    return gpio->ODR & (1 << pin) == (1 << pin);
}
