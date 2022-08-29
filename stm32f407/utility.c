#include "utility.h"




const char* _hex_char_TBL = "0123456789ABCDEF";
const char* _hex_char_tbl = "0123456789abcdef";

volatile unsigned int g_osc         = 8000000;
volatile unsigned int g_sysclk      = 0;
volatile unsigned int g_hclk        = 0;
volatile unsigned int g_pclk1       = 0;
volatile unsigned int g_pclk2       = 0;
volatile unsigned char g_pclk1_pscr = 1;
volatile unsigned char g_pclk2_pscr = 1;







// 进制转换
static const unsigned int _div_tbl[] = { 1000000000, 100000000, 10000000, 1000000, 100000, 10000, 1000, 100, 10, 1 };

unsigned char DPZ_U32Str( unsigned char buff[11], unsigned int d )
{
	unsigned char i, k, a;

	k = 0;
	for ( i = 0; i < 9; i ++ )
	{
		a = d / _div_tbl[i];
		if ( a != 0 )
		{
			d -= a * _div_tbl[i];
		}

		if ( k == 0 && a == 0 )
		{
			continue;
		}
		buff[k++] = a + '0';
	}
	buff[k++] = d + '0';
	buff[k] = 0;

	return k;
}


unsigned char DPZ_U16Str( unsigned char buff[6], unsigned short d )
{
	unsigned char i, k, a;

	k = 0;
	for ( i = 5; i < 9; i ++ )
	{
		a = d / _div_tbl[i];
		if ( a )
		{
			d -= a * _div_tbl[i];
		}

		if ( k == 0 && a == 0 )
		{
			continue;
		}
		buff[k++] = a + '0';
	}
	buff[k++] = d + '0';
	buff[k] = 0;

	return k;
}


unsigned char DPZ_U8Str( unsigned char buff[4], unsigned char c )
{
	unsigned char i, k, a;

	k = 0;
	for ( i = 7; i < 9; i ++ )
	{
		a = c / _div_tbl[i];
		if ( a )
		{
			c -= a * _div_tbl[i];
		}

		if ( k == 0 && a == 0 )
		{
			continue;
		}
		buff[k++] = a + '0';
	}
	buff[k++] = c + '0';
	buff[k] = 0;

	return k;
}

