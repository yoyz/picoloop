/*-----------------------------------------------------------------------------
VOP.cpp
2002.3.11 sam
based Vstxsynth.cpp
© 1999, Steinberg Soft und Hardware GmbH, All Rights Reserved

-----------------------------------------------------------------------------*/

#include <stdio.h>
#include <float.h>
#include "opm.hpp"
#include "OPMdrv.hpp"
#ifndef __VOPM__
#include "VOPM.hpp"
#endif

#include "VOPMEdit.hpp"


#include "aeffeditor.h"
#if DEBUG
extern void DebugPrint (char *format, ...);
#endif
const char TblLabel[kNumParams][8]={
	"Con","FL","AMS","PMS","NFQ", "NoiseEn",
	"SPD","AMD","PMD","WF","OpMsk",
//OP M1
	"M1-TL","M1-AR","M1-D1R","M1-D1L","M1-D2R",
		"M1-RR","M1-KS","M1-MUL","M1-DT1","M1-DT2",
	"M1-AME",
//OP C1
	"C1-TL","C1-AR","C1-D1R","C1-D1L","C1-D2R",
		"C1-RR","C1-KS","C1-MUL","C1-DT1","C1-DT2",
	"C1-AME",
//OP M2
	"M2-TL","M2-AR","M2-D1R","M2-D1L","M2-D2R",
		"M2-RR","M2-KS","M2-MUL","M2-DT1","M2-DT2",
	"M2-AME",

//OP C2
	"C2-TL","C2-AR","C2-D1R","C2-D1L","C2-D2R",
	"C2-RR","C2-KS","C2-MUL","C2-DT1","C2-DT2",
	"C2-AME"
};

//-----------------------------------------------------------------------------------------
// VOPM
//-----------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new VOPM (audioMaster);
}

//-----------------------------------------------------------------------------------------
VOPM::VOPM (audioMasterCallback audioMaster) : AudioEffectX (audioMaster, kNumPrograms, kNumParams)
{
 int iCc; 
//	DebugStr((const unsigned char *)"  VOPM started\n");
	pOPM= new class Opm;
	pOPMdrv=new class OPMDRV(pOPM);
		pOPM->StartPcm(44100,1,0,0);
		if(pOPMdrv){setProgram (0);
		}
	if (audioMaster)
	{
		setNumInputs (0);				// no inputs
		setNumOutputs (kNumOutputs);	//
		canProcessReplacing ();
		isSynth ();
		programsAreChunks (true);
		setUniqueID(CCONST ('V', 'O', 'P', 'M'));
	}
	initProcess ();
	suspend ();
		pChunk= new unsigned char[32*1024];
		editor = new VOPMEdit(this);
		if(!editor){
			oome = true;
		}
    for(iCc=0;iCc<127;iCc++){
      lTblCcLearn[iCc]=-1;
    }
	
}

//-----------------------------------------------------------------------------------------
VOPM::~VOPM ()
{
	if (pOPMdrv){
		delete pOPMdrv;
	}
	if (pOPM){
		delete pOPM;
	}
	delete pChunk;
}
 //------------------------------------------------------------------------

VstIntPtr VOPM::vendorSpecific (VstInt32 lArg1, VstIntPtr lArg2, void* ptrArg, float floatArg)
{
//MouseWhell Enable 
	if (editor && lArg1 == CCONST('s','t','C','A') && lArg2 == CCONST('W','h','e','e'))
	{
		return editor->onWheel (floatArg) == true ? 1 : 0;
	}else{
		return AudioEffectX::vendorSpecific (lArg1, lArg2, ptrArg, floatArg);
	}
}

//-----------------------------------------------------------------------------------------
void VOPM::setProgram (VstInt32 program)
{
#ifdef DEBUG
	DebugPrint("VOPM::setProgram %d",program);
#endif
	pOPMdrv->setProgNum(program);
	curProgram=program;
	if(editor)((class VOPMEdit *)editor)->update();
}

//-----------------------------------------------------------------------------------------
void VOPM::setProgramName (char *name)
{
	pOPMdrv->setProgName(name);
}

//-----------------------------------------------------------------------------------------
void VOPM::getProgramName (char *name)
{
	strcpy (name, pOPMdrv->getProgName());
}

