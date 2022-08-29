#include "stm32f407xx.h"
#include "utility.h"
#include "gpio.h"
#include "sys.h"
#include "timer2.h"



static volatile int g_self = 0;


/* 32位周期定时器 */
void DPZ_Timer2_Delay( unsigned int msec )
{
	/* 打开TIMER2时钟 */
	RCC->APB1ENR |= 1 << 0;
	/* 关闭定时器 */
	TIM2->CR1 = 0;
	/* 自动载入值 */
	TIM2->ARR = msec * 10 - 1;
	/* 分频寄存器 10K */
	if ( g_pclk1_pscr > 1 )
	{
		TIM2->PSC = g_pclk1 / 5000 - 1;
	}
	else
	{
		TIM2->PSC = g_pclk1 / 10000 - 1;
	}
	/* 使能ARPE */
	TIM2->CR1 |= 1 << 7;
	/* 清空计数器 */
	TIM2->CNT = 0;
	/* 开更新中断 */
	TIM2->DIER |= 1 << 0;
	/* 挂接NVIC */
	DPZ_SetNVIC( 13, 1, TIM2_IRQn );
	/* 打开定时器 */
	TIM2->CR1 |= 1 << 0;

	/* 等待中断唤醒 */
	for ( g_self = 0; !g_self; )
	{
		__WFI();
	}

	/* 关闭定时器 */
	TIM2->CR1 &= ~( 1 << 0 );

	/* 关闭TIMER2时钟 */
	//RCC->APB1ENR &= ~( 1 << 0 );
}

void TIM2_IRQHandler( void )
{
	if ( TIM2->SR & ( 1 << 0 ) )
	{
		g_self = 1;
	}
	TIM2->SR &= ~( 1 << 0 );
}


