/*
 * BT_Machine.c
 *
 *  Created on: 25-Apr-2023
 *      Author: harsha
 */

#include "BT_Machine.h"


uint8_t BT_MC_generateSettingsMsg(machineSettingsTypeDef *m){
	  char TLV_Buffer[12];
	  uint8_t tlvSize = 0;
	  uint8_t eof_size  = 0;
	  uint8_t initLength = 0;

	  initLength = Init_TXBuf_Frame(SETTINGS_FROM_MC,SUBSTATE_NA,13);

	  generateTLV_I(TLV_Buffer,SPINDLE_SPEED,m->spindleSpeed);
	  add_TLVBuf_To_TxBuf(TLV_Buffer,TLV_INT,initLength+tlvSize);
	  tlvSize += TLV_INT;

	  generateTLV_F(TLV_Buffer,DRAFT,m->tensionDraft);
	  add_TLVBuf_To_TxBuf(TLV_Buffer,TLV_FLOAT,initLength+tlvSize);
	  tlvSize += TLV_FLOAT;

	  generateTLV_F(TLV_Buffer,TPI_FF,m->tpi);
	  add_TLVBuf_To_TxBuf(TLV_Buffer,TLV_FLOAT,initLength+tlvSize);
	  tlvSize += TLV_FLOAT;

	  generateTLV_I(TLV_Buffer,TARGET_LAYERS,m->targetLayers);
	  add_TLVBuf_To_TxBuf(TLV_Buffer,TLV_INT,initLength+tlvSize);
	  tlvSize += TLV_INT;

	  generateTLV_I(TLV_Buffer,CONTENTHEIGHT,m->contentHeight);
	  add_TLVBuf_To_TxBuf(TLV_Buffer,TLV_INT,initLength+tlvSize);
	  tlvSize += TLV_INT;

	  generateTLV_F(TLV_Buffer,ROVING_WIDTH,m->rovingWidth);
	  add_TLVBuf_To_TxBuf(TLV_Buffer,TLV_FLOAT,initLength+tlvSize);
	  tlvSize += TLV_FLOAT;

	  generateTLV_F(TLV_Buffer,DELTA_BOBBIN_DIA,m->deltaBobbinDia);
	  add_TLVBuf_To_TxBuf(TLV_Buffer,TLV_FLOAT,initLength+tlvSize);
	  tlvSize += TLV_FLOAT;

	  generateTLV_I(TLV_Buffer,BARE_BOBBIN_DIA,m->bareBobbinDia);
	  add_TLVBuf_To_TxBuf(TLV_Buffer,TLV_INT,initLength+tlvSize);
	  tlvSize += TLV_INT;

	  generateTLV_F(TLV_Buffer,RTF,m->rtf);
	  add_TLVBuf_To_TxBuf(TLV_Buffer,TLV_FLOAT,initLength+tlvSize);
	  tlvSize += TLV_FLOAT;

	  generateTLV_I(TLV_Buffer,RAMPUP_TIME,m->rampUpTime);
	  add_TLVBuf_To_TxBuf(TLV_Buffer,TLV_INT,initLength+tlvSize);
	  tlvSize += TLV_INT;

	  generateTLV_I(TLV_Buffer,RAMPDOWN_TIME,m->rampDownTime);
	  add_TLVBuf_To_TxBuf(TLV_Buffer,TLV_INT,initLength+tlvSize);
	  tlvSize += TLV_INT;

	  generateTLV_I(TLV_Buffer,CHANGELAYER_TIME,m->changeLayerTime);
	  add_TLVBuf_To_TxBuf(TLV_Buffer,TLV_INT,initLength+tlvSize);
	  tlvSize += TLV_INT;

	  generateTLV_F(TLV_Buffer,CONEANGLE_FACTOR,m->coneAngleFactor);
	  add_TLVBuf_To_TxBuf(TLV_Buffer,TLV_FLOAT,initLength+tlvSize);
	  tlvSize += TLV_FLOAT;

	  eof_size = addEOF(initLength+tlvSize);
	  correctLengthInFrame(initLength,tlvSize,eof_size);

	  return initLength + tlvSize + eof_size;
}

