#include "stm32f407xx.h"
#include "utility.h"
#include "gpio.h"
#include "sys.h"
#include "uart5.h"


void DPZ_UART5_Init( unsigned int baud )
{
	unsigned int tmp;
	unsigned short mantissa;


	/* 使能PC PC12(TX)推挽输出50M PD2(RX)上拉输入 */
	RCC->AHB1ENR |= 1 << 2 | 1 << 3;
	DPZ_IO_H( GPIOC, 12 );
	DPZ_IO_CFG( GPIOC, 12, 2, 0, 2, 1, 8 );
	DPZ_IO_CFG( GPIOD, 2, 2, 0, 2, 1, 8 );

	tmp = baud << 4;
	if ( g_pclk1 <= tmp )
	{
		return;
	}

	mantissa = g_pclk1 / tmp;
	mantissa =  mantissa << 4 | ( g_pclk1 - mantissa * tmp + ( baud >> 1 ) ) / baud;


	/* 首先禁止UART5 */
	UART5->CR1 &= ~( 1 << 13 );

	/* 使能UART5时钟 */
	RCC->APB1ENR |= 1 << 20;

	/* 重启UART5 */
	RCC->APB1RSTR |= 1 << 20;
	RCC->APB1RSTR &= ~( 1 << 20 );

	/* 置BAUD率 */
	UART5->BRR = mantissa;
	/* 一个停止位 */
	UART5->CR2 &= ~( 3 << 12 );
	/* 8个数据位 无奇偶校验 */
	UART5->CR1 &= ~( 1 << 12 | 1 << 10 );
	/* 允许发送接收 */
	/* 开接收区不空中断(RXNEIE) 也就是有数据就中断(包括过载 过载就是数据有了还来 多了) */
	UART5->CR1 |= 3 << 2 | 1 << 8 | 1 << 5;
	/* USART 16次采样 */
	UART5->CR1 &= ~( 1 << 15 );

	/* 两个优先级各2位  3 1 比串口1和串口2都高
	   通道为UART5_IRQn 对应处理函数为UART5_IRQHandler  在startup的汇编文件中 */
	DPZ_SetNVIC( 3, 1, UART5_IRQn );

	/* 使能 */
	UART5->CR1 |= 1 << 13;
}

void UART5_IRQHandler( void )
{
	if ( UART5->SR & ( 1 << 5 ) )
	{
		( void )UART5->DR;
		//UART5->SR &= ~( 1 << 5 );
	}
}



