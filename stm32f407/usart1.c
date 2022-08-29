#include "stm32f407xx.h"
#include "utility.h"
#include "gpio.h"
#include "sys.h"
#include "usart1.h"

//debug
void DPZ_USART1_Init( unsigned int baud, unsigned int remap )
{
	unsigned int tmp;
	unsigned short mantissa;

	if ( remap == 0 )
	{
		/* 使能PA PA9(TX)推挽输出50M PA10(RX)上拉输入 */
		RCC->AHB1ENR |= 1 << 0;
		DPZ_IO_H( GPIOA, 9 );
		DPZ_IO_CFG( GPIOA,  9, 2, 0, 2, 1, 7 );
		DPZ_IO_CFG( GPIOA, 10, 2, 0, 2, 1, 7 );
	}
	else
	{
		/* 重映射PB6 PB7 */
		RCC->AHB1ENR |= 1 << 1;
		DPZ_IO_H( GPIOB, 6 );
		DPZ_IO_CFG( GPIOB, 6, 2, 0, 2, 1, 7 );
		DPZ_IO_CFG( GPIOB, 7, 2, 0, 2, 1, 7 );
	}

	/* 16次采样 */
	tmp = baud << 4;
	if ( g_pclk2 <= tmp )
	{
		return;
	}

	mantissa = g_pclk2 / tmp;
	mantissa =  mantissa << 4 | ( g_pclk2 - mantissa * tmp + ( baud >> 1 ) ) / baud;

	/* 首先禁止USART1 */
	USART1->CR1 &= ~( 1 << 13 );

	/* 使能USART1时钟 */
	RCC->APB2ENR |= 1 << 4;

	/* 重启USART1 */
	RCC->APB2RSTR |= 1 << 4;
	RCC->APB2RSTR &= ~( 1 << 4 );

	/* 置BAUD率 */
	USART1->BRR = mantissa;
	/* 一个停止位 */
	USART1->CR2 &= ~( 3 << 12 );
	/* 8个数据位 无奇偶校验 */
	USART1->CR1 &= ~( 1 << 12 | 1 << 10 );
	/* 允许发送接收 */
	/* 开接收区不空中断(RXNEIE) 也就是有数据就中断(包括过载 过载就是数据有了还来 多了) */
	USART1->CR1 |= 3 << 2 | 1 << 8 | 1 << 5;
	/* USART 16次采样 */
	USART1->CR1 &= ~( 1 << 15 );

	/* 两个优先级各2位  3 3 全是最低
	   通道为USART1_IRQn 对应处理函数为USART1_IRQHandler  在startup的汇编文件中 */
	DPZ_SetNVIC( 3, 3, USART1_IRQn );

	/* 使能 */
	USART1->CR1 |= 1 << 13;
}


void USART1_IRQHandler( void )
{
	if ( USART1->SR & ( 1 << 5 ) )
	{
		( void )USART1->DR;
		//USART1->SR &= ~( 1 << 5 );
	}
}


