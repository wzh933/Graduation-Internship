#include "stm32f407xx.h"
#include "utility.h"
#include "gpio.h"
#include "sys.h"
#include "uart4.h"


void DPZ_UART4_Init( unsigned int baud, unsigned int remap )
{
	unsigned int tmp;
	unsigned short mantissa;

	tmp = baud << 4;
	if ( g_pclk1 <= tmp )
	{
		return;
	}

	mantissa = g_pclk1 / tmp;
	mantissa =  mantissa << 4 | ( g_pclk1 - mantissa * tmp + ( baud >> 1 ) ) / baud;


	/* 首先禁止UART4 */
	UART4->CR1 &= ~( 1 << 13 );

	/* 使能UART4时钟 */
	RCC->APB1ENR |= 1 << 19;

	if ( remap )
	{
		/* 使能PA PA0(TX)推挽输出50M PA1(RX)上拉输入 */
		RCC->AHB1ENR |= 1 << 0;
		DPZ_IO_H( GPIOA, 0 );
		DPZ_IO_CFG( GPIOA, 0, 2, 0, 2, 1, 8 );
		DPZ_IO_CFG( GPIOA, 1, 2, 0, 2, 1, 8 );
	}
	else
	{
		/* 重映射PC10 PC11 */
		RCC->AHB1ENR |= 1 << 2;
		DPZ_IO_H( GPIOC, 10 );
		DPZ_IO_CFG( GPIOC, 10, 2, 0, 2, 1, 8 );
		DPZ_IO_CFG( GPIOC, 11, 2, 0, 2, 1, 8 );
	}


	/* 重启UART4 */
	RCC->APB1RSTR |= 1 << 19;
	RCC->APB1RSTR &= ~( 1 << 19 );

	/* 置BAUD率 */
	UART4->BRR = mantissa;
	/* 一个停止位 */
	UART4->CR2 &= ~( 3 << 12 );
	/* 8个数据位 无奇偶校验 */
	UART4->CR1 &= ~( 1 << 12 | 1 << 10 );
	/* 允许发送接收 */
	/* 开奇偶校验(PE)中断 他们大牛都说STM32的串口有BUG 此处必须这样 */
	/* 开接收区不空中断(RXNEIE) 也就是有数据就中断(包括过载 过载就是数据有了还来 多了) */
	UART4->CR1 |= 3 << 2 | 1 << 8 | 1 << 5;
	/* USART 16次采样 */
	UART4->CR1 &= ~( 1 << 15 );

	/* 两个优先级各2位  3 1 比串口1和串口2都高
	   通道为UART4_IRQn 对应处理函数为UART4_IRQHandler  在startup的汇编文件中 */
	DPZ_SetNVIC( 3, 1, UART4_IRQn );

	/* 使能 */
	UART4->CR1 |= 1 << 13;
}

void UART4_IRQHandler( void )
{
	if ( UART4->SR & ( 1 << 5 ) )
	{
		( void )UART4->DR;
		//UART4->SR &= ~( 1 << 5 );
	}
}


