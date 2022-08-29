#include "stm32f407xx.h"
#include "utility.h"
#include "clk.h"



static const unsigned int _hclk_tbl[] = { 1, 1, 1, 1, 1, 1, 1, 1, 2, 4, 8, 16, 64, 128, 256, 512 };
static const unsigned int _pclk_tbl[] = { 1, 1, 1, 1, 2, 4, 8, 16 };


// 时钟设置函数
// Fvco=Fs*(plln/pllm);
// Fsys=Fvco/pllp=Fs*(plln/(pllm*pllp));
// Fusb=Fvco/pllq=Fs*(plln/(pllm*pllq));

// Fvco:VCO频率
// Fsys:系统时钟频率
// Fusb:USB,SDIO,RNG等的时钟频率
// Fs:PLL输入时钟频率,可以是HSI,HSE等.
// plln:主PLL倍频系数(PLL倍频),取值范围:64~432.
// pllm:主PLL和音频PLL分频系数(PLL之前的分频),取值范围:2~63.
// pllp:系统时钟的主PLL分频系数(PLL之后的分频),取值范围:2,4,6,8.(仅限这4个值!)
// pllq:USB/SDIO/随机数产生器等的主PLL分频系数(PLL之后的分频),取值范围:2~15.

// 外部晶振为8M的时候,推荐值:plln=336,pllm=8,pllp=2,pllq=7.
// 得到:Fvco=8*(336/8)=336MHz
// Fsys=336/2=168MHz
// Fusb=336/7=48MHz

// 内部RC为16M,推荐值:plln=168,pllm=8,pllp=2,pllq=7.
// 得到:Fvco=16*(168/8)=336MHz
// Fsys=336/2=168MHz
// Fusb=336/7=48MHz


// 外部晶振为25M的时候,推荐值:plln=336,pllm=25,pllp=2,pllq=7.
// 得到:Fvco=25*(336/25)=336MHz
// Fsys=336/2=168MHz
// Fusb=336/7=48MHz


// 407 HSI默认16M
void DPZ_SysHSI( unsigned char AHBDiv, unsigned char APB1Div, unsigned char APB2Div )
{
	/* 关闭AHB总线上所有时钟 */
	RCC->AHB1ENR = 0x00100000;
	RCC->AHB2ENR = 0x00000000;
	RCC->AHB3ENR = 0x00000000;
	/* 关闭APB1 APB2所有外设时钟 */
	RCC->APB2ENR = 0x00000000;
	RCC->APB1ENR = 0x00000000;

	// 电压范围 均按照3.3V 高性能模式
	RCC->APB1ENR |= 1 << 28;
	PWR->CR |= 1 << 14;

	// 开启FLASH预取 数据 指令缓存 等待5周期(安全)
	FLASH->ACR |= 7 << 8;
	DPZ_MOD_BIT( FLASH->ACR, 0, 0x07, 5 );


	// 禁止所有时钟中断
	RCC->CIR = 0;

	if ( !( RCC->CR & ( 1 << 1 ) ) )
	{
		// 开启HSI
		RCC->CR |= 1 << 0;

		// 等待HSI稳定
		while ( !( RCC->CR & ( 1 << 1 ) ) )
		{
			Delay_04NOP();
		}
	}

	if ( ( RCC->CFGR & ( 3 << 2 ) ) != ( 0 << 2 ) )
	{
		/* 切换HSI为系统时钟 */
		DPZ_MOD_BIT( RCC->CFGR, 0, 0x03, 0 );

		/* 等待切换完成 */
		while (	( RCC->CFGR & ( 3 << 2 ) ) != ( 0 << 2 ) )
		{
			Delay_04NOP();
		}
	}

	// 设置Div 同时清除 MCO1 MCO2 I2S RTC
	RCC->CFGR = ( RCC->CFGR & 0x0000030F ) | ( ( unsigned int )AHBDiv & 0x0F ) << 4 | ( ( unsigned int )APB1Div & 0x07 ) << 10 | ( ( unsigned int )APB2Div & 0x07 ) << 13;
	Delay_04NOP();

	// 关闭其它时钟   HSEON, CSSON and PLLON PLLI2S
	RCC->CR &= ~( 1 << 16 | 1 << 19 | 1 << 24 | 1 << 26 );
	// 关闭 LSION
	RCC->CSR &= ~( 1 << 0 );
	// 关闭HSEON后 才能关闭HSEBYP
	RCC->CR &= ~( 1 << 18 );

	// 关闭PLLON后 才能重置 PLLCFGR
	RCC->PLLCFGR = 0x24003010;

	// 关闭PLLI2SON后 才能重置 PLLI2SCFGR
	RCC->PLLI2SCFGR = 0x20003000;

	g_osc = 16000000;
	g_sysclk = g_osc;
	g_hclk = g_sysclk / _hclk_tbl[AHBDiv];
	g_pclk1_pscr = _pclk_tbl[APB1Div];
	g_pclk1 = g_hclk / g_pclk1_pscr;
	g_pclk2_pscr = _pclk_tbl[APB2Div];
	g_pclk2 = g_hclk / g_pclk2_pscr;

	// 重置电压范围与FLASH等待时间
	PWR->CR &= ~( 1 << 14 );
	//RCC->APB1ENR &= ~( 1 << 28 );
	DPZ_MOD_BIT( FLASH->ACR, 0, 0x07, 0 );
}

