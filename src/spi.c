#include <hal/spi.h>
#include <hal/cmsis.h>
#include <hal/buffer.h>
#include <hal/util.h>

#include <string.h>

static hal_spi_t m_hal_spi[HAL_SPI_COUNT];

static hal_error_t hal_spi_snd_start_dma(hal_spi_index_t spi_index, uint16_t length);

static hal_error_t hal_spi_rcv_start_dma(hal_spi_index_t spi_index, uint16_t length);

static hal_error_t hal_spi_check_index(hal_spi_index_t spi_index) {
    hal_error_t err = eHAL_ERROR_OK;


    if (spi_index >= HAL_SPI_COUNT) {
        err = eHAL_ERROR_NOTOK;
    }

    return err;
}

hal_error_t hal_spi_init(void) {
    hal_error_t err = eHAL_ERROR_OK;

    return err;
}

hal_error_t hal_spi_get_index(SPI_TypeDef *spi, hal_spi_index_t *index_ret) {
    hal_spi_index_t spi_index = 0;
    hal_error_t err = eHAL_ERROR_OK;


    if ((spi == NULL) || (index_ret == NULL)) {
        err = eHAL_ERROR_NULLPOINTER;
    }
    if (err == eHAL_ERROR_OK) {
        if (spi == SPI1) {
            spi_index = eHAL_SPI1;
        } else if (spi == SPI2) {
            spi_index = eHAL_SPI2;
        } else if (spi == SPI3) {
            spi_index = eHAL_SPI3;
        } else if (spi == SPI4) {
            spi_index = eHAL_SPI4;
        } else {
            err = eHAL_ERROR_NOTSUPPORTED;
        }
    }

    if (err == eHAL_ERROR_OK) {
        *index_ret = spi_index;
    }

    return err;
}

hal_error_t hal_spi_check_setup_def(hal_spi_setup_def_t setup_def) {
    hal_error_t lError = eHAL_ERROR_OK;

    if (setup_def.spi == NULL || setup_def.rcv_cb == NULL || setup_def.snd_cb == NULL) {
        lError = eHAL_ERROR_NULLPOINTER;
    }

    return lError;
}

hal_error_t hal_spi_setup(hal_spi_setup_def_t setup_def) {
    hal_error_t err = eHAL_ERROR_OK;
    hal_spi_index_t spi_index;

    err = hal_spi_get_index(setup_def.spi, &spi_index);

    if (err == eHAL_ERROR_OK) {
        err = hal_spi_check_setup_def(setup_def);

        m_hal_spi[spi_index].spi_index = spi_index;
        m_hal_spi[spi_index].spi = setup_def.spi;
        m_hal_spi[spi_index].rcv_cb = setup_def.rcv_cb;
        m_hal_spi[spi_index].snd_cb = setup_def.snd_cb;

        setup_def.spi->CR1 = setup_def.cr1;
        setup_def.spi->CR2 = setup_def.cr2;
    }
    return err;
}

hal_error_t hal_spi_setup_dma(hal_spi_index_t spi_index, hal_dma_t *dma_rx, hal_dma_t *dma_tx) {
    hal_error_t err = eHAL_ERROR_OK;

    err = hal_spi_check_index(spi_index);

    if (err == eHAL_ERROR_OK) {
        m_hal_spi[spi_index].dma_rx = dma_rx;
        m_hal_spi[spi_index].dma_tx = dma_tx;
    }

    return err;
}

hal_error_t hal_spi_snd_dma(hal_spi_index_t spi_index, const uint8_t *buffer, uint16_t length) {
    hal_error_t err = eHAL_ERROR_OK;


    if (buffer == NULL) {
        err = eHAL_ERROR_NULLPOINTER;
    }

    if (err == eHAL_ERROR_OK) {
        if (length == 0 || length > HAL_SPI_BUFFER_SIZE) {
            err = eHAL_ERROR_NOTSUPPORTED;
        }
    }

    if (err == eHAL_ERROR_OK) {
        err = hal_spi_check_index(spi_index);
    }

    if (err == eHAL_ERROR_OK) {
        // memcpy(m_hal_spi[spi_index].buffer_rx, buffer, length);

        err = hal_spi_snd_start_dma(spi_index, length);
    }

    return err;
}

