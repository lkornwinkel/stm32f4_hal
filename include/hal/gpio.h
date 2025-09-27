#ifndef HAL_STM32F4_GPIO_H
#define HAL_STM32F4_GPIO_H

#include <stm32f446xx.h>

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

enum hal_gpio_pull
{
    eHAL_GPIO_NOPULL,
    eHAL_GPIO_PULLUP,
    eHAL_GPIO_PULLDOWN
};

typedef enum hal_gpio_pull hal_gpio_pull_t;

enum hal_gpio_mode
{
    eHAL_GPIO_OUTPUT,
    eHAL_GPIO_INPUT,
    eHAL_GPIO_ALTERNATE,
    eHAL_GPIO_ANALOG
};

typedef enum hal_gpio_mode hal_gpio_mode_t;

enum hal_gpio_speed
{
    eHAL_GPIO_SPEED_LOW,
    eHAL_GPIO_SPEED_MEDIUM,
    eHAL_GPIO_SPEED_FAST,
    eHAL_GPIO_SPEED_HIGH
};

typedef enum hal_gpio_speed hal_gpio_speed_t;

enum hal_gpio_af
{
    eHAL_GPIO_AF0,
    eHAL_GPIO_AF1,
    eHAL_GPIO_AF2,
    eHAL_GPIO_AF3,
    eHAL_GPIO_AF4,
    eHAL_GPIO_AF5,
    eHAL_GPIO_AF6,
    eHAL_GPIO_AF7,
    eHAL_GPIO_AF8,
    eHAL_GPIO_AF9,
    eHAL_GPIO_AF10,
    eHAL_GPIO_AF11,
    eHAL_GPIO_AF12,
    eHAL_GPIO_AF13,
    eHAL_GPIO_AF14,
    eHAL_GPIO_AF15
};

typedef enum hal_gpio_af hal_gpio_af_t;

enum hal_gpio_output
{
    eHAL_GPIO_PUSHPULL,
    eHAL_GPIO_OPENDRAIN
};

typedef enum hal_gpio_output hal_gpio_output_t;


struct hal_gpio_setup_def
{
    GPIO_TypeDef *gpio;
    uint8_t pin;
    hal_gpio_mode_t mode;
    hal_gpio_output_t output;
    hal_gpio_af_t af;
    hal_gpio_pull_t pull;
    hal_gpio_speed_t speed;
};

typedef struct hal_gpio_setup_def hal_gpio_setup_def_t;

extern void hal_gpio_setup(hal_gpio_setup_def_t *gpio_data, uint8_t firstIndex, uint8_t count);
extern void hal_gpio_set_mode(GPIO_TypeDef *GPIOx, uint8_t pinPos, hal_gpio_mode_t mode);
extern void hal_gpio_set_output_type(GPIO_TypeDef *GPIOx, uint8_t pinPos, hal_gpio_output_t output);
extern void hal_gpio_set_speed(GPIO_TypeDef *GPIOx, uint8_t pinPos, hal_gpio_speed_t speed);
extern void hal_gpio_set_pull(GPIO_TypeDef *GPIOx, uint8_t pinPos, hal_gpio_pull_t pull);
extern void hal_gpio_set_alternate(GPIO_TypeDef *GPIOx, uint8_t pinPos, hal_gpio_af_t af);

extern void hal_gpio_enable(GPIO_TypeDef *gpio);
extern void hal_gpio_disable(GPIO_TypeDef *gpio);
extern void hal_gpio_set(GPIO_TypeDef *gpio, uint8_t pin);
extern void hal_gpio_reset(GPIO_TypeDef *gpio, uint8_t pin);
extern bool_t hal_gpio_output_is_set(GPIO_TypeDef *gpio, uint8_t pin);
extern void hal_gpio_toggle(GPIO_TypeDef *gpio, uint8_t pin);



#ifdef __cplusplus
}
#endif

#endif
