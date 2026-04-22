#include <stm32f10x.h>
#include <stdint.h>
#include <stdbool.h>

const uint32_t start_time_ms = 3876;


volatile static uint32_t ms_left = 0;



void SystemCoreClockConfigure (void) {
    RCC->CR |= RCC_CR_HSION;
    while ((RCC->CR & RCC_CR_HSIRDY) == 0);

    RCC->CFGR &= ~RCC_CFGR_SW;
	
		RCC->CR &= ~RCC_CR_PLLON;

    RCC->CFGR &= ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL);
    RCC->CFGR |= (RCC_CFGR_PLLSRC_HSI_Div2 | RCC_CFGR_PLLMULL7);

    RCC->CR |= RCC_CR_PLLON;
    while ((RCC->CR & RCC_CR_PLLRDY) == 0) __NOP();

    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV1;
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}

void SysTick_Handler(void) {
	ms_left--;
}


void delay_ms(void) {
	SysTick_Config(28000);
}

void delay(uint32_t ms) {
	ms_left = ms;
	delay_ms();
	while (ms_left > 0);
}


int main(void) {
	SystemCoreClockConfigure();
	SystemCoreClockUpdate();
	delay(start_time_ms);
	while(true);
}