//-----------------------------------------------------------------------------------------
void VOPM::getParameterLabel (VstInt32 index, char *label)
{
#if DEBUG
	DebugPrint("exec VOPM::getParamaeterLabel");
#endif
	switch (index)
	{
		case kNE:
		case kM1F_AME:
		case kM2F_AME:
		case kC1F_AME:
		case kC2F_AME:	strcpy (label, "		");		break;
		case kOpMsk:	strcpy (label, "  OpMsk ");		break;
		default:		strcpy (label, "  HEX   ");		break;
	}
}

//-----------------------------------------------------------------------------------------
void VOPM::getParameterDisplay (VstInt32 index, char *text)
{
#if DEBUG
	DebugPrint("exec VOPM::getParamaeterDisplay");
#endif
	class CHDATA *pVo;
	pVo=&(pOPMdrv->VoTbl[curProgram]);
	text[0] = 0;
	switch (index)
	{
		case kNE:		if(pVo->F_NE==128){strcpy(text,"ON");}else{strcpy(text,"OFF");}break;
		case kFL:		sprintf(text,"%02X",pVo->FL);break;
		case kCon:		sprintf(text,"%02X",pVo->CON);break;
		case kAMS:		sprintf(text,"%02X",pVo->AMS);break;
		case kPMS:		sprintf(text,"%02X",pVo->PMS);break;
		case kSPD:		sprintf(text,"%02X",pVo->SPD);break;
		case kPMD:		sprintf(text,"%02X",pVo->PMD);break;
		case kAMD:		sprintf(text,"%02X",pVo->AMD);break;
		case kWF:
			switch(pVo->WF){
				case 0:strcpy(text,"Saw");break;
				case 1:strcpy(text,"Sqr");break;
				case 2:strcpy(text,"Tri");break;
				case 3:strcpy(text,"Noise");break;
			}					break;
		case kNFQ:		sprintf(text,"%x",pVo->NFQ);		break;
		case kOpMsk:
			if((pVo->OpMsk&0x40)==0x40)strcat(text,"C2");
			if((pVo->OpMsk&0x20)==0x20)strcat(text,"M2");
			if((pVo->OpMsk&0x10)==0x10)strcat(text,"C1");
			if((pVo->OpMsk&0x08)==0x08)strcat(text,"M1");
			break;
		case kM1TL:		sprintf(text,"%02X",pVo->Op[0].TL);break;
		case kM2TL:		sprintf(text,"%02X",pVo->Op[1].TL);break;
		case kC1TL:		sprintf(text,"%02X",pVo->Op[2].TL);break;
		case kC2TL:		sprintf(text,"%02X",pVo->Op[3].TL);break;
		case kM1D1L:	sprintf(text,"%02X",pVo->Op[0].D1L);break;
		case kM2D1L:	sprintf(text,"%02X",pVo->Op[1].D1L);break;
		case kC1D1L:	sprintf(text,"%02X",pVo->Op[2].D1L);break;
		case kC2D1L:	sprintf(text,"%02X",pVo->Op[3].D1L);break;
		case kM1AR:		sprintf(text,"%02X",pVo->Op[0].AR);break;
		case kM2AR:		sprintf(text,"%02X",pVo->Op[1].AR);break;
		case kC1AR:		sprintf(text,"%02X",pVo->Op[2].AR);break;
		case kC2AR:		sprintf(text,"%02X",pVo->Op[3].AR);break;
		case kM1D1R:	sprintf(text,"%02X",pVo->Op[0].D1R);break;
		case kM2D1R:	sprintf(text,"%02X",pVo->Op[1].D1R);break;
		case kC1D1R:	sprintf(text,"%02X",pVo->Op[2].D1R);break;
		case kC2D1R:	sprintf(text,"%02X",pVo->Op[3].D1R);break;
		case kM1D2R:	sprintf(text,"%02X",pVo->Op[0].D2R);break;
		case kM2D2R:	sprintf(text,"%02X",pVo->Op[1].D2R);break;
		case kC1D2R:	sprintf(text,"%02X",pVo->Op[2].D2R);break;
		case kC2D2R:	sprintf(text,"%02X",pVo->Op[3].D2R);break;
		case kM1RR:		sprintf(text,"%02X",pVo->Op[0].RR);break;
		case kM2RR:		sprintf(text,"%02X",pVo->Op[1].RR);break;
		case kC1RR:		sprintf(text,"%02X",pVo->Op[2].RR);break;
		case kC2RR:		sprintf(text,"%02X",pVo->Op[3].RR);break;
		case kM1KS:		sprintf(text,"%02X",pVo->Op[0].KS);break;
		case kM2KS:		sprintf(text,"%02X",pVo->Op[1].KS);break;
		case kC1KS:		sprintf(text,"%02X",pVo->Op[2].KS);break;
		case kC2KS:		sprintf(text,"%02X",pVo->Op[3].KS);break;
		case kM1DT1:	sprintf(text,"%02X",pVo->Op[0].DT1);break;
		case kM2DT1:	sprintf(text,"%02X",pVo->Op[1].DT1);break;
		case kC1DT1:	sprintf(text,"%02X",pVo->Op[2].DT1);break;
		case kC2DT1:	sprintf(text,"%02X",pVo->Op[3].DT1);break;
		case kM1MUL:	sprintf(text,"%02X",pVo->Op[0].MUL);break;
		case kM2MUL:	sprintf(text,"%02X",pVo->Op[1].MUL);break;
		case kC1MUL:	sprintf(text,"%02X",pVo->Op[2].MUL);break;
		case kC2MUL:	sprintf(text,"%02X",pVo->Op[3].MUL);break;
		case kM1DT2:	sprintf(text,"%02X",pVo->Op[0].DT2);break;
		case kM2DT2:	sprintf(text,"%02X",pVo->Op[1].DT2);break;
		case kC1DT2:	sprintf(text,"%02X",pVo->Op[2].DT2);break;
		case kC2DT2:	sprintf(text,"%02X",pVo->Op[3].DT2);break;
		case kM1F_AME:
			if(pVo->Op[0].F_AME==0){
				strcpy(text,"OFF");
			}else{
				strcpy(text,"ON");
			}
			break;
		case kM2F_AME:
			if(pVo->Op[1].F_AME==0){
				strcpy(text,"OFF");
			}else{
				strcpy(text,"ON");
			}
			break;
		case kC1F_AME:
			if(pVo->Op[2].F_AME==0){
				strcpy(text,"OFF");
			}else{
				strcpy(text,"ON");
			}
			break;
		case kC2F_AME:
			if(pVo->Op[3].F_AME==0){
				strcpy(text,"OFF");
			}else{
				strcpy(text,"ON");
			}
			break;
	}

}

