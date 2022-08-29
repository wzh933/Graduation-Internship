#ifndef _USART_H_
#define _USART_H_

#define DPZ_USART1_Send( dat, len )                         DPZ_USART_Send( ( USART1 ), ( dat ), ( len ) )
#define DPZ_USART1_Hex( dat, len, endstr )                  DPZ_USART_Hex( ( USART1 ), ( dat ), ( len ), ( endstr ) )
#define DPZ_USART1_HexUINT16( dat, endstr )                 DPZ_USART_HexUINT16( ( USART1 ), ( dat ), ( endstr ) )
#define DPZ_USART1_HexUINT32( dat, endstr )                 DPZ_USART_HexUINT32( ( USART1 ), ( dat ), ( endstr ) )


#define DPZ_USART2_Send( dat, len )                         DPZ_USART_Send( ( USART2 ), ( dat ), ( len ) )
#define DPZ_USART2_Hex( dat, len, endstr )                  DPZ_USART_Hex( ( USART2 ), ( dat ), ( len ), ( endstr ) )
#define DPZ_USART2_HexUINT16( dat, endstr )                 DPZ_USART_HexUINT16( ( USART2 ), ( dat ), ( endstr ) )
#define DPZ_USART2_HexUINT32( dat, endstr )                 DPZ_USART_HexUINT32( ( USART2 ), ( dat ), ( endstr ) )


#define DPZ_USART3_Send( dat, len )                         DPZ_USART_Send( ( USART3 ), ( dat ), ( len ) )
#define DPZ_USART3_Hex( dat, len, endstr )                  DPZ_USART_Hex( ( USART3 ), ( dat ), ( len ), ( endstr ) )
#define DPZ_USART3_HexUINT16( dat, endstr )                 DPZ_USART_HexUINT16( ( USART3 ), ( dat ), ( endstr ) )
#define DPZ_USART3_HexUINT32( dat, endstr )                 DPZ_USART_HexUINT32( ( USART3 ), ( dat ), ( endstr ) )


#define DPZ_UART4_Send( dat, len )                         DPZ_USART_Send( ( UART4 ), ( dat ), ( len ) )
#define DPZ_UART4_Hex( dat, len, endstr )                  DPZ_USART_Hex( ( UART4 ), ( dat ), ( len ), ( endstr ) )
#define DPZ_UART4_HexUINT16( dat, endstr )                 DPZ_USART_HexUINT16( ( UART4 ), ( dat ), ( endstr ) )
#define DPZ_UART4_HexUINT32( dat, endstr )                 DPZ_USART_HexUINT32( ( UART4 ), ( dat ), ( endstr ) )


#define DPZ_UART5_Send( dat, len )                         DPZ_USART_Send( ( UART5 ), ( dat ), ( len ) )
#define DPZ_UART5_Hex( dat, len, endstr )                  DPZ_USART_Hex( ( UART5 ), ( dat ), ( len ), ( endstr ) )
#define DPZ_UART5_HexUINT16( dat, endstr )                 DPZ_USART_HexUINT16( ( UART5 ), ( dat ), ( endstr ) )
#define DPZ_UART5_HexUINT32( dat, endstr )                 DPZ_USART_HexUINT32( ( UART5 ), ( dat ), ( endstr ) )


#define DPZ_USART6_Send( dat, len )                         DPZ_USART_Send( ( USART6 ), ( dat ), ( len ) )
#define DPZ_USART6_Hex( dat, len, endstr )                  DPZ_USART_Hex( ( USART6 ), ( dat ), ( len ), ( endstr ) )
#define DPZ_USART6_HexUINT16( dat, endstr )                 DPZ_USART_HexUINT16( ( USART6 ), ( dat ), ( endstr ) )
#define DPZ_USART6_HexUINT32( dat, endstr )                 DPZ_USART_HexUINT32( ( USART6 ), ( dat ), ( endstr ) )



#ifdef __cplusplus
extern "C" {
#endif

void DPZ_USART_Init( unsigned char port, unsigned int baudrate, int remap );
void DPZ_USART_Send( USART_TypeDef* p, unsigned char* dat, unsigned int len );
void DPZ_USART_Hex( USART_TypeDef* p, unsigned char* dat, unsigned short len, const char* endstr );
void DPZ_USART_HexUINT16( USART_TypeDef* p, unsigned char* dat, const char* endstr );
void DPZ_USART_HexUINT32( USART_TypeDef* p, unsigned char* dat, const char* endstr );


#ifdef __cplusplus
}
#endif



#endif /* _USART_H_ */
