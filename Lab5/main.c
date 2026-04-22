#include <stm32f10x.h>
#include <stdint.h>
#include <stdbool.h>

volatile static uint16_t target_ticks = 7;
volatile static uint16_t ticks_left;
volatile static uint16_t refresh_ticks = 7;
volatile static bool is_active;

const uint16_t ms_delay = 1000;
const uint16_t freq = 28;

void ConfigureGPIO() {
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN;
	
	GPIOA->CRL |= GPIO_CRL_MODE7_0;
	GPIOA->CRL &= ~GPIO_CRL_CNF7;
	
	GPIOB->CRL &= ~GPIO_CRL_MODE0 & ~GPIO_CRL_MODE1;
	
	GPIOB->CRL &= ~GPIO_CRL_CNF0 & ~GPIO_CRL_CNF1;
	GPIOB->CRL |= GPIO_CRL_CNF0_1 | GPIO_CRL_CNF1_1;
	
	GPIOB->BSRR |= GPIO_BSRR_BR0 | GPIO_BSRR_BR1;
	
	AFIO->EXTICR[0] |= AFIO_EXTICR1_EXTI0_PB | AFIO_EXTICR1_EXTI1_PB;
	
	EXTI->RTSR |= EXTI_RTSR_TR0;
	EXTI->IMR |= EXTI_IMR_MR0;
	
	EXTI->RTSR |= EXTI_RTSR_TR1;
	EXTI->IMR |= EXTI_IMR_MR1;
	
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_EnableIRQ(EXTI1_IRQn);
}

void EXTI0_IRQHandler() {
	refresh_ticks = target_ticks * 2;
	EXTI->PR &= EXTI_PR_PR0;
}

void EXTI1_IRQHandler() {
	refresh_ticks = target_ticks;
	EXTI->PR &= EXTI_PR_PR1;
}

void TIM3InitMS(void) {
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	
	TIM3->PSC = freq - 1;
	TIM3->ARR = (ms_delay / 2) - 1;
	TIM3->DIER |= TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM3_IRQn);
	TIM3->CR1 = TIM_CR1_CEN;
}

void TIM3_IRQHandler() {
	TIM3->SR &= ~TIM_SR_UIF;
	ticks_left -= 1;
}

void start() {
	while (true) {
		while(ticks_left != 0);
		ticks_left = refresh_ticks;
		if (is_active) {
			GPIOA->ODR |= GPIO_ODR_ODR7;
		}
		else {
			GPIOA->ODR &= ~GPIO_ODR_ODR7;
		}
		is_active = !is_active;
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
	//delay(1);
	ConfigureGPIO();
	TIM3InitMS();
	ticks_left = refresh_ticks;
	is_active = false;
	start();
}