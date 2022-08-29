#ifndef _TIMER1_H_
#define _TIMER1_H_

#ifdef __cplusplus
extern "C" {
#endif

void DPZ_Timer1_PWM( unsigned short arr, unsigned short psc );
void DPZ_Timer1_CCR( unsigned short ccr );

#ifdef __cplusplus
}
#endif

#endif /* _TIMER1_H_ */