//-----------------------------------------------------------------------------------------
void VOPM::getParameterName (VstInt32 index, char *label)
{

	strcpy (label, TblLabel[index]);
#if DEBUG
	DebugPrint("exec VOPM::getParamaeterName TblLabel[%d]=%s",index,label);
#endif
}

//-----------------------------------------------------------------------------------------
void VOPM::setParameter (VstInt32 index, float value)
{
#if DEBUG
	DebugPrint("exec VOPM::setParamaeter index=%d value=%f",index,value);
#endif
	class CHDATA *pVo;
	float data;
	pVo=&(pOPMdrv->VoTbl[curProgram]);

	switch (index)
	{
		case kCon:data=value*7.0;pVo->CON=(unsigned char)data;break;
		case kFL:data=value*7.0;pVo->FL=(unsigned char)data;break;
		case kAMS:data=value*3.0;pVo->AMS=(unsigned char)data;break;
		case kPMS:data=value*7.0;pVo->PMS=(unsigned char)data;break;
		case kNE:pVo->F_NE=(value>=0.5)?128:0;break;
		case kSPD:data=value*255.0;pVo->SPD=(unsigned char)data;break;
		case kPMD:data=value*127.0;pVo->PMD=(unsigned char)data;break;
		case kAMD:data=value*127.0;pVo->AMD=(unsigned char)data;break;
		case kWF:data=value*3.0;pVo->WF=(unsigned char)data;break;
		case kNFQ:data=value*31.0;pVo->NFQ=(unsigned char)data;break;
		case kOpMsk:data=value*15.0;pVo->OpMsk=(unsigned char)data<<3;break;
		case kM1TL:data=value*127.0;pVo->Op[0].TL=(unsigned char)data;break;
		case kM2TL:data=value*127.0;pVo->Op[1].TL=(unsigned char)data;break;
		case kC1TL:data=value*127.0;pVo->Op[2].TL=(unsigned char)data;break;
		case kC2TL:data=value*127.0;pVo->Op[3].TL=(unsigned char)data;break;
		case kM1D1L:data=value*15.0;pVo->Op[0].D1L=(unsigned char)data;break;
		case kM2D1L:data=value*15.0;pVo->Op[1].D1L=(unsigned char)data;break;
		case kC1D1L:data=value*15.0;pVo->Op[2].D1L=(unsigned char)data;break;
		case kC2D1L:data=value*15.0;pVo->Op[3].D1L=(unsigned char)data;break;
		case kM1AR:data=value*31.0;pVo->Op[0].AR=(unsigned char)data;break;
		case kM2AR:data=value*31.0;pVo->Op[1].AR=(unsigned char)data;break;
		case kC1AR:data=value*31.0;pVo->Op[2].AR=(unsigned char)data;break;
		case kC2AR:data=value*31.0;pVo->Op[3].AR=(unsigned char)data;break;
		case kM1D1R:data=value*31.0;pVo->Op[0].D1R=(unsigned char)data;break;
		case kM2D1R:data=value*31.0;pVo->Op[1].D1R=(unsigned char)data;break;
		case kC1D1R:data=value*31.0;pVo->Op[2].D1R=(unsigned char)data;break;
		case kC2D1R:data=value*31.0;pVo->Op[3].D1R=(unsigned char)data;break;
		case kM1D2R:data=value*31.0;pVo->Op[0].D2R=(unsigned char)data;break;
		case kM2D2R:data=value*31.0;pVo->Op[1].D2R=(unsigned char)data;break;
		case kC1D2R:data=value*31.0;pVo->Op[2].D2R=(unsigned char)data;break;
		case kC2D2R:data=value*31.0;pVo->Op[3].D2R=(unsigned char)data;break;
		case kM1RR:data=value*15.0;pVo->Op[0].RR=(unsigned char)data;break;
		case kM2RR:data=value*15.0;pVo->Op[1].RR=(unsigned char)data;break;
		case kC1RR:data=value*15.0;pVo->Op[2].RR=(unsigned char)data;break;
		case kC2RR:data=value*15.0;pVo->Op[3].RR=(unsigned char)data;break;
		case kM1KS:data=value*3.0;pVo->Op[0].KS=(unsigned char)data;break;
		case kM2KS:data=value*3.0;pVo->Op[1].KS=(unsigned char)data;break;
		case kC1KS:data=value*3.0;pVo->Op[2].KS=(unsigned char)data;break;
		case kC2KS:data=value*3.0;pVo->Op[3].KS=(unsigned char)data;break;
		case kM1DT1:data=value*7.0;pVo->Op[0].DT1=(unsigned char)data;break;
		case kM2DT1:data=value*7.0;pVo->Op[1].DT1=(unsigned char)data;break;
		case kC1DT1:data=value*7.0;pVo->Op[2].DT1=(unsigned char)data;break;
		case kC2DT1:data=value*7.0;pVo->Op[3].DT1=(unsigned char)data;break;
		case kM1MUL:data=value*15.0;pVo->Op[0].MUL=(unsigned char)data;break;
		case kM2MUL:data=value*15.0;pVo->Op[1].MUL=(unsigned char)data;break;
		case kC1MUL:data=value*15.0;pVo->Op[2].MUL=(unsigned char)data;break;
		case kC2MUL:data=value*15.0;pVo->Op[3].MUL=(unsigned char)data;break;
		case kM1DT2:data=value*3.0;pVo->Op[0].DT2=(unsigned char)data;break;
		case kM2DT2:data=value*3.0;pVo->Op[1].DT2=(unsigned char)data;break;
		case kC1DT2:data=value*3.0;pVo->Op[2].DT2=(unsigned char)data;break;
		case kC2DT2:data=value*3.0;pVo->Op[3].DT2=(unsigned char)data;break;
		case kM1F_AME:pVo->Op[0].F_AME=(value>=0.5)?128:0;break;
		case kM2F_AME:pVo->Op[1].F_AME=(value>=0.5)?128:0;break;
		case kC1F_AME:pVo->Op[2].F_AME=(value>=0.5)?128:0;break;
		case kC2F_AME:pVo->Op[3].F_AME=(value>=0.5)?128:0;break;
	}
}

