#ifndef _CLK_H_
#define _CLK_H_


#ifdef __cplusplus
extern "C" {
#endif

void DPZ_SysHSI( unsigned char AHBDiv, unsigned char APB1Div, unsigned char APB2Div );
void DPZ_SysHSE( unsigned char AHBDiv, unsigned char APB1Div, unsigned char APB2Div );
void DPZ_SysPLL( unsigned int PLLSrc, unsigned int PLLm, unsigned int PLLn, unsigned int PLLp, unsigned int PLLq, unsigned char AHBDiv, unsigned char APB1Div, unsigned char APB2Div );

#ifdef __cplusplus
}
#endif



#endif /* _CLK_H_ */
