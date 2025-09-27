#ifndef HAL_STM32F4_DMA_H
#define HAL_STM32F4_DMA_H

#include "cmsis.h"
#include "error.h"
#include "types.h"

#ifdef __cplusplus
extern "C" {



#endif

typedef enum hal_dma_dir {
    eHAL_DMA_DIR_PER_TO_MEM = 0,
    eHAL_DMA_DIR_MEM_TO_PER = 1,
} hal_dma_dir_t;

typedef struct hal_dma_setup_def {
    DMA_TypeDef *dma; ///< dma
    DMA_Stream_TypeDef *stream; ///< dma stream
    hal_dma_dir_t dir; ///< dma stream direction
    uint32_t per_addr; ///< peripheral address
    uint32_t mem_addr; ///< memory address
    uint8_t channel; ///< dma channel
    bool_t flow_control; ///< peripheral flow control, if supported
    uint8_t prio; ///< stream priority
    uint8_t fifo; ///< DMA FIFO usage
} hal_dma_setup_def_t;

typedef struct hal_dma {
    DMA_TypeDef *dma;
    DMA_Stream_TypeDef *stream;
} hal_dma_t;

extern hal_error_t hal_dma_init(void);
extern hal_error_t hal_dma_setup(hal_dma_setup_def_t setup_def);
extern void hal_dma_enable_clock(DMA_TypeDef *dma);
extern void hal_dma_disable_clock(DMA_TypeDef *dma);
extern hal_error_t hal_dma_stop(DMA_Stream_TypeDef *stream, uint32_t tries);
extern hal_error_t hal_dma_start(DMA_Stream_TypeDef *stream, uint16_t length);
extern bool_t hal_dma_transmission_complete(DMA_TypeDef *dmax);
extern hal_error_t hal_dma_wait_for_transfer_complete(DMA_TypeDef *dmax, DMA_Stream_TypeDef *streamx);

extern hal_error_t hal_dma_clear_flags(DMA_TypeDef *dmax, DMA_Stream_TypeDef *streamx);
extern hal_error_t hal_dma_get_stream_index(DMA_TypeDef *dmax, DMA_Stream_TypeDef *streamx, uint8_t *indexRet);
extern hal_error_t hal_dma_get_dma_index(DMA_TypeDef *dmax, uint8_t *indexRet);

#ifdef __cplusplus
}
#endif

#endif
