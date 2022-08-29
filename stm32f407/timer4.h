#ifndef _TIMER4_H_
#define _TIMER4_H_



extern unsigned short tone[];
extern unsigned char music[];
extern unsigned char duration[];




#ifdef __cplusplus
extern "C" {
#endif

void DPZ_Timer4_PWM( unsigned short arr, unsigned short psc );
void DPZ_Timer4_Restart( unsigned short arr );

#ifdef __cplusplus
}
#endif

#endif /* _TIMER4_H_ */