//-----------------------------------------------------------------------------------------
float VOPM::getParameter (VstInt32 index)
{
#if DEBUG
	DebugPrint("exec VOPM::getParamaeter");
#endif
	float value = 0;
	class CHDATA *pVo;
	pVo=&(pOPMdrv->VoTbl[curProgram]);
	switch (index){
		case kCon:value=pVo->CON/7.0;break;
		case kFL:value=pVo->FL/7.0;break;
		case kAMS:value=pVo->AMS/3.0;break;
		case kPMS:value=pVo->PMS/7.0;break;
		case kNE:value=(pVo->F_NE==0x80)?1.0:0;break;
		case kSPD:value=pVo->SPD/255.0;break;
		case kPMD:value=pVo->PMD/127.0;break;
		case kAMD:value=pVo->AMD/127.0;break;
		case kWF:value=pVo->WF/3.0;break;
		case kNFQ:value=pVo->NFQ/31.0;break;
		case kOpMsk:value=(pVo->OpMsk>>3)/15.0;break;
		case kM1TL:value=pVo->Op[0].TL/127.0;break;
		case kM2TL:value=pVo->Op[1].TL/127.0;break;
		case kC1TL:value=pVo->Op[2].TL/127.0;break;
		case kC2TL:value=pVo->Op[3].TL/127.0;break;
		case kM1D1L:value=pVo->Op[0].D1L/15.0;break;
		case kM2D1L:value=pVo->Op[1].D1L/15.0;break;
		case kC1D1L:value=pVo->Op[2].D1L/15.0;break;
		case kC2D1L:value=pVo->Op[3].D1L/15.0;break;
		case kM1AR:value=pVo->Op[0].AR/31.0;break;
		case kM2AR:value=pVo->Op[1].AR/31.0;break;
		case kC1AR:value=pVo->Op[2].AR/31.0;break;
		case kC2AR:value=pVo->Op[3].AR/31.0;break;
		case kM1D1R:value=pVo->Op[0].D1R/31.0;break;
		case kM2D1R:value=pVo->Op[1].D1R/31.0;break;
		case kC1D1R:value=pVo->Op[2].D1R/31.0;break;
		case kC2D1R:value=pVo->Op[3].D1R/31.0;break;
		case kM1D2R:value=pVo->Op[0].D2R/31.0;break;
		case kM2D2R:value=pVo->Op[1].D2R/31.0;break;
		case kC1D2R:value=pVo->Op[2].D2R/31.0;break;
		case kC2D2R:value=pVo->Op[3].D2R/31.0;break;
		case kM1RR:value=pVo->Op[0].RR/15.0;break;
		case kM2RR:value=pVo->Op[1].RR/15.0;break;
		case kC1RR:value=pVo->Op[2].RR/15.0;break;
		case kC2RR:value=pVo->Op[3].RR/15.0;break;
		case kM1KS:value=pVo->Op[0].KS/3.0;break;
		case kM2KS:value=pVo->Op[1].KS/3.0;break;
		case kC1KS:value=pVo->Op[2].KS/3.0;break;
		case kC2KS:value=pVo->Op[3].KS/3.0;break;
		case kM1DT1:value=pVo->Op[0].DT1/15.0;break;
		case kM2DT1:value=pVo->Op[1].DT1/15.0;break;
		case kC1DT1:value=pVo->Op[2].DT1/15.0;break;
		case kC2DT1:value=pVo->Op[3].DT1/15.0;break;
		case kM1MUL:value=pVo->Op[0].MUL/15.0;break;
		case kM2MUL:value=pVo->Op[1].MUL/15.0;break;
		case kC1MUL:value=pVo->Op[2].MUL/15.0;break;
		case kC2MUL:value=pVo->Op[3].MUL/15.0;break;
		case kM1DT2:value=pVo->Op[0].DT2/3.0;break;
		case kM2DT2:value=pVo->Op[1].DT2/3.0;break;
		case kC1DT2:value=pVo->Op[2].DT2/3.0;break;
		case kC2DT2:value=pVo->Op[3].DT2/3.0;break;
		case kM1F_AME:value=(pVo->Op[0].F_AME==0x80)?1.0:0;break;
		case kM2F_AME:value=(pVo->Op[1].F_AME==0x80)?1.0:0;break;
		case kC1F_AME:value=(pVo->Op[2].F_AME==0x80)?1.0:0;break;
		case kC2F_AME:value=(pVo->Op[3].F_AME==0x80)?1.0:0;break;
	}
	return value;
}

