#ifndef _UTILITY_H_
#define _UTILITY_H_



#define DPZ_MOD_BIT( _REG, _POS, _MSK, _VAL )   (_REG) = ( (_REG) & ~( (_MSK) << (_POS) ) ) | ( (_VAL) & (_MSK) ) << (_POS)
#define Delay_02NOP()                           do { __asm( "NOP" ); __asm( "NOP" ); } while ( 0 )
#define Delay_04NOP()                           do { __asm( "NOP" ); __asm( "NOP" ); __asm( "NOP" ); __asm( "NOP" ); } while ( 0 )



extern const char* _hex_char_TBL;
extern const char* _hex_char_tbl;
extern volatile unsigned int g_osc;
extern volatile unsigned int g_sysclk;
extern volatile unsigned int g_hclk;
extern volatile unsigned int g_pclk1;
extern volatile unsigned int g_pclk2;
extern volatile unsigned char g_pclk1_pscr;
extern volatile unsigned char g_pclk2_pscr;




typedef void ( *CAN_Rx_Handler )( unsigned int id, unsigned char ext, unsigned char rtr, unsigned char* dat, unsigned char len );



#ifdef __cplusplus
extern "C" {
#endif


static __inline void Delay_08NOP( unsigned int n )
{
	register unsigned int i;

	for ( i = 0; i < n; i ++ )
	{
		__asm( "NOP" );
		__asm( "NOP" );
		__asm( "NOP" );
		__asm( "NOP" );
		__asm( "NOP" );
	}
}

static __inline void Delay_10NOP( unsigned int n )
{
	register unsigned int i;

	// 循环三个周期  加 比较  跳转
	for ( i = 0; i < n; i ++ )
	{
		__asm( "NOP" );
		__asm( "NOP" );
		__asm( "NOP" );
		__asm( "NOP" );
		__asm( "NOP" );
		__asm( "NOP" );
		__asm( "NOP" );
	}
}

static __inline void Delay_24NOP( unsigned int n )
{
	register unsigned int i;

	// 循环三个周期  加 比较  跳转
	for ( i = 0; i < n; i ++ )
	{
		__asm( "NOP" );
		__asm( "NOP" );
		__asm( "NOP" );
		__asm( "NOP" );
		__asm( "NOP" );
		__asm( "NOP" );
		__asm( "NOP" );
		__asm( "NOP" );
		__asm( "NOP" );
		__asm( "NOP" );
		__asm( "NOP" );
		__asm( "NOP" );
		__asm( "NOP" );
		__asm( "NOP" );
		__asm( "NOP" );
		__asm( "NOP" );
		__asm( "NOP" );
		__asm( "NOP" );
		__asm( "NOP" );
		__asm( "NOP" );
		__asm( "NOP" );
	}
}


static __inline void* memcpy( void* dst, const void* src, unsigned int len )
{
	unsigned char* p;
	const unsigned char* q, *end;

	p = ( unsigned char* )dst;
	q = ( const unsigned char* )src;
	end = q + len;

	do
	{
		*p ++ = *q ++;
	} while ( q < end );

	return dst;
}


static __inline char memcmp( const void* dst, const void* src, unsigned int len )
{
	const char* p, *q, *end;
	char ret;

	p = ( const char* )dst;
	q = ( const char* )src;
	end = q + len;

	do
	{
		if ( ( ret = *p++ - *q++ ) != 0 )
		{
			return ret;
		}
	} while ( q < end );

	return 0;
}

static __inline void* memset( void* dst, const unsigned char val, unsigned int len )
{
	char* p, *end;

	p = ( char* )dst;
	end = p + len;

	do
	{
		*p ++ = val;
	} while ( p < end );

	return dst;
}



unsigned char DPZ_U32Str( unsigned char buff[11], unsigned int d );
unsigned char DPZ_U16Str( unsigned char buff[6], unsigned short d );
unsigned char DPZ_U8Str( unsigned char buff[4], unsigned char c );



#ifdef __cplusplus
}
#endif



#endif /* _UTILITY_H_ */
