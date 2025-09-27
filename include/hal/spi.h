#ifndef HAL_STM32F4_SPI_H
#define HAL_STM32F4_SPI_H

#define HAL_SPI_COUNT               4
#define HAL_SPI_BUFFER_SIZE         256
#define HAL_SPI_POLLING_TRIES       10000

#ifdef __cplusplus
extern "C" {



#endif

#include "cmsis.h"
#include "types.h"
#include "error.h"
#include "dma.h"

typedef enum hal_spi_index {
    eHAL_SPI1 = 0,
    eHAL_SPI2 = 1,
    eHAL_SPI3 = 2,
    eHAL_SPI4 = 3
} hal_spi_index_t;

typedef struct hal_spi_setup_def {
    SPI_TypeDef *spi;
    GPIO_TypeDef *gpio_nss;
    uint8_t pin_nss;

    uint32_t cr1;
    uint32_t cr2;

    void (*rcv_cb)(hal_spi_index_t spi_index);

    void (*snd_cb)(hal_spi_index_t spi_index);

} hal_spi_setup_def_t;

typedef struct hal_spi_t {
    hal_spi_index_t spi_index;
    SPI_TypeDef *spi;
    GPIO_TypeDef *gpio_nss;
    uint8_t pin_nss;

    hal_dma_t *dma_rx;
    hal_dma_t *dma_tx;

    void (*rcv_cb)(hal_spi_index_t spi_index);

    void (*snd_cb)(hal_spi_index_t spi_index);
} hal_spi_t;

extern hal_error_t hal_spi_init(void);

extern hal_error_t hal_spi_setup(hal_spi_setup_def_t setup_def);

extern hal_error_t hal_spi_setup_dma(hal_spi_index_t spi_index, hal_dma_t *dma_rx, hal_dma_t *dma_tx);

extern hal_error_t hal_spi_snd_dma(hal_spi_index_t spi_index, const uint8_t *buffer, uint16_t length);

extern hal_error_t hal_spi_rcv_dma(hal_spi_index_t spi_index, uint8_t *buffer, uint16_t length);

extern hal_error_t hal_spi_snd_polling(hal_spi_index_t spi_index, const uint8_t *buffer, uint16_t length);

extern hal_error_t hal_spi_rcv_polling(hal_spi_index_t spi_index, uint8_t *buffer, uint16_t length);

extern hal_error_t hal_spi_snd_start(hal_spi_index_t spi_index);


#ifdef __cplusplus
}
#endif

#endif

