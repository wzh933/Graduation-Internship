#include "stm32f407xx.h"
#include "utility.h"
#include "gpio.h"
#include "sys.h"
#include "timer13.h"

static volatile int g_self = 0;

/*
The timer clock frequencies are automatically set by hardware. There are two cases:
1. If the APB prescaler is 1, the timer clock frequencies are set to the same frequency as
that of the APB domain to which the timers are connected.
2. Otherwise, they are set to twice (×2) the frequency of the APB domain to which the
timers are connected.
*/
/* 基础频率100K */
void DPZ_Timer13_PWM( unsigned short arr )
{
	/* 开启GPIOA时钟 */
	RCC->AHB1ENR |= 1 << 0;
	/* 复用功能上拉输出 */
	DPZ_IO_CFG( GPIOA, 6, 2, 0, 3, 0, 9 );

	/* 打开TIMER13时钟 */
	RCC->APB1ENR |= 1 << 7;
	/* 关闭定时器 */
	TIM13->CR1 = 0;
	/* 自动载入值 */
	TIM13->ARR = arr - 1;
	/* 分频寄存器 100K */
	if ( g_pclk1_pscr > 1 )
	{
		TIM13->PSC = g_pclk1 / 50000 - 1;
	}
	else
	{
		TIM13->PSC = g_pclk1 / 100000 - 1;
	}
	/* 使能ARPE */
	TIM13->CR1 |= 1 << 7;
	/* 清空计数器 */
	TIM13->CNT = 0;


	/* OC1 输出禁止 */
	TIM13->CCER &= ~( 1 << 0 );
	/* CH1 PWM1模式 */
	DPZ_MOD_BIT( TIM13->CCMR1, 4, 0x07, 6 );
	/* OC1 配置为输出 */
	DPZ_MOD_BIT( TIM13->CCMR1, 0, 0x03, 0 );
	/* OC1 预加载使能 */
	TIM13->CCMR1 |= 1 << 3;
	/* OC1 低电平有效 */
	TIM13->CCER |= 1 << 1;
	/* 设置比较值 50% */
	TIM13->CCR1 = arr >> 1;
	/* OC4 输出使能 */
	TIM13->CCER |= 1 << 0;

	/* 打开定时器 */
	TIM13->CR1 |= 1 << 0;
}


void DPZ_Timer13_Restart( unsigned short arr )
{
	TIM13->ARR = arr - 1;
	TIM13->CCR1 = arr >> 1;
}

void DPZ_Timer13_CCR( unsigned short ccr )
{
	TIM13->CCR1 = ccr;
}

