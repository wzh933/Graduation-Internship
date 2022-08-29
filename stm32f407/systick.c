#include "stm32f407xx.h"
#include "utility.h"
#include "systick.h"


static volatile unsigned char g_self = 0;

void DPZ_SysTick_US( unsigned int usecs )
{
	register unsigned int load;

	/* 清标志 关闭SYSTICK */
	SysTick->CTRL = 0;

	load = usecs * ( g_hclk / 1000000 ) - 1;
	if ( load & 0xFF000000 )
	{
		load = 0x00FFFFFF;
	}

	/* 将总TICKS装入LOAD寄存器 */
	SysTick->LOAD = load;

	/* 清计数 */
	SysTick->VAL = 0;

	/* 选取频率为HCLK频率 允许中断 启动 */
	SysTick->CTRL |= 1 << 2 | 1 << 1 | 1 << 0;

	/* WFI */
	for ( g_self = 0; !g_self; )
	{
		__asm( "WFI" );
	}

	/* 关闭SYSTICK */
	SysTick->CTRL = 0;
}

void DPZ_SysTick_MS( unsigned int msecs )
{
	register unsigned int load;

	/* 清标志 关闭SYSTICK */
	SysTick->CTRL = 0;

	load = msecs * ( g_hclk / 8000 ) - 1;
	if ( load & 0xFF000000 )
	{
		load = 0x00FFFFFF;
	}

	/* 将总TICKS装入LOAD寄存器 */
	SysTick->LOAD = load;

	/* 清计数 */
	SysTick->VAL = 0;

	/* 选取频率为HCLK/8频率  */
	//SysTick->CTRL &= ~( 1 << 2 );

	/* 允许中断 启动 */
	SysTick->CTRL |= 1 << 1 | 1 << 0;

	/* WFI */
	for ( g_self = 0; !g_self; )
	{
		__asm( "WFI" );
	}

	/* 关闭SYSTICK */
	SysTick->CTRL = 0;
}

void SysTick_Handler( void )
{
	g_self = 1;
}


/* 最高主频168M  */
void DPZ_SysTick_Sleep( unsigned int msecs )
{
	register unsigned int i, n;

	n = msecs >> 9;

	for ( i = 0; i < n; i ++ )
	{
		DPZ_SysTick_MS( 512 );
	}

	DPZ_SysTick_MS( msecs & 511 );
}






