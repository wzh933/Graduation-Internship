#include "stm32f407xx.h"
#include "utility.h"
#include "gpio.h"
#include "sys.h"
#include "timer1.h"


/*
The timer clock frequencies are automatically set by hardware. There are two cases:
1. If the APB prescaler is 1, the timer clock frequencies are set to the same frequency as
that of the APB domain to which the timers are connected.
2. Otherwise, they are set to twice (×2) the frequency of the APB domain to which the
timers are connected.
*/
void DPZ_Timer1_PWM( unsigned short arr, unsigned short psc )
{
	/* 开启GPIOE时钟 */
	RCC->AHB1ENR |= 1 << 4;
	/* 复用功能上拉输出 */
	DPZ_IO_CFG( GPIOE, 13, 2, 0, 3, 0, 1 );
	DPZ_IO_CFG( GPIOE, 14, 2, 0, 3, 0, 1 );


	/* 打开TIMER1时钟 */
	RCC->APB2ENR |= 1 << 0;

	/* 关闭定时器 */
	TIM1->CR1 = 0;
	/* 自动载入值 */
	TIM1->ARR = arr - 1;
	/* 分频寄存器 */
	TIM1->PSC = psc - 1;
	/* 使能ARPE */
	TIM1->CR1 |= 1 << 7;
	/* 清空计数器 */
	TIM1->CNT = 0;

	/* OC3 输出禁止 */
	TIM1->CCER &= ~( 1 << 8 );
	/* CH3 PWM1模式 */
	DPZ_MOD_BIT( TIM1->CCMR2, 4, 0x07, 6 );
	/* OC3 配置为输出 */
	DPZ_MOD_BIT( TIM1->CCMR2, 0, 0x03, 0 );
	/* OC3 预加载使能 */
	TIM1->CCMR2 |= 1 << 3;
	/* OC3 低电平有效 */
	TIM1->CCER |= 1 << 9;
	/* OC3 输出使能 */
	TIM1->CCER |= 1 << 8;


	/* OC4 输出禁止 */
	TIM1->CCER &= ~( 1 << 12 );
	/* CH4 PWM1模式 */
	DPZ_MOD_BIT( TIM1->CCMR2, 12, 0x07, 6 );
	/* OC4 配置为输出 */
	DPZ_MOD_BIT( TIM1->CCMR2, 8, 0x03, 0 );
	/* OC4 预加载使能 */
	TIM1->CCMR2 |= 1 << 11;
	/* OC4 高电平有效 */
	TIM1->CCER |= 1 << 13;
	/* OC4 输出使能 */
	TIM1->CCER |= 1 << 12;



	/* 主输出使能 可软件配置 */
	TIM1->BDTR |= 1 << 15 | 1 << 14;

	/* 打开定时器 */
	TIM1->CR1 |= 1 << 0;
}


void DPZ_Timer1_CCR( unsigned short ccr )
{
	TIM1->CCR3 = ccr;
	TIM1->CCR4 = 201 - ccr;
}

