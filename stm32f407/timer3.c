#include "stm32f407xx.h"
#include "utility.h"
#include "sys.h"
#include "timer3.h"

static volatile int g_self = 0;

/*
The timer clock frequencies are automatically set by hardware. There are two cases:
1. If the APB prescaler is 1, the timer clock frequencies are set to the same frequency as
that of the APB domain to which the timers are connected.
2. Otherwise, they are set to twice (×2) the frequency of the APB domain to which the
timers are connected.
*/
void DPZ_Timer3_Delay( unsigned int msecs )
{
	/* 打开TIMER3时钟 */
	RCC->APB1ENR |= 1 << 1;
	/* 关闭定时器 清空原配置(默认 不产生事件 向上计数 不用预加载) */
	TIM3->CR1 = 0;
	/* 自动载入值 */
	TIM3->ARR = ( msecs << 2 ) - 1;
	/* 分频寄存器 */
	if ( g_pclk1_pscr > 1 )
	{
		TIM3->PSC = g_pclk1 / 2000 - 1;
	}
	else
	{
		TIM3->PSC = g_pclk1 / 4000 - 1;
	}
	/* 清空计数器 */
	TIM3->CNT = 0;
	/* 开更新中断 */
	TIM3->DIER |= 1 << 0;
	/* 挂接NVIC */
	DPZ_SetNVIC( 5, 0, TIM3_IRQn );
	/* 打开定时器 */
	TIM3->CR1 |= 1 << 0;

	for ( g_self = 0; !g_self; )
	{
		__WFI();
	}

	/* 关闭定时器 */
	TIM3->CR1 &= ~( 1 << 0 );

	/* 关闭TIMER3时钟 */
	RCC->APB1ENR &= ~( 1 << 1 );
}




void TIM3_IRQHandler( void )
{
	if ( TIM3->SR & ( 1 << 0 ) )
	{
		g_self = 1;
	}
	TIM3->SR &= ~( 1 << 0 );
}


