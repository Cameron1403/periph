#ifndef PLATEAU_H
#define PLATEAU_H

#include "MyTimer.h"
#include "stm32f1xx_ll_tim.h" 

#include "stm32f1xx_ll_bus.h" // Pour l'activation des horloges

void config_plateau(void);
void commander_vitesse_moteur(float vitesse_moteur_pourcentage);
void commander_plateau(void);



	




















#endif
