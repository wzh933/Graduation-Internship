#include "stm32f407xx.h"
#include "sys.h"


static unsigned char g_group_setup = 2;


void DPZ_SoftReset( void )
{
	__DSB();

	SCB->AIRCR = 0x05FA0004;

	__DSB();

	while ( 1 );
}

// 参数为平等级位数
void DPZ_NVIC_Init( unsigned char Sub )
{
	// 中断向量位于FLASH        STM32是0x08000000
	SCB->VTOR = 0x08000000;

	// 下面三项 分别为 清除原数据 写入钥匙 写入本次分组形式数据
	SCB->AIRCR = ( SCB->AIRCR & 0x0000F8FF ) | 0x05FA0000 | ( ( unsigned int )( 7 - __NVIC_PRIO_BITS + Sub ) << 8 );
	g_group_setup = Sub;
}

void DPZ_SetNVIC( unsigned char Preemption, unsigned char Sub, int Channel )
{
	register unsigned char premsk, submsk;

	premsk = ( 1 << ( __NVIC_PRIO_BITS - g_group_setup ) ) - 1;
	submsk = ( 1 << g_group_setup ) - 1;

	if ( Channel >= 0 )
	{
		NVIC->ISER[Channel >> 5] |= 1 << ( Channel & 0x1F );
		NVIC->IP[Channel] = ( ( Preemption & premsk ) << g_group_setup | ( Sub & submsk ) ) << ( 8 - __NVIC_PRIO_BITS );
	}
	else
	{
		SCB->SHP[12 + Channel] = ( ( Preemption & premsk ) << 2 | ( Sub & submsk ) ) << ( 8 - __NVIC_PRIO_BITS );
	}
}


void DPZ_ClearNVIC( int Channel )
{
	NVIC->ICER[Channel >> 5] |= 1 << ( Channel & 0x1F );
}

void DPZ_PendingIRQ( int Channel, int flag )
{
	if ( flag )
	{
		NVIC->ISPR[Channel >> 5] |= 1 << ( Channel & 0x1F );
	}
	else
	{
		NVIC->ICPR[Channel >> 5] |= 1 << ( Channel & 0x1F );
	}
}








