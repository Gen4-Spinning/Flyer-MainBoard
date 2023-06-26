/*
 * GB_Diag.c
 *
 *  Created on: 04-May-2023
 *      Author: harsha
 */

#include "GB_Diag.h"

void ResetGB_Diag(GB_diag *gbd){
	gbd->leftLiftPWM = 0;
	gbd->leftLift_DutyCycle = 0;
	gbd->liftLift_AbsPos = 0;

	gbd->rightLiftPWM = 0;
	gbd->rightLift_DutyCycle = 0;
	gbd->rightLift_AbsPos = 0;

}
