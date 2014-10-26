/*-----------------------------------------------------------------------------
VOPMproc.cpp
2002.3.11 sam
based vstsynthproc.cpp
-----------------------------------------------------------------------------*/

#ifndef __VOPM__
#include "VOPM.hpp"
#endif
#include "OPM.hpp"
#include "OPMdrv.hpp"
#include <math.h>
#include "aeffeditor.h"

#include "VOPMEdit.hpp"
#if DEBUG
extern void DebugPrint (char *format, ...);
#endif

#ifdef MAC
	#include <Carbon/Carbon.h>
	#include "globhead.h"
#endif
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------
void VOPM::setSampleRate (float sampleRate)
{
	AudioEffectX::setSampleRate (sampleRate);
		SampleRate=(long)sampleRate;
		pOPM->SetSamprate((int)sampleRate);



}

//-----------------------------------------------------------------------------------------
void VOPM::setBlockSize (long blockSize)
{
	AudioEffectX::setBlockSize (blockSize);
	// you may need to have to do something here...
}

//-----------------------------------------------------------------------------------------
void VOPM::resume ()
{
//	wantEvents ();
	AudioEffectX::resume ();
}

//-----------------------------------------------------------------------------------------
void VOPM::initProcess ()
{
char *ptmp;
char *pbuf;
char filename[512];
char data[32];
long bytesize;
bool Err;
FILE *fin;
		Err=false;
		rate=0;
		SampleRate=44100;
		strcpy(filename,"");
		ptmp=(char *)getDirectory();
		if(ptmp==NULL){
#if MACX
			FSRef outFSRef,parFSRef;
			ProcessSerialNumber  currentProcess = { 0, kCurrentProcess }; 
			GetProcessBundleLocation( &currentProcess, &outFSRef );
			FSGetCatalogInfo(&outFSRef,kFSCatInfoNone,nil,nil,nil,&parFSRef);
			FSRefMakePath(&parFSRef,(UInt8 *)filename,512);
			strcat(filename,"/");	
#else
		strcpy(filename,".\\");
#endif
		}else{
		//Get plugin Directory,Maybe Bug because include plugfilename?
		strcpy(filename,ptmp);
		}

		strcat(filename,"VOPM.fxb");
//		DebugStr((const unsigned char *)filename);
		if((fin=fopen(filename,"rb"))==NULL){
//				DebugStr((const unsigned char *)" couldn't open the file");
				return;
		}
		//Chunk check
		fread(data,sizeof(char),4,fin);
		if(strncmp(data,"CcnK",4)){
		Err=true;
		}
		fseek(fin,4,SEEK_CUR);
		fread(data,sizeof(char),4,fin);
		if(strncmp(data,"FBCh",4)){
		Err=true;
		}
		fseek(fin,4,SEEK_CUR);
		fread(data,sizeof(char),4,fin);
		if(strncmp(data,"VOPM",4)){
		Err=true;
		}
		if(Err){fclose(fin);return;}
		fseek(fin,128+8,SEEK_CUR);
		//calc chunk size
		fread(data,sizeof(char),4,fin);
		bytesize=((unsigned long)data[0]<<24)+
			((unsigned long)data[1]<<16)+
			((unsigned long)data[2]<<8)+
			((unsigned long)data[3]);
		if((ptmp=(char *)malloc(bytesize))==NULL){fclose(fin);return;}
		//read chunk
		fread(ptmp,sizeof(char),bytesize,fin);
		setChunk(ptmp,bytesize,false);
		free(ptmp);
		fclose(fin);
}

//-----------------------------------------------------------------------------------------
void VOPM::process (float **inputs, float **outputs, int sampleFrames)
{
	//波形生成ルーチンを呼ぶ
	long fs;
	fs=SampleRate/100;
	rate+=sampleFrames;
	while(rate>fs){
		//10ms毎にAutoSeqを進める
		rate-=fs;
		pOPMdrv->AutoSeq(0);
	}
	pOPM->pcmset22(sampleFrames,outputs,false);
#if DEBUG
	DebugPrint("VOPMproc::process *outputs[0]=%f",
		**outputs
	);
#endif
}

//-----------------------------------------------------------------------------------------
void VOPM::processReplacing (float **inputs, float **outputs, int sampleFrames)
{
	//波形生成ルーチンを呼ぶ
	long fs;
	fs=SampleRate/100;
	rate+=sampleFrames;
	while(rate>fs){
		//10ms毎にAutoSeqを進める
		rate-=fs;
		pOPMdrv->AutoSeq(0);
	}
	pOPM->pcmset22(sampleFrames,outputs,true);
#if DEBUG
	DebugPrint("VOPMproc::processReplacing *outputs[0]=%f",
		**outputs
	);
#endif
}