//FLYER
uint8_t BT_MC_parse_Settings(machineSettingsTypeDef *mspBT){
	//Buffer Rec index 10 onwards is TLVs till 10 + TlvsLength
	TLVStruct_TypeDef T;
	uint8_t TLV_start = 10;
	uint8_t tlvSize = 0;
	uint8_t count = 0;
	uint8_t allSettingsRecieved = 0;

    for (int i=0;i<BT.attributeCnt;i++){
    	tlvSize = parseTLV(&T,TLV_start);
    	TLV_start += tlvSize;
    	switch (T.type){
    		case SPINDLE_SPEED:
    			mspBT->spindleSpeed = T.value_int;
    			count += 1;
    			break;
    		case DRAFT:
    			mspBT->tensionDraft = T.value_f;
    			count += 1;
    			break;
    		case TPI_FF:
    			mspBT->tpi = T.value_f;
    			count += 1;
    			break;
    		case RTF:
    			mspBT->rtf = T.value_f;
    			count += 1;
    			break;
    		case TARGET_LAYERS:
    			mspBT->targetLayers = T.value_int;
    			count += 1;
    			break;
    		case CONTENTHEIGHT:
    			mspBT->contentHeight = T.value_int;
    			count += 1;
    			break;
    		case ROVING_WIDTH:
    			mspBT->rovingWidth = T.value_f;
    			count += 1;
    			break;
    		case DELTA_BOBBIN_DIA:
    			mspBT->deltaBobbinDia = T.value_f;
    			count += 1;
    			break;
    		case BARE_BOBBIN_DIA:
    			mspBT->bareBobbinDia = T.value_int;
    			count += 1;
    			break;
    		case RAMPUP_TIME:
    			mspBT->rampUpTime = T.value_int;
    			count += 1;
    			break;
    		case RAMPDOWN_TIME:
    			mspBT->rampDownTime = T.value_int;
    			count += 1;
    			break;
    		case CHANGELAYER_TIME:
    			mspBT->changeLayerTime = T.value_int;
    			count += 1;
    			break;
    		case CONEANGLE_FACTOR:
    			mspBT->coneAngleFactor = T.value_f;
    			count += 1;
    			break;
    	}
    }
    if (count == 13){
    	allSettingsRecieved = 1;
    }

    return allSettingsRecieved;
}

//FLYER
uint8_t BT_MC_Save_Settings(void){
	uint8_t fail;
	fail = WriteMachineSettingsIntoEeprom(&msp_BT);
	if (fail == 0){
		msp.spindleSpeed = msp_BT.spindleSpeed;
		msp.tensionDraft = msp_BT.tensionDraft;
		msp.tpi = msp_BT.tpi;
		msp.rtf = msp_BT.rtf;
		msp.targetLayers = msp_BT.targetLayers;
		msp.contentHeight = msp_BT.contentHeight;
		msp.deltaBobbinDia = msp_BT.deltaBobbinDia;
		msp.bareBobbinDia = msp_BT.bareBobbinDia;
		msp.rovingWidth = msp_BT.rovingWidth;
		msp.rampDownTime = msp_BT.rampDownTime;
		msp.rampUpTime = msp_BT.rampUpTime;
		msp.changeLayerTime = msp_BT.changeLayerTime;
		msp.coneAngleFactor = msp_BT.coneAngleFactor;
		//send success msg to APP
	}
	return !fail;
}


uint8_t GetMotorID_from_BTMotor_ID(uint8_t BT_motorID){
	if (BT_motorID == BT_FLYER){
		return FLYER;
	}else if (BT_motorID == BT_BOBBIN){
		return BOBBIN;
	}else if (BT_motorID == BT_FR){
		return FR;
	}else if (BT_motorID == BT_BR){
		return BR;
	}
	else if (BT_motorID == BT_LIFT_LEFT){
		return LEFT_LIFT;
	}
	else if (BT_motorID == BT_LIFT_RIGHT){
		return RIGHT_LIFT;
	}
	return 0;
}

uint8_t Get_BTMotorID_from_Motor_ID(uint8_t motorID){
	if (motorID == FLYER){
		return BT_FLYER;
	}else if (motorID == BOBBIN){
		return BT_BOBBIN;
	}else if (motorID == FR){
		return BT_FR;
	}else if (motorID == BR){
		return BT_BR;
	}else if (motorID == LEFT_LIFT){
		return BT_LIFT_LEFT;
	}else if (motorID == RIGHT_LIFT){
		return BT_LIFT_RIGHT;
	}
	return 99;
}

uint8_t GetMotorId_from_CarousalID(uint8_t carousalID){
	if (carousalID == BT_FLYER){
		return FLYER;
	}else if (carousalID == BT_BOBBIN){
		return BOBBIN;
	}else if (carousalID == BT_FR){
		return FR;
	}else if (carousalID == BT_BR){
		return BR;
	}else if (carousalID == BT_LIFT_LEFT){
		return LEFT_LIFT;
	}else if (carousalID == BT_LIFT_RIGHT){
		return RIGHT_LIFT;
	}
	return 99;
}

uint8_t BT_generate_GbCalibMsg(GB_diag *gbd){
	  char TLV_Buffer[12];
	  uint8_t tlvSize = 0;
	  uint8_t eof_size  = 0;
	  uint8_t initLength = 0;

	  initLength = Init_TXBuf_Frame(GBCALIB_DATA_FROM_MB,SUBSTATE_NA,2);

	  generateTLV_F(TLV_Buffer,GB_CALIB_LEFT_ABSPOS,gbd->leftLift_DutyCycle);
	  add_TLVBuf_To_TxBuf(TLV_Buffer,TLV_FLOAT,initLength+tlvSize);
	  tlvSize += TLV_FLOAT;

	  generateTLV_F(TLV_Buffer,GB_CALIB_RIGHT_ABSPOS,gbd->rightLift_DutyCycle);
	  add_TLVBuf_To_TxBuf(TLV_Buffer,TLV_FLOAT,initLength+tlvSize);
	  tlvSize += TLV_FLOAT;

	  eof_size = addEOF(initLength+tlvSize);
	  correctLengthInFrame(initLength,tlvSize,eof_size);
	  return initLength+tlvSize+eof_size;

}
