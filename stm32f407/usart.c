#include "stm32f407xx.h"
#include "utility.h"
#include "sys.h"
#include "usart1.h"
#include "usart2.h"
#include "usart3.h"
#include "uart4.h"
#include "uart5.h"
#include "usart6.h"
#include "usart.h"


void DPZ_USART_Init( unsigned char port, unsigned int baudrate, int remap )
{
	switch ( port )
	{
		case 1:
		default:
			DPZ_USART1_Init( baudrate, remap );
			break;
		case 2:
			DPZ_USART2_Init( baudrate, remap );
			break;
		case 3:
			DPZ_USART3_Init( baudrate, remap );
			break;
		case 4:
			DPZ_UART4_Init( baudrate, remap );
			break;
		case 5:
			DPZ_UART5_Init( baudrate );
			break;
		case 6:
			DPZ_USART6_Init( baudrate, remap );
			break;
	}
}

void DPZ_USART_Send( USART_TypeDef* p, unsigned char* dat, unsigned int len )
{
	unsigned int i;

	while ( !( p->SR & ( 1 << 6 ) ) );

	if ( !len )
	{
		for ( i = 0; dat[i]; i ++ )
		{
			while ( !( p->SR & ( 1 << 7 ) ) )
			{
				Delay_04NOP();
			}
			p->DR = dat[i];
		}
	}
	else
	{
		for ( i = 0; i < len; i ++ )
		{
			while ( !( p->SR & ( 1 << 7 ) ) )
			{
				Delay_04NOP();
			}
			p->DR = dat[i];
		}
	}

	while ( !( p->SR & ( 1 << 6 ) ) )
	{
		Delay_04NOP();
	}
}


void DPZ_USART_Hex( USART_TypeDef* p, unsigned char* dat, unsigned short len, const char* endstr )
{
	unsigned int i;

	for ( i = 0; i < len; i ++ )
	{
		while ( !( p->SR & ( 1 << 7 ) ) );
		p->DR = _hex_char_TBL[dat[i] >> 4];

		while ( !( p->SR & ( 1 << 7 ) ) );
		p->DR = _hex_char_TBL[dat[i] & 0x0F];
	}

	while ( *endstr )
	{
		while ( !( p->SR & ( 1 << 7 ) ) );
		p->DR = *endstr++;
	}

	while ( !( p->SR & ( 1 << 6 ) ) )
	{
		Delay_04NOP();
	}
}

void DPZ_USART_HexUINT16( USART_TypeDef* p, unsigned char* dat, const char* endstr )
{
	while ( !( p->SR & ( 1 << 7 ) ) );
	p->DR = '0';
	while ( !( p->SR & ( 1 << 7 ) ) );
	p->DR = 'x';

	while ( !( p->SR & ( 1 << 7 ) ) );
	p->DR = _hex_char_TBL[dat[1] >> 4];;
	while ( !( p->SR & ( 1 << 7 ) ) );
	p->DR = _hex_char_TBL[dat[1] & 0x0F];

	while ( !( p->SR & ( 1 << 7 ) ) );
	p->DR = _hex_char_TBL[dat[0] >> 4];;
	while ( !( p->SR & ( 1 << 7 ) ) );
	p->DR = _hex_char_TBL[dat[0] & 0x0F];


	while ( *endstr )
	{
		while ( !( p->SR & ( 1 << 7 ) ) );
		p->DR = *endstr++;
	}

	while ( !( p->SR & ( 1 << 6 ) ) )
	{
		Delay_04NOP();
	}
}

void DPZ_USART_HexUINT32( USART_TypeDef* p, unsigned char* dat, const char* endstr )
{
	while ( !( p->SR & ( 1 << 7 ) ) );
	p->DR = '0';
	while ( !( p->SR & ( 1 << 7 ) ) );
	p->DR = 'x';

	while ( !( p->SR & ( 1 << 7 ) ) );
	p->DR = _hex_char_TBL[dat[3] >> 4];
	while ( !( p->SR & ( 1 << 7 ) ) );
	p->DR = _hex_char_TBL[dat[3] & 0x0F];

	while ( !( p->SR & ( 1 << 7 ) ) );
	p->DR = _hex_char_TBL[dat[2] >> 4];;
	while ( !( p->SR & ( 1 << 7 ) ) );
	p->DR = _hex_char_TBL[dat[2] & 0x0F];

	while ( !( p->SR & ( 1 << 7 ) ) );
	p->DR = _hex_char_TBL[dat[1] >> 4];;
	while ( !( p->SR & ( 1 << 7 ) ) );
	p->DR = _hex_char_TBL[dat[1] & 0x0F];

	while ( !( p->SR & ( 1 << 7 ) ) );
	p->DR = _hex_char_TBL[dat[0] >> 4];;
	while ( !( p->SR & ( 1 << 7 ) ) );
	p->DR = _hex_char_TBL[dat[0] & 0x0F];


	while ( *endstr )
	{
		while ( !( p->SR & ( 1 << 7 ) ) );
		p->DR = *endstr++;
	}

	while ( !( p->SR & ( 1 << 6 ) ) )
	{
		Delay_04NOP();
	}
}






