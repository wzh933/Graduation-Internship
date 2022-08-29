#include "stm32f407xx.h"
#include "utility.h"
#include "gpio.h"


// BSP相关 PA6 PA7
void DPZ_LED_Init()
{
	RCC->AHB1ENR |= 1 << 0;
	DPZ_IO_H( GPIOA, 6 );
	DPZ_IO_CFG( GPIOA, 6, 1, 0, 0, 1, 0 );
	DPZ_IO_H( GPIOA, 7 );
	DPZ_IO_CFG( GPIOA, 7, 1, 0, 0, 1, 0 );
}


void DPZ_GPIO_Reset( void )
{
	RCC->AHB1ENR |= 0x1FF;
	Delay_02NOP();


	RCC->AHB1RSTR |= 0x1FF;

	Delay_08NOP( 2 );

	RCC->AHB1RSTR &= ~0x1FF;
	Delay_02NOP();


	RCC->AHB1ENR &= ~0x1FF;
}