static hal_error_t hal_spi_snd_start_dma(hal_spi_index_t spi_index, uint16_t length) {
    hal_error_t err = eHAL_ERROR_OK;

    if (m_hal_spi[spi_index].dma_tx == NULL) {
        err = eHAL_ERROR_NULLPOINTER;
    }

    if (err == eHAL_ERROR_OK) {
        err = hal_spi_check_index(spi_index);
    }

    if (err == eHAL_ERROR_OK) {
        err = hal_dma_start(m_hal_spi[spi_index].dma_tx->stream, length);
    }

    return err;
}

hal_error_t hal_spi_snd_polling(hal_spi_index_t spi_index, const uint8_t *buffer, uint16_t length) {
    hal_error_t err = eHAL_ERROR_OK;

    if (buffer == NULL) {
        err = eHAL_ERROR_NULLPOINTER;
    }

    if (err == eHAL_ERROR_OK) {
        if (length == 0 || length > HAL_SPI_BUFFER_SIZE) {
            err = eHAL_ERROR_NOTSUPPORTED;
        }
    }

    if (err == eHAL_ERROR_OK) {
        err = hal_spi_check_index(spi_index);
    }

    if (err == eHAL_ERROR_OK) {
        for (uint16_t i = 0; i < length; ++i) {
            m_hal_spi[spi_index].spi->DR = buffer[i];
            m_hal_spi[spi_index].spi->SR |= SPI_SR_TXE;
            err = hal_util_wait_clear(&m_hal_spi[spi_index].spi->SR, SPI_SR_TXE, HAL_SPI_POLLING_TRIES);
            if (err != eHAL_ERROR_OK) {
                break;
            }
        }
    }

    return err;
}

hal_error_t hal_spi_rcv_dma(hal_spi_index_t spi_index, uint8_t *buffer, uint16_t length) {
    hal_error_t err = eHAL_ERROR_OK;


    if (buffer == NULL) {
        err = eHAL_ERROR_NULLPOINTER;
    }

    if (err == eHAL_ERROR_OK) {
        if (length == 0 || length > HAL_SPI_BUFFER_SIZE) {
            err = eHAL_ERROR_NOTSUPPORTED;
        }
    }

    if (err == eHAL_ERROR_OK) {
        err = hal_spi_check_index(spi_index);
    }

    if (err == eHAL_ERROR_OK) {
        err = hal_spi_rcv_start_dma(spi_index, length);
    }

    return err;
}

static hal_error_t hal_spi_rcv_start_dma(hal_spi_index_t spi_index, uint16_t length) {
    hal_error_t err = eHAL_ERROR_OK;

    err = hal_spi_check_index(spi_index);

    if (err == eHAL_ERROR_OK) {
        if (m_hal_spi[spi_index].dma_rx == NULL) {
            err = eHAL_ERROR_NULLPOINTER;
        }
    }

    if (err == eHAL_ERROR_OK) {
        err = hal_dma_start(m_hal_spi[spi_index].dma_rx->stream, length);
    }

    return err;
}

hal_error_t hal_spi_rcv_polling(hal_spi_index_t spi_index, uint8_t *buffer, uint16_t length) {
    hal_error_t err = eHAL_ERROR_OK;


    if (buffer == NULL) {
        err = eHAL_ERROR_NULLPOINTER;
    }

    if (err == eHAL_ERROR_OK) {
        if (length == 0 || length > HAL_SPI_BUFFER_SIZE) {
            err = eHAL_ERROR_NOTSUPPORTED;
        }
    }

    if (err == eHAL_ERROR_OK) {
        err = hal_spi_check_index(spi_index);
    }

    if (err == eHAL_ERROR_OK) {
        for (uint16_t i = 0; i < length; ++i) {
            err = hal_util_wait_set(&m_hal_spi[spi_index].spi->SR, SPI_SR_RXNE, HAL_SPI_POLLING_TRIES);
            if (err != eHAL_ERROR_OK) {
                break;
            }

            buffer[i] = m_hal_spi[spi_index].spi->DR;
        }
    }

    return err;
}
