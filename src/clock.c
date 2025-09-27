#include <hal/cmsis.h>
#include <hal/clock.h>
#include <hal/util.h>

void hal_clock_enable(hal_clock_register_t reg)
{
    *(uint32_t *)reg.address |= reg.bit;
}

void hal_clock_disable(hal_clock_register_t reg)
{
    *(uint32_t *)reg.address &= ~reg.bit;
}

hal_error_t hal_system_clock_init(void)
{
    hal_error_t err = eHAL_ERROR_OK;
    uint32_t timeout = 100000, counter;
    uint32_t pll_m = 8, pll_n = 360, pll_p = 2;

    // enable HSE
    RCC->CR |= (RCC_CR_HSEON);

    counter = timeout;
    while((counter > 0) && (RCC->CR & RCC_CR_HSERDY) != RCC_CR_HSERDY)
    {
        counter--;
    }

    if((RCC->CR & RCC_CR_HSERDY) != RCC_CR_HSERDY)
    {
        err = eHAL_ERROR_TIMEOUT;
    }

    if(err == eHAL_ERROR_OK) {

        RCC->APB1ENR |= (RCC_APB1ENR_PWREN);

        PWR->CR |= (3 << PWR_CR_VOS_Pos);  // regulator voltage scaling output selection (p. 111f)

        FLASH->ACR = (
                FLASH_ACR_DCEN |    // data cache enable
                FLASH_ACR_ICEN |    // instruction cache enable
                FLASH_ACR_PRFTEN |  // prefetch enable
                (5 << FLASH_ACR_LATENCY_Pos)    // 6 wait states latency
        );

        RCC->CFGR &= ~(1 << RCC_CFGR_HPRE_Pos);     // set AHB prescaler to 1
        RCC->CFGR |= (5 << RCC_CFGR_PPRE1_Pos);     // set APB1 prescaler to 4
        RCC->CFGR |= (4 << RCC_CFGR_PPRE2_Pos);     // set APB2 prescaler to 2

        RCC->PLLCFGR =
                (pll_m << RCC_PLLCFGR_PLLM_Pos) | (pll_n << RCC_PLLCFGR_PLLN_Pos) | (pll_p << RCC_PLLCFGR_PLLP_Pos)
                | RCC_PLLCFGR_PLLSRC_HSE;    // enable HSE

    }

    counter = timeout;
    RCC->CR |= (RCC_CR_PLLON);
    while((counter > 0) && (RCC->CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY)
    {
        counter--;
    }

    if((RCC->CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY)
    {
        err = eHAL_ERROR_TIMEOUT;
    }

    return err;

}

hal_error_t hal_system_clock_set_speed(uint32_t hs_clock_mhz, uint32_t sys_clock_mhz)
{
    hal_error_t err = eHAL_ERROR_OK;
    uint32_t timeout = 100000, counter;
    uint32_t pll_m = 8, pll_n = 360, pll_p = 2;

    if((hs_clock_mhz < 4) || (hs_clock_mhz > 25))
    {
        err = eHAL_ERROR_NOTSUPPORTED;
    }

    if(err == eHAL_ERROR_OK)
    {
        pll_m = hs_clock_mhz;

    }
}