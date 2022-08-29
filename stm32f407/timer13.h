#ifndef _TIMER13_H_
#define _TIMER13_H_

#ifdef __cplusplus
extern "C" {
#endif

void DPZ_Timer13_PWM( unsigned short arr );
void DPZ_Timer13_Restart( unsigned short arr );
void DPZ_Timer13_CCR( unsigned short ccr );


#ifdef __cplusplus
}
#endif

#endif /* _TIMER13_H_ */
