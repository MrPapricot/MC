#include <stm32f10x.h>
#include <stdint.h>
#include <stdbool.h>


uint32_t start_delay_ms = 3876;

/*
void Delay(uint32_t delay) {
	for (uint32_t i = 0; i < delay; i++);
}
*/


/*
void Delay_ms(uint32_t delay_ms) {
	uint32_t delay = delay_ms * 1400;
	Delay(delay);
}
*/

void Delay_1_ms(void) {
	for (uint32_t i = 0; i < 1471; i++);
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
	__NOP();
}	

void Delay(uint32_t ms) {
	for (uint32_t i = 0; i < ms; i++) {
		Delay_1_ms();
	}
}


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


int main(void) {
	SystemCoreClockConfigure();
	SystemCoreClockUpdate();
	Delay(start_delay_ms);
	while(true);
}