#ifndef _SYS_H_
#define _SYS_H_


#ifdef __cplusplus
extern "C" {
#endif


void DPZ_SoftReset( void );

// 参数为平等级位数
void DPZ_NVIC_Init( unsigned char Sub );
void DPZ_SetNVIC( unsigned char Preemption, unsigned char Sub, int Channel );
void DPZ_ClearNVIC( int Channel );
void DPZ_PendingIRQ( int Channel, int flag );



#ifdef __cplusplus
}
#endif



#endif /* _SYS_H_ */
