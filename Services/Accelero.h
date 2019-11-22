// RIEN A MODIFIER //

#ifndef CHRONO_H
#define CHRONO_H

/*
Service permettant de chornométrer jusqu'à 59mn 59s 99 1/100
Utilise un timer au choix (TIMER1 à TIMER4).
Utilise la lib MyTimers.h /.c
*/

#include "stm32f103xb.h" 

/* =====================================================================================
Les fonctions qui gèrent les IO (ajout par rapport à l'activité 1)
=======================================================================================*/
typedef struct any init_adc_regular;

struct any {
		uint32_t TRIG ;
		uint32_t SCAN;
		uint32_t DISCONT;
		uint32_t CONT;
		uint32_t DMA;
	};
void adc_gpio_conf(void);
void uart_conf(void);
void UART_Send(USART_TypeDef *,int);
void sendAlerte(void);
	
void start_adc(void);
	
void gpio_conf(void);
	
float read_adc_angle(void);
int sup45(float);
int inf35(float);
int isFucked(int, float);
float lvlBatt(void);
int isBattLow(float);
	
#endif
