// TOUT A FAIRE !! //

/*
 indispensable pour pouvoir adresser les registres des p�riph�riques.
 Rem : OBLIGATION d'utiliser les d�finitions utiles contenues dans ce fichier (ex : TIM_CR1_CEN, RCC_APB1ENR_TIM2EN ...)
 pour une meilleure lisibilit� du code.

 Pour les masques, utiliser �galement les d�finitions propos�e
 Rappel : pour mettre � 1  , reg = reg | Mask (ou Mask est le repr�sente le ou les bits � positionner � 1)
				  pour mettre � 0  , reg = reg&~ Mask (ou Mask est le repr�sente le ou les bits � positionner � 0)
 
*/ 

#include "MyTimer.h"
#include "stm32f1xx_ll_bus.h" // Pour l'activation des horloges
#include "stm32f1xx_ll_tim.h" 






/**
	* @brief  Active l'horloge et r�gle l'ARR et le PSC du timer vis�
  * @note   Fonction � lancer avant toute autre. Le timer n'est pas encore lanc� (voir MyTimerStart)
	* @param  TIM_TypeDef Timer : indique le timer � utiliser par le chronom�tre, TIM1, TIM2, TIM3 ou TIM4
	* 				int Arr   : valeur � placer dans ARR
	*					int Psc   : valeur � placer dans PSC
  * @retval None
  */
void MyTimer_Conf(TIM_TypeDef * Timer,int Arr, int Psc)
{
	LL_TIM_InitTypeDef My_LL_Tim_Init_Struct;
	
	// Validation horloge locale
	if (Timer==TIM1) LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
	else if (Timer==TIM2) LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
	else if (Timer==TIM3) LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
	else  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);
	
	// chargement structure Arr, Psc, Up Count
	My_LL_Tim_Init_Struct.Autoreload=Arr;
	My_LL_Tim_Init_Struct.Prescaler=Psc;
	My_LL_Tim_Init_Struct.ClockDivision=LL_TIM_CLOCKDIVISION_DIV1;
	My_LL_Tim_Init_Struct.CounterMode=LL_TIM_COUNTERMODE_UP;
	My_LL_Tim_Init_Struct.RepetitionCounter=0;
	
	LL_TIM_Init(Timer,&My_LL_Tim_Init_Struct);
	

	// Blocage IT
	LL_TIM_DisableIT_UPDATE(Timer);
	
	
	// Blocage Timer
	LL_TIM_DisableCounter(Timer);
	

		
}


/**
	* @brief  D�marre le timer consid�r�
  * @note   
	* @param  TIM_TypeDef Timer : indique le timer � utiliser par le chronom�tre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void MyTimer_Start(TIM_TypeDef * Timer)
{
		LL_TIM_EnableCounter(Timer);
}

/**
	* @brief  Arr�t le timer consid�r�
  * @note   
	* @param  TIM_TypeDef Timer : indique le timer � utiliser par le chronom�tre, TIM1, TIM2, TIM3 ou TIM4
  * @retval None
  */
void MyTimer_Stop(TIM_TypeDef * Timer)
{
	LL_TIM_DisableCounter(Timer);
}

// Fonction permettant de recevoir un signal PWM sur le channel 1 du timer 4
void conf_capture_pwm_input (TIM_TypeDef * Timer) 
{
	// Configuration GPIOB
	//Enable Horloge pour GPIOB
		LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
	
	// Configuration de PB6, soit le pin 6 du GPIOB
		LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_6, LL_GPIO_MODE_INPUT);
	
	// Config PWM timer 4, channel 1
	if (Timer == TIM4) {
	Timer -> CCMR1 |= TIM_CCMR1_CC1S_0 ; 
	Timer -> CCMR1 &= ~TIM_CCMR1_CC1S_1 ;
	
	Timer -> CCER &= ~TIM_CCER_CC1P_Msk ;

	Timer -> CCMR1 &= ~TIM_CCMR1_CC2S_0;
	Timer -> CCMR1 |= TIM_CCMR1_CC2S_1;

	Timer-> CCER |= TIM_CCER_CC2P_Msk ;
	
	Timer -> SMCR |= TIM_SMCR_TS_0 ;
	Timer -> SMCR &= ~TIM_SMCR_TS_1;
	Timer -> SMCR |= TIM_SMCR_TS_2 ;
		
	Timer -> SMCR &= ~TIM_SMCR_SMS_0 ;
	Timer -> SMCR &= ~TIM_SMCR_SMS_1 ;
	Timer -> SMCR |= TIM_SMCR_SMS_2 ;
		
	Timer -> CCER |= TIM_CCER_CC1E ;
	Timer -> CCER |= TIM_CCER_CC2E ;
		
	}
	
	
}

// fonction permettant de configurer le port A pin 1 pour produire une PWM depuis le channel 2 du timer 4
void conf_commande_pwm_output ( TIM_TypeDef * Timer) 
{
	// Configuation GPIO A
	// Enable horloge GPIOA
		LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
	
	
	
	// Param�trage du pin 1 et du pin 2 
		LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_1, LL_GPIO_MODE_ALTERNATE);
		LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_2, LL_GPIO_MODE_OUTPUT );
	
		LL_TIM_OC_SetMode(TIM2,LL_TIM_CHANNEL_CH2,LL_TIM_OCMODE_PWM1);
		LL_TIM_CC_EnableChannel(TIM2,LL_TIM_CHANNEL_CH2);
		
}

