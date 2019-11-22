// A COMPLETER

/*
Service permettant de chornométrer jusqu'à 59mn 59s 99 1/100
Utilise un timer au choix (TIMER1 à TIMER4).
Utilise la lib MyTimers.h /.c
*/



#include <math.h>
#include "Accelero.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_usart.h"
#include "stm32f1xx_ll_spi.h"
#include "stm32f1xx_ll_utils.h"
#include "stm32f1xx_ll_adc.h"
#include "stm32f1xx_ll_rcc.h"



void adc_gpio_conf(void){
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);//clock enable GPIOA
	LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSRC_PCLK2_DIV_6); //adc prescaler 6
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);//clock enable ADC1
	
	
	//LL_ADC_Init(ADC1, LL_ADC_DATA_ALIGN_RIGHT); //init ADC1
	LL_ADC_REG_InitTypeDef init_my_adc = {LL_ADC_REG_TRIG_SOFTWARE,LL_ADC_REG_SEQ_SCAN_DISABLE,LL_ADC_REG_SEQ_DISCONT_DISABLE,LL_ADC_REG_CONV_SINGLE,LL_ADC_REG_DMA_TRANSFER_NONE}; //init des param de l'adc
	LL_ADC_REG_Init(ADC1,&init_my_adc);

	LL_GPIO_SetPinMode(GPIOC,LL_GPIO_PIN_0,LL_GPIO_MODE_ANALOG); //set pin to analog input
	LL_GPIO_SetPinMode(GPIOC,LL_GPIO_PIN_1,LL_GPIO_MODE_ANALOG);
	LL_GPIO_SetPinMode(GPIOC,LL_GPIO_PIN_2,LL_GPIO_MODE_ANALOG);
}

void uart_conf(void){ //Configuration UART
	
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1); //activation clk de uart1 et gpioA
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
		
	LL_USART_Enable(USART1);
	LL_USART_EnableDirectionTx(USART1); //mettre en transmission
	LL_USART_SetBaudRate(USART1,36000000,19200);
	
	
	LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_9,LL_GPIO_MODE_ALTERNATE);//Activation de l'alternate
	
	
}

void start_adc(void){
	LL_ADC_Enable(ADC1);
}



float read_adc_angle (){ //Lis les valeurs de x et y et calcule l'angle
	LL_ADC_ClearFlag_EOS(ADC1); //vider le flag EOC qui indique la fin de la conversion
	LL_ADC_REG_SetSequencerRanks(ADC1,LL_ADC_REG_RANK_1,LL_ADC_CHANNEL_10); //Choisir le channel 10 à lire
	LL_ADC_REG_StartConversionSWStart(ADC1); //démarrer conversion
	while (!(LL_ADC_IsActiveFlag_EOS(ADC1))){ //Attendre fin conv
	}
	float xval = ((float)LL_ADC_REG_ReadConversionData12(ADC1)); //récupération de x en hexa
	
	LL_ADC_ClearFlag_EOS(ADC1);
	LL_ADC_REG_SetSequencerRanks(ADC1,LL_ADC_REG_RANK_1,LL_ADC_CHANNEL_11); //Choisir le channel 11 à lire
	LL_ADC_REG_StartConversionSWStart(ADC1); //démarrer conversion
	while (!(LL_ADC_IsActiveFlag_EOS(ADC1))){ //Attendre fin conv
	}
	float yval = ((float)LL_ADC_REG_ReadConversionData12(ADC1)); //récupération de y en hexa
	
	xval = fabs(xval*3.3/0xFFF); //valeur en tension
	yval = fabs(yval*3.3/0xFFF); //valeur en tension
	
	float xnormal = fabs(xval - 1.47); //normalisation
	float ynormal = fabs(yval - 1.47);
	
	float angle=180*atan(xnormal/ynormal)/3.14159; //détermination de l'angle en degrés
	
	return angle;
}


int sup45 (float angle){ //renvoyer 1 si angle sup à 45
	int fucked = 0;
	if (angle > 45) {
		fucked = 1;
	}
	return fucked;
}

int inf35 (float angle){ //renvoyer 1 si angle inf à 35
	int notfucked = 0;
	if (angle < 35) {
		notfucked = 1;
	}
	return notfucked;
}

int isFucked(int fucked, float angle){ //change la valeur de fucked si on dépasse 45 ou inf à 35
	fucked = fucked;
	if (sup45(angle)){
			fucked = 1;
		}else if(inf35(angle)){
			fucked = 0;
		}
		return fucked;
}

float lvlBatt(){ //renvoit le niveau de batterie
	LL_ADC_ClearFlag_EOS(ADC1);
	LL_ADC_REG_SetSequencerRanks(ADC1,LL_ADC_REG_RANK_1,LL_ADC_CHANNEL_12); //Choisir le channel 12 à lire
	LL_ADC_REG_StartConversionSWStart(ADC1); //démarrer conversion
	while (!(LL_ADC_IsActiveFlag_EOS(ADC1))){ //Attendre fin conv
	}
	float batt = ((float)LL_ADC_REG_ReadConversionData12(ADC1)); //récupération de lvl batterie
	batt = batt*3.3*13.0/0xFFF;//normalisation
	return batt; 
}

int isBattLow(float batt){ //renvoit 1 si batterie inf à la valeur 
	int battLow = 0;
	if (batt<10.0){
		battLow = 1;
	}
	return battLow;
}

void UART_Send(USART_TypeDef * USART,	int value){ //envoi d'un seul caractère
	while (LL_USART_IsActiveFlag_TXE(USART) == 0){
		
	}
	LL_USART_TransmitData8(USART,value);
}

void sendAlerte(){ //envoi du message d'alerte, faisant appel à UART_Send
	LL_GPIO_SetPinMode(GPIOA,LL_GPIO_PIN_11, LL_GPIO_MODE_OUTPUT );
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_11);
	char mess[] = "Alerte, batterie faible ! ";
	for(char* i = mess; *i;++i){	
		UART_Send(USART1,*i);
	}
	//UART_Send(USART1,0x0D);
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_11);
}
//salut