//-----------------------------------------------------------------------------------------
bool VOPM::getOutputProperties (VstInt32 index, VstPinProperties* properties)
{
#if DEBUG
	DebugPrint("exec VOPM::getOutputProperties %d ",index);
#endif
	if (index < kNumOutputs)
	{
		sprintf (properties->label, "VOPM %1d", index + 1);
		properties->flags = kVstPinIsActive;
		if (index < 2)
			properties->flags |= kVstPinIsStereo;	// test, make channel 1+2 stereo
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------------------
bool VOPM::copyProgram (long destination)
{
	if (destination < kNumPrograms)
	{
		pOPMdrv->VoTbl[destination] = pOPMdrv->VoTbl[curProgram];
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------------------
bool VOPM::getEffectName (char* name)
{
	strcpy (name, "VOPM");
	return true;
}

//-----------------------------------------------------------------------------------------
bool VOPM::getVendorString (char* text)
{
	strcpy (text, "Sam");
	return true;
}

//-----------------------------------------------------------------------------------------
bool VOPM::getProductString (char* text)
{
	strcpy (text, "Vst OPM ");
	return true;
}

//-----------------------------------------------------------------------------------------

VstInt32 VOPM::canDo (char* text)
{
#if DEBUG
	DebugPrint("exec VOPM::canDo [%s] ",text);
#endif	
	if (!strcmp (text, "receiveVstEvents"))
		return 1;
	if (!strcmp (text, "receiveVstMidiEvent"))
		return 1;
		//if(!strcmp(text, "midiProgramNames"))
		//		return 1;
	return -1;	// explicitly can't do; 0 => don't know
}

VstInt32 VOPM::getChunk(void** data, bool isPreset) {
	unsigned char *pData;
	int index,Prog,Size,OldProg;
	char buf[16];
	pData=(unsigned char*)pChunk;
	*data=pChunk;
	Size=0;
	
	#if DEBUG
		DebugPrint("VOPM::getChunk");
	#endif
	if(isPreset){
		for(index=0;index<kNumParams;index++){
			*pData=pOPMdrv->getPrm(index);
			pData++;
			Size++;
		}
		strncpy(buf,pOPMdrv->getProgName(),16);
		buf[15]=0x00;
		for(index=0;index<16;index++){
			*pData=buf[index];
			pData++;
			Size++;
		}
	}else{
		OldProg=pOPMdrv->getProgNum();
		for(Prog=0;Prog<128;Prog++){
			pOPMdrv->setProgNum(Prog);

			for(index=0;index<kNumParams;index++){
				*pData=pOPMdrv->getPrm(index);
				pData++;
				Size++;
			}

			strncpy(buf,pOPMdrv->getProgName(),16);
			buf[15]=0x00;
			
			for(index=0;index<16;index++){
				*pData=buf[index];
				pData++;
				Size++;
			}
		}
		pOPMdrv->setProgNum(OldProg);
	}
	return(Size);
}	// returns byteSize

VstInt32 VOPM::setChunk(void* data, VstInt32 byteSize, bool isPreset) {
	unsigned char *pData;
	int index,Prog,OldProg;
	char buf[16];
	pData=(unsigned char*)data;
	bool bResult=true;

	if(isPreset){
	
		#if DEBUG
			DebugPrint("VOPM::setChunk<Priset>");
		#endif
		
		for(index=0;index<kNumParams;index++){
			pOPMdrv->setPrm(index,*pData);
			pData++;
		}

		for(index=0;index<16;index++){
			buf[index]=*pData;
			pData++;
		}
		pOPMdrv->setProgName(buf);
	}else{

		#if DEBUG
			DebugPrint("VOPM::setChunk<Bank>");
		#endif
		
		OldProg=pOPMdrv->getProgNum();
		for(Prog=0;Prog<128;Prog++){
			pOPMdrv->setProgNum(Prog);
		
			for(index=0;index<kNumParams;index++){
				pOPMdrv->setPrm(index,*pData);
				pData++;
			}
			
			for(index=0;index<16;index++){
				buf[index]=*pData;
				pData++;
			}
			
			pOPMdrv->setProgName(buf);
		}
		pOPMdrv->setProgNum(OldProg);
	}
	if(editor)((class VOPMEdit *)editor)->update();		
	return bResult;
}

