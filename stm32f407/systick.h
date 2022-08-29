#ifndef _SYSTICK_H_
#define _SYSTICK_H_

#ifdef __cplusplus
extern "C" {
#endif

void DPZ_SysTick_US( unsigned int usecs );

void DPZ_SysTick_MS( unsigned int msecs );

void DPZ_SysTick_Sleep( unsigned int msecs );

#ifdef __cplusplus
}
#endif



#endif /* _SYSTICK_H_ */