//-----------------------------------------------------------------------------------------
VstInt32 VOPM::processEvents (VstEvents* ev)
{
	for (long i = 0; i < ev->numEvents; i++)
	{
		if ((ev->events[i])->type != kVstMidiType)
			continue;
		VstMidiEvent* event = (VstMidiEvent*)ev->events[i];
		char* midiData = event->midiData;
		pOPMdrv->setDelta((unsigned int)event->deltaFrames);
		unsigned char status = midiData[0] & 0xf0;		// ignoring channel
		char MidiCh = midiData[0] & 0x0f;
		char note = midiData[1] & 0x7f;
		char velocity = midiData[2] & 0x7f;
/*
#if DEBUG
	DebugPrint("VOPMproc::ProcessEvents Message[%02x %02x %02x]",
	midiData[0],midiData[1],midiData[2]);
#endif
*/
		switch(status){
			case 0x80:
				pOPMdrv->NoteOff(MidiCh,note);
				break;
			case 0x90:
				if(velocity==0){
					pOPMdrv->NoteOff(MidiCh,note);
				}else{
					pOPMdrv->NoteOn(MidiCh,note,velocity);
				}
				break;
			case 0xb0:
				switch(note){
				case 0x01://LFO AMD_H
				case 0x0c:
					pOPMdrv->setCcAMD_H(MidiCh,velocity);
					break;
				case 0x21://LFO AMD_L
				case 0x2c:
					pOPMdrv->setCcAMD_L(MidiCh,velocity);
					break;
				case 0x02://LFO PMD_H
				case 0x0d:
					pOPMdrv->setCcPMD_H(MidiCh,velocity);
					break;
				case 0x22://LFO PMD_L
				case 0x2d:
					pOPMdrv->setCcPMD_L(MidiCh,velocity);
					break;

				case 0x03://LFO rate_H
					pOPMdrv->setCcLFQ_H(MidiCh,velocity);
					break;
				case 0x23://LFO rate_L
					pOPMdrv->setCcLFQ_L(MidiCh,velocity);
					break;

				case 0x05://PortaTime
					pOPMdrv->setPortaSpeed(MidiCh,velocity);
					break;
				case 0x06://DataH Entr
					pOPMdrv->setData(MidiCh,velocity);
					break;
				case 0x0A://pan
					pOPMdrv->PanCng(MidiCh,velocity);
					break;

				case 0x10://TL0_H
					pOPMdrv->setCcTL_H(MidiCh,velocity,0);
					break;
				case 0x30://TL0_L
					pOPMdrv->setCcTL_L(MidiCh,velocity,0);
					break;
				case 0x11://TL1_H
					pOPMdrv->setCcTL_H(MidiCh,velocity,1);
					break;
				case 0x31://TL1_L
					pOPMdrv->setCcTL_L(MidiCh,velocity,1);
					break;
				case 0x12://TL2_H
					pOPMdrv->setCcTL_H(MidiCh,velocity,2);
					break;
				case 0x32://TL2_L
					pOPMdrv->setCcTL_L(MidiCh,velocity,2);
					break;
				case 0x13://TL3_H
					pOPMdrv->setCcTL_H(MidiCh,velocity,3);
					break;
				case 0x33://TL3_L
					pOPMdrv->setCcTL_L(MidiCh,velocity,3);
					break;

				case 0x07://ch-vol
					pOPMdrv->VolCng(MidiCh,velocity);
					break;
				case 0x41://portaOnOff
					pOPMdrv->setPortaOnOff(MidiCh,(velocity>=64)?true:false);
					break;
				case 0x4C://LFO rate
					pOPMdrv->setCcLFQ_H(MidiCh,velocity);
					//updateDisplay();
					if(editor)((class VOPMEdit *)editor)->update();
					break;
				case 0x4E://LFO delay
					pOPMdrv->setLFOdelay(MidiCh,velocity);
					break;
				case 0x54://Porta Ctr
					pOPMdrv->setPortaCtr(MidiCh,velocity);
					break;
				case 0x62://NRPNL
					pOPMdrv->setNRPNL(MidiCh,velocity);
					break;
				case 0x63://NRPNH
					pOPMdrv->setNRPNH(MidiCh,velocity);
					break;

				case 0x64://RPNL
					pOPMdrv->setRPNL(MidiCh,velocity);
					break;
				case 0x65://RPNH
					pOPMdrv->setRPNH(MidiCh,velocity);
					break;
				case 0x78://Force off
					pOPMdrv->ForceAllOff(MidiCh);
					break;
				case 0x7B://All Note Off
					pOPMdrv->NoteAllOff(MidiCh);
					break;
				case 0x7E://Mono
					pOPMdrv->setPoly(false);
					break;
				case 0x7F://Poly
					pOPMdrv->setPoly(true);
					break;
				default:
					
					if(editor){
						long lTag;
						lTag=((class VOPMEdit *)editor)->getTag();
						if(lTag!=-1){
							lTblCcLearn[note&0x7f]=lTag;
#if DEBUG
	DebugPrint("VOPMproc::learnIn Cc=%x Tag=%d",note,lTag);
#endif
						}else{
							lTag=lTblCcLearn[note&0x7f];
#if DEBUG
	DebugPrint("VOPMproc::learnOut Cc=%x Tag=%d",note,lTag);
#endif
						}
						
						setParameter((VstInt32)lTag,(float)velocity/127.0);
#if DEBUG
	DebugPrint("VOPMproc::defaultCC CcNo=%x tag=%d float=%f",note,lTag,(float)velocity/127.0);
#endif
						((class VOPMEdit *)editor)->update();
					}

				}

				break;
			case 0xc0:
				pOPMdrv->VoCng(MidiCh,note);
				break;
			case 0xe0:
					pOPMdrv->BendCng(MidiCh,(velocity<<7)+note);
				break;
		}
		//event++;
	}
	return 1;	// want more
}

//-----------------------------------------------------------------------------------------

