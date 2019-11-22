/**
  ******************************************************************************
  * @file    Templates_LL/Src/voile.h
  * @author  MCD Application Team
  * @brief   Programe qui controle les voiles
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
	
#ifndef VOILE_H
#define VOILE_H


#include "stm32f1xx_ll_rcc.h" // utile dans la fonction SystemClock_Config
#include "stm32f1xx_ll_utils.h"   // utile dans la fonction SystemClock_Config
#include "stm32f1xx_ll_system.h" // utile dans la fonction SystemClock_Con#
#include "stm32f1xx_ll_gpio.h" 
#include "stm32f1xx_ll_bus.h"
#include "stdio.h"
#include "stm32f1xx_ll_tim.h" 




void Config_TIM (TIM_TypeDef *TIMx) ;
	
void Config_GPIO(GPIO_TypeDef *GPIOx,TIM_TypeDef *TIMx);
	
void Config_voiles(void);
	
void Config_girouette(void);

void Gestion_Index(void);
		
int Calcul_teta(int alpha);
	
void Run_Vent (int Roulis_exces);

void Set_Voile(int Angle_voulu);
int Angle_Girouette(void);


#endif



	
	
	