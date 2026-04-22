#include <stm32f10x.h>
#include <stdint.h>
#include <stdbool.h>

const uint16_t start_time_ms = 3876;


volatile static uint16_t ms_left = 0;
const uint16_t ms_delay = 1000;
const uint16_t freq = 28;

void TIM3InitMS(void) {
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	
	TIM3->PSC = freq - 1;
	TIM3->ARR = ms_delay - 1;
	TIM3->DIER |= TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM3_IRQn);
	TIM3->CR1 = TIM_CR1_CEN;
}

void delay(uint16_t timeout) {
	ms_left = timeout;
	TIM3InitMS();
	while (ms_left != 0);
}

void TIM3_IRQHandler() {
	TIM3->SR &= ~TIM_SR_UIF;
	ms_left -= 1;
}


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


int main(void) {
	SystemCoreClockConfigure();
	SystemCoreClockUpdate();
	//delay(start_time_ms);
	delay(1);
	while(true);
}