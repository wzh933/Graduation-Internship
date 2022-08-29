#include "stm32f407xx.h"
#include "utility.h"
#include "gpio.h"
#include "sys.h"
#include "usart3.h"


void DPZ_USART3_Init( unsigned int baud, unsigned int remap )
{
	unsigned int tmp;
	unsigned short mantissa;

	if ( remap == 1 )
	{
		/* 重映射PC10 PC11 */
		RCC->AHB1ENR |= 1 << 2;
		DPZ_IO_H( GPIOC, 10 );
		DPZ_IO_CFG( GPIOC, 10, 2, 0, 2, 1, 7 );
		DPZ_IO_CFG( GPIOC, 11, 2, 0, 2, 1, 7 );

	}
	else if ( remap == 2 )
	{
		/* 重映射PD8 PD9 */
		RCC->AHB1ENR |= 1 << 3;
		DPZ_IO_H( GPIOD, 8 );
		DPZ_IO_CFG( GPIOD, 8, 2, 0, 2, 1, 7 );
		DPZ_IO_CFG( GPIOD, 9, 2, 0, 2, 1, 7 );
	}
	else
	{
		/* 使能PB PB10(TX)推挽输出50M PB11(RX)上拉输入 */
		RCC->AHB1ENR |= 1 << 1;
		DPZ_IO_H( GPIOB, 10 );
		DPZ_IO_CFG( GPIOB, 10, 2, 0, 2, 1, 7 );
		DPZ_IO_CFG( GPIOB, 11, 2, 0, 2, 1, 7 );
	}

	tmp = baud << 4;
	if ( g_pclk1 <= tmp )
	{
		return;
	}

	mantissa = g_pclk1 / tmp;
	mantissa =  mantissa << 4 | ( g_pclk1 - mantissa * tmp + ( baud >> 1 ) ) / baud;

	/* 首先禁止USART3 */
	USART3->CR1 &= ~( 1 << 13 );

	/* 使能USART3时钟 */
	RCC->APB1ENR |= 1 << 18;

	/* 重启USART3 */
	RCC->APB1RSTR |= 1 << 18;
	RCC->APB1RSTR &= ~( 1 << 18 );

	/* 置BAUD率 */
	USART3->BRR = mantissa;
	/* 一个停止位 */
	USART3->CR2 &= ~( 3 << 12 );
	/* 8个数据位 无奇偶校验 */
	USART3->CR1 &= ~( 1 << 12 | 1 << 10 );
	/* 允许发送接收 */
	/* 开奇偶校验(PE)中断 他们大牛都说STM32的串口有BUG 此处必须这样 */
	/* 开接收区不空中断(RXNEIE) 也就是有数据就中断(包括过载 过载就是数据有了还来 多了) */
	USART3->CR1 |= 3 << 2 | 1 << 8 | 1 << 5;
	/* USART 16次采样 */
	USART3->CR1 &= ~( 1 << 15 );

	/* 两个优先级各2位  3 1 比串口1和串口2都高
	   通道为USART3_IRQn 对应处理函数为USART3_IRQHandler  在startup的汇编文件中 */
	DPZ_SetNVIC( 3, 1, USART3_IRQn );

	/* 使能 */
	USART3->CR1 |= 1 << 13;
}

void USART3_IRQHandler( void )
{
	if ( USART3->SR & ( 1 << 5 ) )
	{
		( void )USART3->DR;
		//USART3->SR &= ~( 1 << 5 );
	}
}

