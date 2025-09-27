#include <hal/dma.h>

void hal_dma_enable_clock(DMA_TypeDef *DMAx) {
    if (DMAx == DMA1) {
        RCC->AHB1ENR |= RCC_AHB1ENR_DMA1EN;
    }

    if (DMAx == DMA2) {
        RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
    }
}

void hal_dma_disable_clock(DMA_TypeDef *DMAx) {
    if (DMAx == DMA1) {
        RCC->AHB1ENR &= ~RCC_AHB1ENR_DMA1EN;
    }

    if (DMAx == DMA2) {
        RCC->AHB1ENR &= ~RCC_AHB1ENR_DMA2EN;
    }
}

hal_error_t hal_dma_stop(DMA_Stream_TypeDef *streamx, uint32_t tries) {
    streamx->CR &= ~(DMA_SxCR_EN << DMA_SxCR_EN_Pos);
    hal_error_t err = eHAL_ERROR_OK;


    while ((streamx->CR & (DMA_SxCR_EN_Msk)) != 0 && tries > 0) {
        --tries;
    }

    if (tries == 0) {
        err = eHAL_ERROR_TIMEOUT;
    }

    return err;
}

hal_error_t hal_dma_setup(hal_dma_setup_def_t def) {
    hal_error_t err;


    err = hal_dma_stop(def.stream, 10000);

    // hal_dma_enable_clock(def.dma);

    if (err == eHAL_ERROR_OK) {
        def.stream->CR = 0;
        def.stream->PAR = def.per_addr;
        def.stream->M0AR = def.mem_addr;
        def.stream->CR |= (def.channel & 0x7) << DMA_SxCR_CHSEL_Pos;
        def.stream->CR |= def.flow_control ? (1 << DMA_SxCR_PFCTRL_Pos) : 0;
        def.stream->CR |= (def.prio & 0x3) << DMA_SxCR_PL_Pos;
        def.stream->CR |= (def.dir & 0x3) << DMA_SxCR_DIR_Pos;
        def.stream->CR |= DMA_SxCR_MINC;
        def.stream->FCR = 0;
    }

    return eHAL_ERROR_OK;
}

hal_error_t hal_dma_start(DMA_Stream_TypeDef *streamx, uint16_t length) {
    streamx->NDTR = length;
    streamx->CR |= DMA_SxCR_EN;

    return eHAL_ERROR_OK;
}

hal_error_t hal_dma_wait_for_transfer_complete(DMA_TypeDef *dmax, DMA_Stream_TypeDef *streamx) {
    hal_error_t lErr = eHAL_ERROR_OK;
    volatile uint32_t *lSr = 0;
    uint32_t lMask;
    uint8_t lOffsetIndex;
    uint8_t lOffsets[] = {5, 11, 21, 27};
    uint8_t streamIndex;

    lErr = hal_dma_get_stream_index(dmax, streamx, &streamIndex);

    if (lErr == eHAL_ERROR_OK) {
        if (streamIndex >= 0 && streamIndex <= 3) {
            lSr = &dmax->LISR;
            lOffsetIndex = streamIndex;
        } else if (streamIndex >= 4 && streamIndex <= 7) {
            lSr = &dmax->HISR;
            lOffsetIndex = streamIndex - 4;
        }

        lMask = 1 << lOffsets[lOffsetIndex];

        while ((*lSr & lMask) == 0) {
            /* wait */
        }
    }

    return lErr;
}

