#include "stm32f407xx.h"
#include "utility.h"
#include "sys.h"
#include "timer14.h"

static volatile int g_self = 0;

/*
The timer clock frequencies are automatically set by hardware. There are two cases:
1. If the APB prescaler is 1, the timer clock frequencies are set to the same frequency as
that of the APB domain to which the timers are connected.
2. Otherwise, they are set to twice (×2) the frequency of the APB domain to which the
timers are connected.
*/
void DPZ_Timer14_MS( unsigned int msecs )
{
	/* 打开TIMER14时钟 */
	RCC->APB1ENR |= 1 << 8;
	/* 关闭定时器 清空原配置(默认 不产生事件 向上计数 不用预加载) */
	TIM14->CR1 = 0;
	/* 自动载入值 */
	TIM14->ARR = ( msecs << 2 ) - 1;
	/* 分频寄存器 */
	if ( g_pclk1_pscr > 1 )
	{
		TIM14->PSC = g_pclk1 / 2000 - 1;
	}
	else
	{
		TIM14->PSC = g_pclk1 / 4000 - 1;
	}
	/* 清空计数器 */
	TIM14->CNT = 0;
	/* 开更新中断 */
	TIM14->DIER |= 1 << 0;
	/* 挂接NVIC */
	DPZ_SetNVIC( 15, 0, TIM8_TRG_COM_TIM14_IRQn );
	/* 打开定时器 */
	TIM14->CR1 |= 1 << 0;

	for ( g_self = 0; !g_self; )
	{
		__WFI();
	}

	/* 关闭定时器 */
	TIM14->CR1 &= ~( 1 << 0 );

	/* 关闭TIMER14时钟 */
	//RCC->APB1ENR &= ~( 1 << 8 );
}



void TIM8_TRG_COM_TIM14_IRQHandler( void )
{
	if ( TIM14->SR & ( 1 << 0 ) )
	{
		g_self = 1;
	}
	TIM14->SR &= ~( 1 << 0 );
}


/* 最高主频168M  */
void DPZ_Timer14_Sleep( unsigned int msecs )
{
	register unsigned int i, n;

	n = msecs >> 14;

	for ( i = 0; i < n; i ++ )
	{
		DPZ_Timer14_MS( 1 << 14 );
	}

	DPZ_Timer14_MS( msecs & ( ( 1 << 14 ) - 1 ) );
}