// 默认8000000
void DPZ_SysHSE( unsigned char AHBDiv, unsigned char APB1Div, unsigned char APB2Div )
{
	/* AHB总线上所有时钟使能 恢复到初始状态 */
	RCC->AHB1ENR = 0x00100000;
	RCC->AHB2ENR = 0x00000000;
	RCC->AHB3ENR = 0x00000000;
	/* 关闭APB1 APB2所有外设时钟 */
	RCC->APB2ENR = 0x00000000;
	RCC->APB1ENR = 0x00000000;

	// 电压范围 均按照3.3V 高性能模式
	RCC->APB1ENR |= 1 << 28;
	PWR->CR |= 1 << 14;

	// 开启FLASH预取 数据 指令缓存 等待5周期(安全)
	FLASH->ACR |= 7 << 8;
	DPZ_MOD_BIT( FLASH->ACR, 0, 0x07, 5 );

	// 禁止所有时钟中断
	RCC->CIR = 0;

	if ( !( RCC->CR & ( 1 << 17 ) ) )
	{
		// 开启HSE
		RCC->CR |= 1 << 16;

		// 等待HSE稳定
		while ( !( RCC->CR & ( 1 << 17 ) ) )
		{
			Delay_04NOP();
		}
	}

	if ( ( RCC->CFGR & ( 3 << 2 ) ) != ( 1 << 2 ) )
	{
		/* 切换HSE为系统时钟 */
		DPZ_MOD_BIT( RCC->CFGR, 0, 0x03, 1 );

		/* 等待切换完成 */
		while (	( RCC->CFGR & ( 3 << 2 ) ) != ( 1 << 2 ) )
		{
			Delay_04NOP();
		}
	}

	// 设置Div 同时清除 MCO1 MCO2 I2S RTC
	RCC->CFGR = ( RCC->CFGR & 0x0000030F ) | ( ( unsigned int )AHBDiv & 0x0F ) << 4 | ( ( unsigned int )APB1Div & 0x07 ) << 10 | ( ( unsigned int )APB2Div & 0x07 ) << 13;
	Delay_04NOP();

	// 关闭其它时钟   HSION, CSSON and PLLON PLLI2S
	RCC->CR &= ~( 1 << 0 | 1 << 19 | 1 << 24 | 1 << 26 );
	// 关闭 LSION
	RCC->CSR &= ~( 1 << 0 );

	// 关闭PLLON后 才能重置 PLLCFGR
	RCC->PLLCFGR = 0x24003010;

	// 关闭PLLI2SON后 才能重置 PLLI2SCFGR
	RCC->PLLI2SCFGR = 0x20003000;

	g_osc = 8000000;
	g_sysclk = g_osc;
	g_hclk = g_sysclk / _hclk_tbl[AHBDiv];
	g_pclk1_pscr = _pclk_tbl[APB1Div];
	g_pclk1 = g_hclk / g_pclk1_pscr;
	g_pclk2_pscr = _pclk_tbl[APB2Div];
	g_pclk2 = g_hclk / g_pclk2_pscr;

	// 重置电压范围与FLASH等待时间
	PWR->CR &= ~( 1 << 14 );
	//RCC->APB1ENR &= ~( 1 << 28 );
	DPZ_MOD_BIT( FLASH->ACR, 0, 0x07, 0 );
}


