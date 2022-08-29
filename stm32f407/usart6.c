#include "stm32f407xx.h"
#include "utility.h"
#include "gpio.h"
#include "sys.h"
#include "usart6.h"


void DPZ_USART6_Init( unsigned int baud, unsigned int remap )
{
	unsigned int tmp;
	unsigned short mantissa;


	if ( remap == 0 )
	{
		/* 使能PC PC6(TX)推挽输出50M PC7(RX)上拉输入 */
		RCC->AHB1ENR |= 1 << 2;
		DPZ_IO_H( GPIOC, 6 );
		DPZ_IO_CFG( GPIOC, 6, 2, 0, 2, 1, 8 );
		DPZ_IO_CFG( GPIOC, 7, 2, 0, 2, 1, 8 );
	}
	else
	{
		/* PG14 PG9 */
		RCC->AHB1ENR |= 1 << 6;
		DPZ_IO_H( GPIOG, 14 );
		DPZ_IO_CFG( GPIOG, 14, 2, 0, 2, 1, 8 );
		DPZ_IO_CFG( GPIOG,  9, 2, 0, 2, 1, 8 );
	}


	/* 16次采样 */
	tmp = baud << 4;
	if ( g_pclk2 <= tmp )
	{
		return;
	}

	mantissa = g_pclk2 / tmp;
	mantissa =  mantissa << 4 | ( g_pclk2 - mantissa * tmp + ( baud >> 1 ) ) / baud;

	/* 首先禁止USART6 */
	USART6->CR1 &= ~( 1 << 13 );

	/* 使能USART6时钟 */
	RCC->APB2ENR |= 1 << 5;

	/* 重启USART6 */
	RCC->APB2RSTR |= 1 << 5;
	RCC->APB2RSTR &= ~( 1 << 5 );

	/* 置BAUD率 */
	USART6->BRR = mantissa;
	/* 一个停止位 */
	USART6->CR2 &= ~( 3 << 12 );
	/* 8个数据位 无奇偶校验 */
	USART6->CR1 &= ~( 1 << 12 | 1 << 10 );
	/* 允许发送接收 */
	/* 开奇偶校验(PE)中断 他们大牛都说STM32的串口有BUG 此处必须这样 */
	/* 开接收区不空中断(RXNEIE) 也就是有数据就中断(包括过载 过载就是数据有了还来 多了) */
	USART6->CR1 |= 3 << 2 | 1 << 8 | 1 << 5;
	/* USART 16次采样 */
	USART6->CR1 &= ~( 1 << 15 );

	/* 两个优先级各2位  3 3 全是最低
	   通道为USART6_IRQn 对应处理函数为USART6_IRQHandler  在startup的汇编文件中 */
	DPZ_SetNVIC( 3, 3, USART6_IRQn );

	/* 使能 */
	USART6->CR1 |= 1 << 13;
}

void USART6_IRQHandler( void )
{
	if ( USART6->SR & ( 1 << 5 ) )
	{
		( void )USART6->DR;
		//USART6->SR &= ~( 1 << 5 );
	}
}


