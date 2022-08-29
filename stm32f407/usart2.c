#include "stm32f407xx.h"
#include "utility.h"
#include "gpio.h"
#include "sys.h"
#include "usart2.h"


void DPZ_USART2_Init( unsigned int baud, unsigned int remap )
{
	unsigned int tmp;
	unsigned short mantissa;

	if ( remap == 0 )
	{
		/* 使能PA PA2(TX)推挽输出50M PA3(RX)上拉输入 */
		RCC->AHB1ENR |= 1 << 0;
		DPZ_IO_H( GPIOA, 2 );
		DPZ_IO_CFG( GPIOA, 2, 2, 0, 2, 1, 7 );
		DPZ_IO_CFG( GPIOA, 3, 2, 0, 2, 1, 7 );
	}
	else
	{
		/* 重映射PD5 PD6 */
		RCC->AHB1ENR |= 1 << 3;
		DPZ_IO_H( GPIOD, 5 );
		DPZ_IO_CFG( GPIOD, 5, 2, 0, 2, 1, 7 );
		DPZ_IO_CFG( GPIOD, 6, 2, 0, 2, 1, 7 );
	}

	tmp = baud << 4;
	if ( g_pclk1 <= tmp )
	{
		return;
	}

	mantissa = g_pclk1 / tmp;
	mantissa =  mantissa << 4 | ( g_pclk1 - mantissa * tmp + ( baud >> 1 ) ) / baud;

	/* 首先禁止USART2 */
	USART2->CR1 &= ~( 1 << 13 );

	/* 使能USART2时钟 */
	RCC->APB1ENR |= 1 << 17;

	/* 重启USART2 */
	RCC->APB1RSTR |= 1 << 17;
	RCC->APB1RSTR &= ~( 1 << 17 );

	/* 置BAUD率 */
	USART2->BRR = mantissa;
	/* 一个停止位 */
	USART2->CR2 &= ~( 3 << 12 );
	/* 8个数据位 无奇偶校验 */
	USART2->CR1 &= ~( 1 << 12 | 1 << 10 );
	/* 允许发送接收 */
	/* 开奇偶校验(PE)中断 他们大牛都说STM32的串口有BUG 此处必须这样 */
	/* 开接收区不空中断(RXNEIE) 也就是有数据就中断(包括过载 过载就是数据有了还来 多了) */
	USART2->CR1 |= 3 << 2 | 1 << 8 | 1 << 5;
	/* USART 16次采样 */
	USART2->CR1 &= ~( 1 << 15 );

	/* 两个优先级各2位  3 2 比串口1高
	   通道为USART2_IRQn 对应处理函数为USART2_IRQHandler  在startup的汇编文件中 */
	DPZ_SetNVIC( 3, 2, USART2_IRQn );

	/* 使能 */
	USART2->CR1 |= 1 << 13;
}

void USART2_IRQHandler( void )
{
	if ( USART2->SR & ( 1 << 5 ) )
	{
		( void )USART2->DR;
		//USART2->SR &= ~( 1 << 5 );
	}
}



