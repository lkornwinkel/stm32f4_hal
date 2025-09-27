#ifndef HAL_STM32F4_ERROR_H
#define HAL_STM32F4_ERROR_H

#ifdef __cplusplus
extern "C" {



#endif

enum hal_error {
    eHAL_ERROR_OK,
    eHAL_ERROR_NOTOK,
    eHAL_ERROR_NULLPOINTER,
    eHAL_ERROR_NOTSUPPORTED,
    eHAL_ERROR_TIMEOUT,
    eHAL_ERROR_PARAM
};

typedef enum hal_error hal_error_t;

#ifdef __cplusplus
}
#endif

#endif