hal_error_t hal_dma_clear_flags(DMA_TypeDef *dmax, DMA_Stream_TypeDef *streamx) {
    // DMA2->HIFCR = DMA_HIFCR_CTCIF7 | DMA_HIFCR_CTEIF7 | DMA_HIFCR_CFEIF7 | DMA_HIFCR_CDMEIF7 | DMA_HIFCR_CHTIF7;
    hal_error_t lErr = eHAL_ERROR_OK;
    volatile uint32_t *lSr = 0;
    uint32_t lMask = 0;
    uint8_t lOffsetIndex = 0;
    uint8_t lOffsets[] = {0, 6, 16, 22};
    uint8_t lGroupIndex[] = {0, 2, 3, 4, 5};
    uint8_t lStreamIndex;

    lErr = hal_dma_get_stream_index(dmax, streamx, &lStreamIndex);

    if (lErr == eHAL_ERROR_OK) {
        if (lStreamIndex <= 3) {
            lSr = &dmax->LIFCR;
            lOffsetIndex = lStreamIndex;
        } else if (lStreamIndex >= 4 && lStreamIndex <= 7) {
            lSr = &dmax->HIFCR;
            lOffsetIndex = lStreamIndex - 4;
        }

        for (uint8_t i = 0; i < 5; i++) {
            lMask |= 1 << (lOffsets[lOffsetIndex] + lGroupIndex[i]);
        }

        *lSr = lMask;
    }

    return lErr;


    DMA2->HIFCR = DMA_HIFCR_CTCIF7 | DMA_HIFCR_CTEIF7 | DMA_HIFCR_CFEIF7 | DMA_HIFCR_CDMEIF7 | DMA_HIFCR_CHTIF7;
}

hal_error_t hal_dma_get_stream_index(DMA_TypeDef *dmax, DMA_Stream_TypeDef *streamx, uint8_t *indexRet) {
    hal_error_t lErr = eHAL_ERROR_OK;
    uint8_t lStreamIndex = 0;
    uint8_t lDmaIndex = 0;

    if (indexRet == NULL || dmax == NULL || streamx == NULL) {
        lErr = eHAL_ERROR_NULLPOINTER;
    }

    lErr = hal_dma_get_dma_index(dmax, &lDmaIndex);

    if (lErr == eHAL_ERROR_OK) {
        if (lDmaIndex == 0) {
            if (streamx == DMA1_Stream0) {
                lStreamIndex = 0;
            } else if (streamx == DMA1_Stream1) {
                lStreamIndex = 1;
            } else if (streamx == DMA1_Stream2) {
                lStreamIndex = 2;
            } else if (streamx == DMA1_Stream3) {
                lStreamIndex = 3;
            } else if (streamx == DMA1_Stream4) {
                lStreamIndex = 4;
            } else if (streamx == DMA1_Stream5) {
                lStreamIndex = 5;
            } else if (streamx == DMA1_Stream6) {
                lStreamIndex = 6;
            } else if (streamx == DMA1_Stream7) {
                lStreamIndex = 7;
            } else {
                lErr = eHAL_ERROR_PARAM;
            }
        } else {
            if (streamx == DMA2_Stream0) {
                lStreamIndex = 0;
            } else if (streamx == DMA2_Stream1) {
                lStreamIndex = 1;
            } else if (streamx == DMA2_Stream2) {
                lStreamIndex = 2;
            } else if (streamx == DMA2_Stream3) {
                lStreamIndex = 3;
            } else if (streamx == DMA2_Stream4) {
                lStreamIndex = 4;
            } else if (streamx == DMA2_Stream5) {
                lStreamIndex = 5;
            } else if (streamx == DMA2_Stream6) {
                lStreamIndex = 6;
            } else if (streamx == DMA2_Stream7) {
                lStreamIndex = 7;
            } else {
                lErr = eHAL_ERROR_PARAM;
            }
        }
    }

    if (lErr == eHAL_ERROR_OK) {
        *indexRet = lStreamIndex;
    }

    return lErr;
}

hal_error_t hal_dma_get_dma_index(DMA_TypeDef *dmax, uint8_t *indexRet) {
    hal_error_t lErr = eHAL_ERROR_OK;
    uint8_t lIndex = 0;

    if (indexRet == NULL) {
        lErr = eHAL_ERROR_NULLPOINTER;
    }

    if (lErr == eHAL_ERROR_OK) {
        if (dmax == DMA1) {
            lIndex = 0;
        } else if (dmax == DMA2) {
            lIndex = 1;
        } else {
            lErr = eHAL_ERROR_PARAM;
        }
    }

    if (lErr == eHAL_ERROR_OK) {
        *indexRet = lIndex;
    }

    return lErr;
}
