#ifndef _GPIO_H_
#define _GPIO_H_


#define DPZ_IO_CFG( port, pin, mode, otype, ospeed, pupd, af )  do { \
		DPZ_MOD_BIT( ( port )->MODER, ( ( unsigned int )( pin ) ) << 1, 0x03UL, ( unsigned int )( mode ) ); \
		DPZ_MOD_BIT( ( port )->OTYPER, ( unsigned int )( pin ), 0x01UL, ( unsigned int )( otype ) ); \
		DPZ_MOD_BIT( ( port )->OSPEEDR, ( ( unsigned int )( pin ) ) << 1, 0x03UL, ( unsigned int )( ospeed ) ); \
		DPZ_MOD_BIT( ( port )->PUPDR, ( ( unsigned int )( pin ) ) << 1, 0x03UL, ( unsigned int )( pupd ) ); \
		DPZ_MOD_BIT( ( port )->AFR[( pin ) >> 3], ( ( ( unsigned int )( pin ) ) & 7 ) << 2, 0x0FUL, ( unsigned int )( af ) ); \
	} while( 0 )


#define DPZ_IO_H( port, pin )  ( port )->BSRR = 1 << ( ( unsigned int )( pin ) )
#define DPZ_IO_L( port, pin )  ( port )->BSRR = 1 << ( ( unsigned int )( pin ) + 16 )
#define DPZ_IO_T( port, pin )  ( port )->ODR ^= 1 << ( ( unsigned int )( pin ) )



#ifdef __cplusplus
extern "C" {
#endif

void DPZ_LED_Init( void );
void DPZ_LED_ON( void );
void DPZ_LED_OFF( void );
void DPZ_LED_TOG( void );



void DPZ_GPIO_LowPower( void );


#ifdef __cplusplus
}
#endif



#endif /* _GPIO_H_ */
