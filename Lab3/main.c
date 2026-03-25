#include <stm32f10x.h>
#include <stdint.h>
#include <stdbool.h>

uint32_t start_time_ms = 3876;


uint32_t ms_to_ticks(uint32_t ms) {
	return ms * 1400;
}


volatile bool flag = true;


void SystemCoreClockConfigure (void) {
    RCC->CR |= RCC_CR_HSION;
    while ((RCC->CR & RCC_CR_HSIRDY) == 0);

    RCC->CFGR &= ~RCC_CFGR_SW;
	
		RCC->CR &= ~RCC_CR_PLLON;

    RCC->CFGR &= ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL);
    RCC->CFGR |= (RCC_CFGR_PLLSRC_HSI_Div2 | RCC_CFGR_PLLMULL7);

    RCC->CR |= RCC_CR_PLLON;
    while ((RCC->CR & RCC_CR_PLLRDY) == 0);

    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV1;
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}

void SysTick_Handler(void) {
	flag = false;
}


int main(void) {
	SystemCoreClockConfigure();
	SystemCoreClockUpdate();
	SysTick_Config(ms_to_ticks(3876));
	
	while(flag);
	return 0;
}