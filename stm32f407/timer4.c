#include "stm32f407xx.h"
#include "utility.h"
#include "gpio.h"
#include "sys.h"
#include "timer4.h"

/*
The timer clock frequencies are automatically set by hardware. There are two cases:
1. If the APB prescaler is 1, the timer clock frequencies are set to the same frequency as
that of the APB domain to which the timers are connected.
2. Otherwise, they are set to twice (×2) the frequency of the APB domain to which the
timers are connected.
*/
void DPZ_Timer4_PWM( unsigned short arr, unsigned short psc )
{
	/* 开启GPIOA时钟 */
	RCC->AHB1ENR |= 1 << 0;
	/* 复用功能上拉输出 */
	DPZ_IO_CFG( GPIOA, 15, 2, 0, 3, 0, 2 );


	/* 打开TIMER4时钟 */
	RCC->APB1ENR |= 1 << 2;
	/* 关闭定时器 */
	TIM4->CR1 = 0;
	/* 自动载入值 */
	TIM4->ARR = arr - 1;
	/* 分频寄存器 */
	TIM4->PSC = psc - 1;
	/* 使能ARPE */
	TIM4->CR1 |= 1 << 7;
	/* 清空计数器 */
	TIM4->CNT = 0;


	/* OC4 输出禁止 */
	TIM4->CCER &= ~( 1 << 12 );
	/* CH4 PWM1模式 */
	DPZ_MOD_BIT( TIM4->CCMR2, 12, 0x07, 6 );
	/* OC4 配置为输出 */
	DPZ_MOD_BIT( TIM4->CCMR2, 8, 0x03, 0 );
	/* OC4 预加载使能 */
	TIM4->CCMR2 |= 1 << 11;
	/* OC4 低电平有效 */
	TIM4->CCER |= 1 << 13;
	/* 设置比较值 50% */
	TIM4->CCR4 = arr >> 1;
	/* OC4 输出使能 */
	TIM4->CCER |= 1 << 12;


	/* 打开定时器 */
	TIM4->CR1 |= 1 << 0;
}


void DPZ_Timer4_Restart( unsigned short arr )
{
	TIM4->ARR = arr - 1;
	TIM4->CCR4 = arr >> 1;
}
