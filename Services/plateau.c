#include "plateau.h"




void config_plateau(){
	MyTimer_Conf(TIM4,400, 3598);
	conf_capture_pwm_input(TIM4);
	
	
	MyTimer_Conf(TIM2,10, 359);
	conf_commande_pwm_output(TIM2);
	
	
	MyTimer_Start(TIM4);
	MyTimer_Start(TIM2);
	
}








void commander_vitesse_moteur(float vitesse_moteur_pourcentage) {
		
		float vitesse = (vitesse_moteur_pourcentage / 100.0) * 10.0 ;
		LL_TIM_OC_SetCompareCH2(TIM2,vitesse);
}

void commander_plateau(){
	
		float impulsion = TIM4->CCR2 ;
		
		float pourcentage = 0.0;
	
		if (impulsion < 30.0){
			
			pourcentage = ((30.0 - impulsion)/10.0) * 100.0;
			GPIOA->ODR |= GPIO_ODR_ODR2 ;
			commander_vitesse_moteur(pourcentage);
			
		}
		
		else if ( impulsion > 30.0){
			
			pourcentage = ((impulsion - 30.0 )/10.0) * 100.0;
			GPIOA->ODR &= ~GPIO_ODR_ODR2 ;
			commander_vitesse_moteur(pourcentage);
				
		}
		commander_vitesse_moteur(pourcentage);
							 				
	}
	
	