void DPZ_SysPLL( unsigned int PLLSrc, unsigned int PLLm, unsigned int PLLn, unsigned int PLLp, unsigned int PLLq, unsigned char AHBDiv, unsigned char APB1Div, unsigned char APB2Div )
{
	unsigned char reg;

	/* AHB总线上所有时钟使能 恢复到初始状态 */
	RCC->AHB1ENR = 0x00100000;
	RCC->AHB2ENR = 0x00000000;
	RCC->AHB3ENR = 0x00000000;
	/* 关闭APB1 APB2所有外设时钟 */
	RCC->APB2ENR = 0x00000000;
	RCC->APB1ENR = 0x00000000;

	// 电压范围 均按照3.3V 高性能模式
	RCC->APB1ENR |= 1 << 28;
	PWR->CR |= 1 << 14;

	// 开启FLASH预取 数据 指令缓存 等待5周期(安全)
	FLASH->ACR |= 7 << 8;
	DPZ_MOD_BIT( FLASH->ACR, 0, 0x07, 5 );

	// 禁止所有时钟中断
	RCC->CIR = 0;

	if ( PLLSrc == 1 )
	{
		if ( !( RCC->CR & ( 1 << 17 ) ) )
		{
			// 开启HSE
			RCC->CR |= 1 << 16;

			// 等待HSE稳定
			while ( !( RCC->CR & ( 1 << 17 ) ) )
			{
				Delay_04NOP();
			}
		}
	}
	else
	{
		if ( !( RCC->CR & ( 1 << 1 ) ) )
		{
			// 开启HSI
			RCC->CR |= 1 << 0;

			// 等待HSI稳定
			while ( !( RCC->CR & ( 1 << 1 ) ) )
			{
				Delay_04NOP();
			}
		}
	}

	reg = ( RCC->CFGR & ( 3 << 2 ) ) >> 2;
	if ( reg != 2 )
	{
		if ( PLLSrc == 1 && reg != 1 )
		{
			/* 切换HSE为系统时钟 */
			DPZ_MOD_BIT( RCC->CFGR, 0, 0x03, 1 );
			/* 等待切换完成 */
			while (	( RCC->CFGR & ( 3 << 2 ) ) != ( 1 << 2 ) )
			{
				Delay_04NOP();
			}
		}
		else if ( PLLSrc == 0 && reg != 0 )
		{
			/* 切换HSI为系统时钟 */
			DPZ_MOD_BIT( RCC->CFGR, 0, 0x03, 0 );
			/* 等待切换完成 */
			while (	( RCC->CFGR & ( 3 << 2 ) ) != ( 0 << 2 ) )
			{
				Delay_04NOP();
			}
		}
	}

	// 设置Div 同时清除 MCO1 MCO2 I2S RTC (11为安全分频)
	RCC->CFGR = ( RCC->CFGR & 0x0000030F ) | ( ( unsigned int )11 & 0x0F ) << 4 | ( ( unsigned int )APB1Div & 0x07 ) << 10 | ( ( unsigned int )APB2Div & 0x07 ) << 13;
	Delay_04NOP();

	// 关闭其它时钟   CSSON PLLON PLLI2SON
	RCC->CR &= ~( 1 << 19 | 1 << 24 | 1 << 26 );
	// 关闭其它时钟 LSION
	RCC->CSR &= ~( 1 << 0 );


	// PLL设置 关闭PLLON后 才能重置 m n p q
	RCC->PLLCFGR &= ~0x0F437FFF;
	RCC->PLLCFGR |= ( PLLSrc & 0x01 ) << 22
	                | ( PLLn & 0x1FF ) << 6
	                | ( PLLm & 0x3F ) << 0
	                | ( ( ( PLLp >> 1 ) - 1 ) & 0x03 ) << 16
	                | ( PLLq & 0x0F ) << 24;

	// 开启PLL
	RCC->CR |= 1 << 24;

	// 等待PLL稳定
	while ( !( RCC->CR & ( 1 << 25 ) ) )
	{
		Delay_04NOP();
	}

	// 切换PLL为系统时钟
	DPZ_MOD_BIT( RCC->CFGR, 0, 0x03, 2 );

	// 等待切换完成
	while ( ( RCC->CFGR & ( 3 << 2 ) ) != ( 2 << 2 ) )
	{
		Delay_04NOP();
	}


	// 重置AHBDiv
	DPZ_MOD_BIT( RCC->CFGR, 4, 0x0F, ( unsigned int )AHBDiv );
	Delay_04NOP();

	if ( PLLSrc == 1 )
	{
		// 关闭HSI
		RCC->CR &= ~( 1 << 0 );

		g_osc = 8000000;
		g_sysclk = g_osc / PLLm * PLLn / PLLp;
		g_hclk = g_sysclk / _hclk_tbl[AHBDiv];
	}
	else
	{
		// 关闭HSE
		RCC->CR &= ~( 1 << 16 );
		// 关闭HSEON后 才能关闭HSEBYP
		RCC->CR &= ~( 1 << 18 );

		g_osc = 16000000;
		g_sysclk = g_osc / PLLm * PLLn / PLLp;
		g_hclk = g_sysclk / _hclk_tbl[AHBDiv];
	}


	g_pclk1_pscr = _pclk_tbl[APB1Div];
	g_pclk1 = g_hclk / g_pclk1_pscr;
	g_pclk2_pscr = _pclk_tbl[APB2Div];
	g_pclk2 = g_hclk / g_pclk2_pscr;

	if ( g_hclk <= 144000000 )
	{
		// 重置电压范围
		PWR->CR &= ~( 1 << 14 );
	}

	if ( g_hclk <= 30000000 )
	{
		DPZ_MOD_BIT( FLASH->ACR, 0, 0x07, 0 );
	}
	else if ( g_hclk <= 60000000 )
	{
		DPZ_MOD_BIT( FLASH->ACR, 0, 0x07, 1 );
	}
	else if ( g_hclk <= 90000000 )
	{
		DPZ_MOD_BIT( FLASH->ACR, 0, 0x07, 2 );
	}
	else if ( g_hclk <= 120000000 )
	{
		DPZ_MOD_BIT( FLASH->ACR, 0, 0x07, 3 );
	}
	else if ( g_hclk <= 150000000 )
	{
		DPZ_MOD_BIT( FLASH->ACR, 0, 0x07, 4 );
	}
	else
	{
		DPZ_MOD_BIT( FLASH->ACR, 0, 0x07, 5 );
	}
}



