#include <hal/util.h>

static volatile uint64_t _systick_ms = 0;

uint64_t hal_util_get_ms(void)
{
    return _systick_ms;
}

void hal_util_set_ms(uint64_t new_ms)
{
    _systick_ms = new_ms;
}

void hal_util_inc_ms(void)
{
    ++_systick_ms;
}

void hal_util_wait_ms(uint64_t ms)
{
    uint64_t start_time = hal_util_get_ms();
    uint64_t time = start_time;


    while(time - start_time < ms)
    {
        time = hal_util_get_ms();
    }
}

hal_error_t hal_util_wait_set(volatile uint32_t *reg, uint32_t mask, uint32_t tries)
{
    hal_error_t err = eHAL_ERROR_OK;


    while(tries > 0)
    {
        if((*reg & mask) == mask)
        {
            break;
        }
    }
    if((*reg & mask) != mask)
    {
        err = eHAL_ERROR_NOTOK;
    }

    return err;
}

hal_error_t hal_util_wait_clear(volatile uint32_t *reg, uint32_t mask, uint32_t tries)
{
    hal_error_t err = eHAL_ERROR_OK;


    while(tries > 0)
    {
        if((*reg & mask) == 0)
        {
            break;
        }
    }
    if((*reg & mask) != 0)
    {
        err = eHAL_ERROR_NOTOK;
    }

    return err;
}
