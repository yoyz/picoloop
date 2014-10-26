//OPMdrv.cpp
//Midi->OPM パラメータ、音色管理
//2002.3.11 sam
//based MiOPMdrv

#include "OPMdrv.hpp"
#include "OPM.hpp"
#include <stdio.h>
#include <ctype.h>
#include "VOPM.hpp"

#include "globhead.h"

#if DEBUG
extern void DebugPrint (char *format, ...);
#endif

//Class LFODATA
//LFO Data Class
const int Ktbl[12]={0,1,2,4,5,6,8,9,10,12,13,14};


#define _iocs_opmset(RegNo,data) pOPM->OpmPoke(RegNo,data,SampleTime)

//Class OPDATA
//Operater Data Class

OPDATA::OPDATA(void){
	//EG-Level
	TL=0x00;
	D1L=0x0;
	//EG-ADR
	AR=0x1f;
	D1R=0x00;
	D2R=0x00;
	RR=0x4;
	//DeTune
	KS=0;
	DT1=0;
	MUL=1;
	DT2=0;
	F_AME=0;
}

//Class CHDATA
//chData（音色） クラス
CHDATA::CHDATA(void){
	F_PAN=0x40;
	CON=0;
	FL=0;
	AMS=0;
	PMS=0;
	OpMsk=0x40;
	F_NE=0;
	SPD=0;
	PMD=0;
	AMD=0;
	WF=0;
	NFQ=0;
	strcpy(Name,"init");
}


//Class OPMDRV
//Midi to OPM Emu Driver
void OPMDRV::setDelta(unsigned int Delta){
	SampleTime=Delta;
}

void OPMDRV::AutoSeq(int sampleframes){
//ポルタメント用
//AutoSeqはOPMの波形レンダリング処理からコールバックされる。
//次回のレンダリング開始時に、
//この関数が発行したコマンドをすべて実行させる。
	int i;
	int Porta;
	int Cnt;
	class CHDATA *pCh;
	setDelta(0);
	for(i=0;i<CHMAX;i++){
		Porta=TblCh[i].PortaBend;
		if(Porta!=0){
			if(Porta<-1023){
				Porta+=TblCh[i].PortaSpeed;
			}else{
				if(Porta>+1023){
					Porta-=TblCh[i].PortaSpeed;
				}else{Porta=0;}
			}

			TblCh[i].PortaBend=Porta;
			SendKc(i);
		}
		Cnt=TblCh[i].LFOdelayCnt;
		if(Cnt!=0){
			if(Cnt==1){
				pCh=&(VoTbl[TblCh[i].VoNum]);
				_iocs_opmset(0x00,i);
				_iocs_opmset(0x01,0x02);//LfoReset
				_iocs_opmset(0x38+i,(pCh->PMS<<4)|(pCh->AMS&0x3));//PMS/AMS
				_iocs_opmset(0x01,0x00);//LfoStart
			}
			Cnt--;
		}
		TblCh[i].LFOdelayCnt=Cnt;
	}

}


void OPMDRV::setProgNum(long ProgNum){
	int i;
	CuProgNum=ProgNum;
#ifdef DEBUG
	DebugPrint("OPMdrv::setProgNum %d",ProgNum);
#endif
	for(i=0;i<16;i++){
		TblMidiCh[i].VoNum=ProgNum;
	};

}
int OPMDRV::getProgNum(void){
		return((int)CuProgNum);
}
void OPMDRV::setProgName(char *Name){
	strcpy(VoTbl[CuProgNum].Name,Name);
	strcpy(CuProgName,Name);
}

char* OPMDRV::getProgName(void){
	strcpy(CuProgName,VoTbl[CuProgNum].Name);
	return(CuProgName);
}
char* OPMDRV::getProgName(int index){
	return(VoTbl[index].Name);
}

void OPMDRV::setRPNH(int ch,int data){
	TblMidiCh[ch].RPN&=0x007f;
	TblMidiCh[ch].RPN|=(data&0x007f)<<8;
	TblMidiCh[ch].CuRPN=true;

}
void OPMDRV::setRPNL(int ch,int data){
	TblMidiCh[ch].RPN&=0x7f00;
	TblMidiCh[ch].RPN|=data&0x007f;
	TblMidiCh[ch].CuRPN=true;
}
void OPMDRV::setNRPNH(int ch,int data){
	TblMidiCh[ch].NRPN&=0x007f;
	TblMidiCh[ch].NRPN|=(data&0x007f)<<8;
	TblMidiCh[ch].CuRPN=false;
}
void OPMDRV::setNRPNL(int ch,int data){
	TblMidiCh[ch].NRPN&=0x7f00;
	TblMidiCh[ch].NRPN|=data&0x007f;
	TblMidiCh[ch].CuRPN=false;
}

void OPMDRV::setData(int ch,int data){
	if(TblMidiCh[ch].CuRPN==true){
	//RPN Data Entr
		switch(TblMidiCh[ch].RPN){
			case 0x0000://ピッチベンドセンス
				if(data<=0 || data>=96){data=2;}
				TblMidiCh[ch].BendRang=data;
				break;
		}
	}else{
	//NRPN Data Entr
		switch(TblMidiCh[ch].NRPN){
			case 0x0000://OPMクロック設定
				if(data<=0x3f){
					//OPM Clock=3.58MHz:default
					pOPM->SetOpmClock(3580000);
					OpmClock=0;
				}else{
					if(data<=0x5f){
						//OPM Clock=3.579545MHz
						pOPM->SetOpmClock(4000000);
						OpmClock=2;
					}else{
						//OPM Clock=4MHz
						pOPM->SetOpmClock(4000000);
						OpmClock=1;
					}
				}
				break;
			case 0x0001://音量バイアス設定
				_iocs_opmset(0x5,data);
				break;
			case 0x0002://LPF ON/OFF設定
				if(data<=0x3f){
					//ON:default
					_iocs_opmset(0x6,0);
				}else{
					//OFF
					_iocs_opmset(0x6,1);
				}
				break;
		}
	}
}
	
void OPMDRV::SendKc(int ch){
 unsigned int Word,Note;
 unsigned char kc,kf;
 unsigned char MidiCh;
 MidiCh=TblCh[ch].MidiCh;
 Note=TblCh[ch].Note;
 if(Note<0x80){
	Word=(Note<<16)+((TblMidiCh[MidiCh].Bend<<3)*TblMidiCh[MidiCh].BendRang)+TblCh[ch].PortaBend;	//16.16bit固定小数点
	switch(OpmClock){
		case 1://4MHz
			//OPM Clockが4MHzの場合、周波数を補正 12*log2(4MHz/3.58MHz)*65536<16.16bit固定小数点のoffset>
			Word-=125861;
			break;
		case 2://3.579545MHzの場合、周波数を補正 12*log2(3.579545MHz/3.58MHz)*65536<16.16bit固定小数点のoffset>
			Word+=144;
			break;
	}

	Note=Word>>16;
//	kc=(Ktbl[(Note-3)%12])+((int)((Note-3)/12)*16); //Kc[Oct::key]の算出

	 kc=(Ktbl[(Note-1)%12])+((int)((Note-1)/12)*16); //Kc[Oct::key]の算出
	kf=(Word>>8)&0xfc;							 //Kf[ベンド微調節]の算出
	_iocs_opmset(0x28+ch,kc);	// 音階をOPMレジスタに書き込む
	_iocs_opmset(0x30+ch,kf);	//	ベンドをOPMレジスタに書き込む
 }
 #ifdef MyDEBUG
	fprintf(stdout,"kc:%02x kf:%02x\n",kc,kf);
 #endif
}

void OPMDRV::SendPan(int ch,int F_PAN){
	unsigned char tmp;
	int VoNum;
	ch&=0x07;
	VoNum=TblCh[ch].VoNum;
	tmp=(VoTbl[VoNum].CON&0x07)|((VoTbl[VoNum].FL&0x7)<<3);
	_iocs_opmset(0x20+ch,tmp);
	_iocs_opmset(0x00,ch);
	_iocs_opmset(0x03,F_PAN);
}

void OPMDRV::SendVo(int ch){
int tmp,i;
class CHDATA *pCh;
class OPDATA *pOp;
	ch&=0x07;
	pCh=&(VoTbl[TblCh[ch].VoNum]);
	//_iocs_opmset(0x38+ch,(pCh->PMS<<4)|(pCh->AMS&0x3));//PMS/AMS
	for(i=0;i<4;i++){
		tmp=(i<<3)+ch;
		pOp=&(pCh->Op[i]);
		_iocs_opmset(0x40+tmp,(pOp->DT1<<4)|(pOp->MUL&0x0f));	//DT1/MUL
		_iocs_opmset(0x80+tmp,(pOp->KS<<6)|(pOp->AR&0x1f));	 //KS/AR
		_iocs_opmset(0xA0+tmp,(pOp->F_AME)|(pOp->D1R&0x1f)); //AME/D1R
		_iocs_opmset(0xC0+tmp,(pOp->DT2<<6)|(pOp->D2R&0x1f));	 //DT2/D2R
		_iocs_opmset(0xE0+tmp,(pOp->D1L<<4)|(pOp->RR&0x0f));	//D1L/RR
	}
}

void OPMDRV::SendLfo(int ch){
	class CHDATA *pCh;
	int tmp;
	pCh=&(VoTbl[TblCh[ch].VoNum]);
	_iocs_opmset(0x00,ch);
	_iocs_opmset(0x1B,pCh->WF);
}


OPMDRV::OPMDRV(class Opm *pTmp){
	int i;
	PolyMono=true;
	pOPM=pTmp;
	//SLOTテーブル初期化
	for(i=0;i<CHMAX;i++){
		TblCh[i].VoNum=0;
		TblCh[i].MidiCh=i;
		TblCh[i].Note=0xff;	 //Note:0xff 空きチャンネル
		TblCh[i].Vel=0x00;	//消音
		TblCh[i].PortaBend=0;
		TblCh[i].PortaSpeed=0;
		TblCh[i].LFOdelayCnt=0;
	}

 for(i=0;i<CHMAX;i++){
	WaitCh.push_back(i);
 }

	for(i=0;i<16;i++){
		TblMidiCh[i].VoNum=0;
		TblMidiCh[i].Bend=0;
		TblMidiCh[i].Vol=127;
		TblMidiCh[i].Pan=0x40;
		TblMidiCh[i].BendRang=2;
		TblMidiCh[i].SPD=0x8000;
		TblMidiCh[i].PMD=0x4000;
		TblMidiCh[i].AMD=0x4000;
		TblMidiCh[i].BfNote=0xff;
		TblMidiCh[i].RPN=0;
		TblMidiCh[i].NRPN=0;
		TblMidiCh[i].CuRPN=false;
		TblMidiCh[i].PortaSpeed=0;
		TblMidiCh[i].PortaOnOff=false;
		TblMidiCh[i].LFOdelay=0;
		TblMidiCh[i].OpTL[0]=0x4000;
		TblMidiCh[i].OpTL[1]=0x4000;
		TblMidiCh[i].OpTL[2]=0x4000;
		TblMidiCh[i].OpTL[3]=0x4000;

	}
		CuProgNum=0;
		strcpy(CuProgName,"");
		SampleTime=0;
		OpmClock=0;
}



OPMDRV::~OPMDRV(void){
}
void OPMDRV::ChDelay(int delay){
	int i;
	for (i=0;i<16;i++){
	 NoteAllOff(i);
	}

}


void OPMDRV::NoteOn(int MidiCh,int Note,int Vel){
 int ch,i,BfNote,PortaDelta;
 class CHDATA *pCh;
	if(PolyMono==true){
		if((ch=NonCh(MidiCh,Note,Vel))==-1){return;}
	}else{
		MidiCh&=0x07;
		ch=MidiCh;
		BfNote=TblCh[ch].Note;
		TblCh[ch].Note=Note;
		TblCh[ch].Vel=Vel;
		TblCh[ch].MidiCh=MidiCh;
		TblCh[ch].VoNum=TblMidiCh[MidiCh].VoNum;
	}

	TblCh[ch].LFOdelayCnt=TblMidiCh[MidiCh].LFOdelay;
	if(TblMidiCh[MidiCh].PortaSpeed ){
	if((TblMidiCh[MidiCh].PortaOnOff==true)||
		(PolyMono==false && BfNote<=0x7f))
	{
		PortaDelta=(TblMidiCh[MidiCh].BfNote-Note);
		TblCh[ch].PortaBend=PortaDelta*0x00010000;

		if(PortaDelta<0){PortaDelta*=-1;}
		TblCh[ch].PortaSpeed=(PortaDelta*0x10000)/TblMidiCh[MidiCh].PortaSpeed;

	}else{
		TblCh[ch].PortaBend=0;
		TblCh[ch].PortaSpeed=0;
	}
	}else{
	TblCh[ch].PortaBend=0;
	TblCh[ch].PortaSpeed=0;
	}

	//OPM拡張仕様
	pCh=&(VoTbl[TblMidiCh[MidiCh].VoNum]);
//
	_iocs_opmset(0x00,ch);
	_iocs_opmset(0x0F,(pCh->NFQ&0x1f)|(pCh->F_NE&0x80));
	SendLfo(ch);

	setPMD(ch,CalcLim7(pCh->PMD,TblMidiCh[MidiCh].PMD));
	setAMD(ch,CalcLim7(pCh->AMD,TblMidiCh[MidiCh].AMD));
	setSPD(ch,CalcLim8(pCh->SPD,TblMidiCh[MidiCh].SPD));

	SendVo(ch);
	if(TblMidiCh[MidiCh].LFOdelay==0){
		_iocs_opmset(0x01,0x02);//LfoReset								
		_iocs_opmset(0x38+ch,(pCh->PMS<<4)|(pCh->AMS&0x3));//PMS/AMS
		_iocs_opmset(0x01,0x00);//LfoStart 
	}else{
		_iocs_opmset(0x38+ch,0);//PMS/AMS LFOを掛けない
	}
	
	SendPan(ch,TblMidiCh[MidiCh].Pan);
	SendKc(ch);

//TLSend
	setTL(ch,CalcLimTL(pCh->Op[0].TL,TblMidiCh[MidiCh].OpTL[0]),0);
	setTL(ch,CalcLimTL(pCh->Op[1].TL,TblMidiCh[MidiCh].OpTL[1]),1);
	setTL(ch,CalcLimTL(pCh->Op[2].TL,TblMidiCh[MidiCh].OpTL[2]),2);
	setTL(ch,CalcLimTL(pCh->Op[3].TL,TblMidiCh[MidiCh].OpTL[3]),3);

//Vel&Vol Set
	_iocs_opmset(0x02,TblMidiCh[TblCh[ch].MidiCh].Vol);			//Vol拡張セット
	_iocs_opmset(0x04,TblCh[ch].Vel);							//Vel拡張セット

	//_iocs_opmset(0x01, 2);	// LFOをリセット
 if(PolyMono==false && BfNote!=0xff){
		//Monoモードで発音されてる場合、アタックなし（レガート）
	}else{
		_iocs_opmset(0x01, 0);	// LFOをスタート
		_iocs_opmset(0x08,(ch&0x07)+(VoTbl[TblMidiCh[MidiCh].VoNum].OpMsk&0x78));	// キーオン
	}
	 TblMidiCh[MidiCh].BfNote=Note;
	return;
}

void OPMDRV::NoteAllOff(int MidiCh){
int ch;

 while((ch=ChkSonCh(MidiCh,0xff))!=-1){
	_iocs_opmset(0x08, ch);	// キーオフ
	DelCh(ch);
 }
	
}
void OPMDRV::ForceAllOff(void){
int ch,MidiCh;
	for(MidiCh=0;MidiCh<16;MidiCh++){
		while((ch=ChkSonCh(MidiCh,0xff))!=-1){
			DelCh(ch);
		}
	}
	pOPM->ForceNoteOff();
}
void OPMDRV::ForceAllOff(int MidiCh){
int ch;
	while((ch=ChkSonCh(MidiCh,0xff))!=-1){
		_iocs_opmset(0x09, ch);	// キーオフ
		DelCh(ch);
	}
}

void OPMDRV::NoteOff(int MidiCh,int Note){
 int ch;

 if(PolyMono==false){
		MidiCh&=0x07;
		ch=MidiCh;
		if(TblCh[ch].Note==Note){
			_iocs_opmset(0x08,ch);
			TblCh[ch].Note=0xff;
		}
 }else{
	while((ch=ChkSonCh(MidiCh,Note))!=-1){
	_iocs_opmset(0x08, ch);	// キーオフ
	DelCh(ch);
 }
 }
}

void OPMDRV::BendCng(int MidiCh,int Bend){
int i;

 TblMidiCh[MidiCh].Bend=Bend-0x2000;
 for(i=0;i<CHMAX;i++){
	if(TblCh[i].MidiCh==MidiCh && TblCh[i].Note!=0xff){
	 SendKc(i);
	}
 }
}

void OPMDRV::VolCng(int MidiCh,int Vol){
int i;

 TblMidiCh[MidiCh].Vol=Vol;
 for(i=0;i<CHMAX;i++){
	if(TblCh[i].MidiCh==MidiCh && TblCh[i].Note!=0xff){
		_iocs_opmset(0x00,i);				//チャンネル拡張セット
		_iocs_opmset(0x02,Vol);			//Vol拡張セット
	}
 }
}

void OPMDRV::VoCng(int MidiCh,int Vo){
	int i;
	int Spd;
	int Amd;
	int Pmd;

	TblMidiCh[MidiCh].VoNum=Vo;
	//strncpy(MidiProg[MidiCh].name,VoTbl[Vo].Name,15);
	//MidiProg[MidiCh].midiProgram=Vo;

	Spd=CalcLim8(VoTbl[Vo].SPD,TblMidiCh[MidiCh].SPD);
	Amd=CalcLim7(VoTbl[Vo].AMD,TblMidiCh[MidiCh].AMD);
	Pmd=CalcLim7(VoTbl[Vo].PMD,TblMidiCh[MidiCh].PMD);

	for(i=0;i<CHMAX;i++){
		if(TblCh[i].MidiCh==MidiCh && TblCh[i].Note!=0xff){
			SendVo(i);
			SendLfo(i);
			setSPD(i,Spd);
			setAMD(i,Amd);
			setPMD(i,Pmd);
		}
	}
}


void OPMDRV::PanCng(int MidiCh,int Pan){
	int F_Pan,i;
	TblMidiCh[MidiCh].Pan=Pan;
	for(i=0;i<CHMAX;i++){
		if(TblCh[i].MidiCh==MidiCh && TblCh[i].Note!=0xff){
			SendPan(i,Pan);
		}
	}
}

int OPMDRV::Save(char* tmpName){
	int i,j,ch;
	FILE *File;
	class OPDATA *pOp;
	class CHDATA *pCh;
	int TblS[4]={0,2,1,3};
	char name[4][8]={"M1","M2","C1","C2"};

	if((File=fopen(tmpName,"w"))==NULL){
		return(ERR_NOFILE);
	}
	//音色パラメータ書き出し
	fprintf(File,"//MiOPMdrv sound bank Paramer Ver2002.04.22 \n");
	fprintf(File,"//LFO: LFRQ AMD PMD WF NFRQ\n");
	fprintf(File,"//@:[Num] [Name]\n");
	fprintf(File,"//CH: PAN	FL CON AMS PMS SLOT NE\n");
	fprintf(File,"//[OPname]: AR D1R D2R	RR D1L	TL	KS MUL DT1 DT2 AMS-EN\n");

	for(ch=0;ch<VOMAX;ch++){
		pCh=&(VoTbl[ch]);
		fprintf(File,"\n@:%d %s\n",ch,pCh->Name);
		fprintf(File,"LFO:%3d %3d %3d %3d %3d\n",
		pCh->SPD,pCh->AMD,pCh->PMD,pCh->WF,pCh->NFQ);
		fprintf(File,"CH:%3d %3d %3d %3d %3d %3d %3d\n",
		pCh->F_PAN,pCh->FL,pCh->CON,pCh->AMS,pCh->PMS,pCh->OpMsk,pCh->F_NE);
		for(i=0;i<4;i++){
			j=TblS[i];
			pOp=&(pCh->Op[j]);
			fprintf(File,"%s:%3d %3d %3d %3d %3d %3d %3d %3d %3d %3d %3d\n",
			 name[j],pOp->AR,pOp->D1R,pOp->D2R,pOp->RR,pOp->D1L,pOp->TL,
			 pOp->KS,pOp->MUL,pOp->DT1,pOp->DT2,pOp->F_AME);
		}
	}
	fclose(File);
	return(ERR_NOERR);
}
int OPMDRV::Load(char* tmpName){
	int i,itmp,errMsk=0,Nline;
	FILE *File;
	char	line[256];
	char name[64];
	char *pline;
	int CuVoNum;
	class OPDATA *pOp;
	class CHDATA *pCh;
	CuVoNum=0;
	pCh=&(VoTbl[0]);
	pOp=&(VoTbl[0].Op[0]);
	if((File=fopen(tmpName,"r"))==NULL){
		return(ERR_NOFILE);
	}
	//音色パラメータ読み込み
	Nline=0;
	while(fgets(line,256,File)!=NULL){
		Nline++;
		//fprintf(stderr,"line %d:[%s]",Nline,line);
	
		pline=line;
		while(*pline!=0 && *pline!=0x0a){				//行末まで解析繰り返す
			//fprintf(stderr,"*[%c]=%02x\n",*pline,*pline);
			if(strncmp("//",pline,2)==0){
				while(*pline!=0){pline++;}
				//fprintf(stderr,"// comment\n");
				errMsk|=0x08;
			}	//コメント以降は飛ばす

			//スペースを飛ばす
			while(strncmp(" ",pline,1)==0 && pline!=(256+line)){pline++;}

			//@:音色番号設定
			if(strncmp("@:",pline,2)==0){
				pline+=2;
				if(sscanf(pline,"%d",&CuVoNum)!=1){return(ERR_LESPRM+Nline);}
				if(CuVoNum<0){CuVoNum=0;}
				if(CuVoNum>127){CuVoNum&=0x7f;}
				pCh=&(VoTbl[CuVoNum]);

				//文字列[0-9]^n + " "^mを飛ばす
				while(isdigit(*pline)){pline++;} //数字を飛ばす
				while(*pline==' '){pline++;}	 //空白を飛ばす
				strcpy(name,"no Name");
				sscanf(pline,"%[^\n]",&name);
				name[15]='\0';
				strncpy(pCh->Name,name,15);
				errMsk|=0x01;
				while(*pline!=0 && *pline!=0x0a){pline++;}//行末までとばす
			}
			if(strncmp("CH:",pline,3)==0){
				pline+=3;
				if(sscanf(pline,"%d %d %d %d %d %d %d",
					 &(pCh->F_PAN),&(pCh->FL),&(pCh->CON),
					 &(pCh->AMS),&(pCh->PMS),&(pCh->OpMsk),&(pCh->F_NE))
					 !=7){return(ERR_LESPRM+Nline);}
				errMsk|=0x02;
				//fprintf(stderr,"CH exec:");
				while(isdigit(*pline)||*pline==' '){pline++;}
			}

			if(strncmp("LFO:",pline,4)==0){
				pline+=4;
				if(sscanf(pline,"%d %d %d %d %d",
				 &(pCh->SPD),&(pCh->AMD),&(pCh->PMD),&(pCh->WF),&pCh->NFQ)
				!=5){return(ERR_LESPRM+Nline);}
				errMsk|=0x04;
				while(isdigit(*pline)||*pline==' '){pline++;}
			}
			i=-1;
			if(strncmp("M1:",pline,3)==0){i=0;pline+=3;errMsk|=0x10;}
			if(strncmp("M2:",pline,3)==0){i=1;pline+=3;errMsk|=0x20;}
			if(strncmp("C1:",pline,3)==0){i=2;pline+=3;errMsk|=0x40;}
			if(strncmp("C2:",pline,3)==0){i=3;pline+=3;errMsk|=0x80;}
			if(i!=-1){
				pOp=&(VoTbl[CuVoNum].Op[i]);
				if(sscanf(pline,"%d %d %d %d %d %d %d %d %d %d %d",
				 &(pOp->AR),&(pOp->D1R),&(pOp->D2R),&(pOp->RR),&(pOp->D1L),&(pOp->TL),
				 &(pOp->KS),&(pOp->MUL),&(pOp->DT1),&(pOp->DT2),&(pOp->F_AME))
				 !=11){return(ERR_LESPRM+Nline);}
				while(isdigit(*pline)||*pline==' '){pline++;}
			}

			if((errMsk&0xff)==0){
				//fprintf(stderr,"OPMDRV.Load:Syntax Error in %d\n",Nline);
				fclose(File);
				return(ERR_SYNTAX+Nline);
			}else{errMsk=0;}
		}
	}
	fclose(File);
	return(ERR_NOERR);
}

//OPM chanel 管理 テーブル
int OPMDRV::ChkEnCh(void){
 if(WaitCh.size()==0){
		return(-1);
 }else{
		return(WaitCh.front());
 }
}

int OPMDRV::AtachCh(void){
//チャンネルを割り当て
	int NewCh;
	NewCh=ChkEnCh();
	if(NewCh==-1){
		 //空きチャンネルがない場合,先頭をラストにする。
		NewCh=PlayCh.front();
		PlayCh.pop_front();
	}else{
		WaitCh.pop_front();
	}
	PlayCh.push_back(NewCh);
	return(NewCh);
}

void OPMDRV::DelCh(int ch){
 //チャンネルを削除
	TblCh[ch].MidiCh=0xff;
	TblCh[ch].Note=0xff;
	PlayCh.remove(ch);
	WaitCh.push_back(ch);

}

int OPMDRV::ChkSonCh(int MidiCh,int Note){
//MidiChとベロシティより発音中のスロットを探す。
//Note=0xffはワイルドカード
	list<int>::iterator plist;
	if(PlayCh.size()==0){return(-1);}
	plist=PlayCh.begin();
	while(plist!=PlayCh.end()){
	if(MidiCh==TblCh[*plist].MidiCh){
		if(Note==0xff || Note==TblCh[*plist].Note){
			return(*plist);
		}
	}
	plist++;
	}
	return(-1);
}

int OPMDRV::NonCh(int MidiCh,int Note,int Vel){
	int ch;
//MidiChとノート、ベロシティで発音する。
	if(ChkSonCh(MidiCh,Note)!=-1){return(-1);}//発音されてないか？

	ch=AtachCh();	//リストから使えるスロットを探し割り当て
	TblCh[ch].Note=Note;
	TblCh[ch].Vel=Vel;
	TblCh[ch].MidiCh=MidiCh;
	TblCh[ch].VoNum=TblMidiCh[MidiCh].VoNum;
	return(ch);
}

int OPMDRV::NoffCh(int MidiCh,int Note){
	int ch;
//MidiChとノートで消音する。
	if((ch=ChkSonCh(MidiCh,Note))!=-1){//消音されてないか？
		DelCh(ch);
	}

#ifdef MyDEBUG
	fprintf(stdout,"NoffSlot:ch=%x Note=%x MidiCh=%x\n",ch,Note,Ch);
#endif

return(ch);
}


unsigned char OPMDRV::getPrm(int index){
		class CHDATA *pVo;
	pVo=&VoTbl[CuProgNum];

	switch (index)
	{
		case kCon:return(pVo->CON);
		case kFL:return(pVo->FL);
		case kAMS:return(pVo->AMS);
		case kPMS:return(pVo->PMS);
		case kNE:return((pVo->F_NE==128)?1:0);
		case kSPD:return(pVo->SPD);
		case kPMD:return(pVo->PMD);
		case kAMD:return(pVo->AMD);
		case kWF:return(pVo->WF);
		case kNFQ:return(pVo->NFQ);
		case kOpMsk:return(pVo->OpMsk);
		case kM1TL:return(pVo->Op[0].TL);
		case kM2TL:return(pVo->Op[1].TL);
		case kC1TL:return(pVo->Op[2].TL);
		case kC2TL:return(pVo->Op[3].TL);
		case kM1D1L:return(pVo->Op[0].D1L);
		case kM2D1L:return(pVo->Op[1].D1L);
		case kC1D1L:return(pVo->Op[2].D1L);
		case kC2D1L:return(pVo->Op[3].D1L);
		case kM1AR:return(pVo->Op[0].AR);
		case kM2AR:return(pVo->Op[1].AR);
		case kC1AR:return(pVo->Op[2].AR);
		case kC2AR:return(pVo->Op[3].AR);
		case kM1D1R:return(pVo->Op[0].D1R);
		case kM2D1R:return(pVo->Op[1].D1R);
		case kC1D1R:return(pVo->Op[2].D1R);
		case kC2D1R:return(pVo->Op[3].D1R);
		case kM1D2R:return(pVo->Op[0].D2R);
		case kM2D2R:return(pVo->Op[1].D2R);
		case kC1D2R:return(pVo->Op[2].D2R);
		case kC2D2R:return(pVo->Op[3].D2R);
		case kM1RR:return(pVo->Op[0].RR);
		case kM2RR:return(pVo->Op[1].RR);
		case kC1RR:return(pVo->Op[2].RR);
		case kC2RR:return(pVo->Op[3].RR);
		case kM1KS:return(pVo->Op[0].KS);
		case kM2KS:return(pVo->Op[1].KS);
		case kC1KS:return(pVo->Op[2].KS);
		case kC2KS:return(pVo->Op[3].KS);
		case kM1DT1:return(pVo->Op[0].DT1);
		case kM2DT1:return(pVo->Op[1].DT1);
		case kC1DT1:return(pVo->Op[2].DT1);
		case kC2DT1:return(pVo->Op[3].DT1);
		case kM1MUL:return(pVo->Op[0].MUL);
		case kM2MUL:return(pVo->Op[1].MUL);
		case kC1MUL:return(pVo->Op[2].MUL);
		case kC2MUL:return(pVo->Op[3].MUL);
		case kM1DT2:return(pVo->Op[0].DT2);
		case kM2DT2:return(pVo->Op[1].DT2);
		case kC1DT2:return(pVo->Op[2].DT2);
		case kC2DT2:return(pVo->Op[3].DT2);
		case kM1F_AME:return((pVo->Op[0].F_AME==128)?1:0);
		case kM2F_AME:return((pVo->Op[1].F_AME==128)?1:0);
		case kC1F_AME:return((pVo->Op[2].F_AME==128)?1:0);
		case kC2F_AME:return((pVo->Op[3].F_AME==128)?1:0);
//OpMsk
		case kMskM1:return((pVo->OpMsk&0x08)?1:0);
		case kMskC1:return((pVo->OpMsk&0x10)?1:0);
		case kMskM2:return((pVo->OpMsk&0x20)?1:0);
		case kMskC2:return((pVo->OpMsk&0x40)?1:0);
//Con
		case kCon0:case kCon1:case kCon2:case kCon3:
		case kCon4:case kCon5:case kCon6:case kCon7:
			return((pVo->CON==(index-kCon0))?1:0);

//WF
		case kWf0:case kWf1:case kWf2: case kWf3:
			return((pVo->WF==(index-kWf0))?1:0);
	}
		return(0);
}


void OPMDRV::setPrm(int index,unsigned char data){
	class CHDATA *pVo;
	int i;
	pVo=&VoTbl[CuProgNum];
	i=index;
	switch (index)
	{
		case kCon:pVo->CON=data;break;
				case kFL:pVo->FL=data;break;
		case kAMS:pVo->AMS=data;break;
		case kPMS:pVo->PMS=data;break;
				case kNE:pVo->F_NE=(data==1)?128:0;break;
		case kSPD:pVo->SPD=data;break;
		case kPMD:pVo->PMD=data;break;
		case kAMD:pVo->AMD=data;break;
		case kWF:pVo->WF=data;break;
		case kNFQ:pVo->NFQ=data;break;
		case kOpMsk:pVo->OpMsk=data;break;
		case kM1TL:pVo->Op[0].TL=data;break;
		case kM2TL:pVo->Op[1].TL=data;break;
		case kC1TL:pVo->Op[2].TL=data;break;
		case kC2TL:pVo->Op[3].TL=data;break;
		case kM1D1L:pVo->Op[0].D1L=data;break;
		case kM2D1L:pVo->Op[1].D1L=data;break;
		case kC1D1L:pVo->Op[2].D1L=data;break;
		case kC2D1L:pVo->Op[3].D1L=data;break;
		case kM1AR:pVo->Op[0].AR=data;break;
		case kM2AR:pVo->Op[1].AR=data;break;
		case kC1AR:pVo->Op[2].AR=data;break;
		case kC2AR:pVo->Op[3].AR=data;break;
		case kM1D1R:pVo->Op[0].D1R=data;break;
		case kM2D1R:pVo->Op[1].D1R=data;break;
		case kC1D1R:pVo->Op[2].D1R=data;break;
		case kC2D1R:pVo->Op[3].D1R=data;break;
		case kM1D2R:pVo->Op[0].D2R=data;break;
		case kM2D2R:pVo->Op[1].D2R=data;break;
		case kC1D2R:pVo->Op[2].D2R=data;break;
		case kC2D2R:pVo->Op[3].D2R=data;break;
		case kM1RR:pVo->Op[0].RR=data;break;
		case kM2RR:pVo->Op[1].RR=data;break;
		case kC1RR:pVo->Op[2].RR=data;break;
		case kC2RR:pVo->Op[3].RR=data;break;
		case kM1KS:pVo->Op[0].KS=data;break;
		case kM2KS:pVo->Op[1].KS=data;break;
		case kC1KS:pVo->Op[2].KS=data;break;
		case kC2KS:pVo->Op[3].KS=data;break;
		case kM1DT1:pVo->Op[0].DT1=data;break;
		case kM2DT1:pVo->Op[1].DT1=data;break;
		case kC1DT1:pVo->Op[2].DT1=data;break;
		case kC2DT1:pVo->Op[3].DT1=data;break;
		case kM1MUL:pVo->Op[0].MUL=data;break;
		case kM2MUL:pVo->Op[1].MUL=data;break;
		case kC1MUL:pVo->Op[2].MUL=data;break;
		case kC2MUL:pVo->Op[3].MUL=data;break;
		case kM1DT2:pVo->Op[0].DT2=data;break;
		case kM2DT2:pVo->Op[1].DT2=data;break;
		case kC1DT2:pVo->Op[2].DT2=data;break;
		case kC2DT2:pVo->Op[3].DT2=data;break;
		case kM1F_AME:pVo->Op[0].F_AME=(data==1)?128:0;break;
		case kM2F_AME:pVo->Op[1].F_AME=(data==1)?128:0;break;
		case kC1F_AME:pVo->Op[2].F_AME=(data==1)?128:0;break;
		case kC2F_AME:pVo->Op[3].F_AME=(data==1)?128:0;break;
//OpMsk 
		case kMskM1:
			if(data==0){
				pVo->OpMsk&=~0x08;
			}else{
				pVo->OpMsk|=0x08;
			}
			break;
		case kMskC1:
			if(data==0){
				pVo->OpMsk&=~0x10;
			}else{
				pVo->OpMsk|=0x10;
			}
			break;

		case kMskM2:
			if(data==0){
				pVo->OpMsk&=~0x20;
			}else{
				pVo->OpMsk|=0x20;
			}
			break;

		case kMskC2:
			if(data==0){
				pVo->OpMsk&=~0x40;
			}else{
				pVo->OpMsk|=0x40;
			}
			break;
				//Con
				case kCon0:case kCon1:case kCon2:case kCon3:
				case kCon4:case kCon5:case kCon6:case kCon7:
								if(data==1){
										pVo->CON=(i-kCon0);
								}

						break;
//WF
				case kWf0:case kWf1:case kWf2: case kWf3:
						if(data==1){
								pVo->WF=(i-kWf0);
						}
						break;

	}
}

void OPMDRV::setPoly(bool poly){
 int i;
		for(i=0;i<8;i++){
				NoteAllOff(i);
		}
	PolyMono=poly;
}
/*
long OPMDRV::getMidiProgram(long channel,MidiProgramName *curProg){
MidiProg[channel].parentCategoryIndex=-1;	
*curProg=MidiProg[channel];
return(0);
}
*/
void OPMDRV::setPortaSpeed(int MidiCh,int speed){
	if(speed<0){speed=0;}
	if(speed>127){speed=127;}
	TblMidiCh[MidiCh].PortaSpeed=speed;
}

void OPMDRV::setPortaOnOff(int MidiCh,bool onoff){
	TblMidiCh[MidiCh].PortaOnOff=onoff;
}

void OPMDRV::setPortaCtr(int MidiCh,int Note){
	if(Note<0){Note=0;}
	if(Note>127){Note=127;}
	TblMidiCh[MidiCh].BfNote=Note;
}
void OPMDRV::setLFOdelay(int MidiCh,int data){
	if(data<0){data=0;}
	if(data>127){data=127;}
	TblMidiCh[MidiCh].LFOdelay=data;
}

void OPMDRV::setCcTL_L(int MidiCh,int data,int OpNum){
	TblMidiCh[MidiCh].OpTL[OpNum]&=0xff00;
	TblMidiCh[MidiCh].OpTL[OpNum]|=data<<1;
}

void OPMDRV::setCcTL_H(int MidiCh,int data,int OpNum){
	int ch;
	int Tl;
	int tmp;

	OpNum&=0x03;
	tmp=TblMidiCh[MidiCh].OpTL[OpNum];
	tmp&=0x00ff;
	tmp|=data<<8;
	TblMidiCh[MidiCh].OpTL[OpNum]=tmp;	
	Tl=CalcLimTL(VoTbl[TblMidiCh[MidiCh].VoNum].Op[OpNum].TL,tmp);

	for(ch=0;ch<8;ch++){
		if(MidiCh==TblCh[ch].MidiCh && TblCh[ch].Note!=0xff){
			setTL(ch,Tl,OpNum);
		}
	}
}

void OPMDRV::setTL(int ch,int Tl,int OpNum){
			switch(OpNum&0x03){
				case 0x00:
					_iocs_opmset(0x60+ch,Tl);
					break;
				case 0x01:
					_iocs_opmset(0x68+ch,Tl);
					break;
				case 0x02:
					_iocs_opmset(0x70+ch,Tl);
					break;
				case 0x03:
					_iocs_opmset(0x78+ch,Tl);
					break;
			}

}

void OPMDRV::setCcLFQ_L(int MidiCh,int data){
	TblMidiCh[MidiCh].SPD&=0xff00;
	TblMidiCh[MidiCh].SPD|=data<<1;
}
void OPMDRV::setCcLFQ_H(int MidiCh,int data){
	int ch;
	int Spd;
	int tmp;

	tmp=TblMidiCh[MidiCh].SPD;
	tmp&=0x00ff;
	tmp|=data<<8;
	TblMidiCh[MidiCh].SPD=tmp;

	Spd=CalcLim8(VoTbl[TblMidiCh[MidiCh].VoNum].SPD,tmp);

	for(ch=0;ch<8;ch++){
		if(MidiCh==TblCh[ch].MidiCh && TblCh[ch].Note!=0xff){
			setSPD(ch,Spd);
		}
	}
}

void OPMDRV::setSPD(int ch,int Spd){
	_iocs_opmset(0x00,ch);
	_iocs_opmset(0x18,Spd);
}

void OPMDRV::setCcPMD_L(int MidiCh,int data){
	TblMidiCh[MidiCh].PMD&=0xff00;
	TblMidiCh[MidiCh].PMD|=data<<1;
}
void OPMDRV::setCcPMD_H(int MidiCh,int data){
	int ch;
	int Pmd;
	int tmp;

	tmp=TblMidiCh[MidiCh].PMD;
	tmp&=0x00ff;
	tmp|=data<<8;
	TblMidiCh[MidiCh].PMD=tmp;

	Pmd=CalcLim7(VoTbl[TblMidiCh[MidiCh].VoNum].PMD,tmp);

	for(ch=0;ch<8;ch++){
		if(MidiCh==TblCh[ch].MidiCh && TblCh[ch].Note!=0xff){
			setPMD(ch,Pmd);
		}
	}
}

void OPMDRV::setPMD(int ch,int Pmd){
	_iocs_opmset(0x00,ch);
	_iocs_opmset(0x19,(0x80|Pmd));
}

void OPMDRV::setCcAMD_L(int MidiCh,int data){
	TblMidiCh[MidiCh].AMD&=0xff00;
	TblMidiCh[MidiCh].AMD|=data<<1;
}
void OPMDRV::setCcAMD_H(int MidiCh,int data){
	int ch;
	int Amd;
	int tmp;
	tmp=TblMidiCh[MidiCh].AMD;
	tmp&=0x00ff;
	tmp|=data<<8;
	TblMidiCh[MidiCh].AMD=tmp;

	Amd=CalcLim7(VoTbl[TblMidiCh[MidiCh].VoNum].AMD,tmp);

	for(ch=0;ch<8;ch++){
		if(MidiCh==TblCh[ch].MidiCh && TblCh[ch].Note!=0xff){
			setAMD(ch,Amd);
		}
	}
}

void OPMDRV::setAMD(int ch,int Amd){
	_iocs_opmset(0x00,ch);
	_iocs_opmset(0x19,Amd);
}

int OPMDRV::CalcLim8(int VoData,int DifData){
	int ret;
	ret=VoData+(((DifData&0x7fff)-0x4000)>>7);
	if(ret>0xff){ret=0xff;}
	if(ret<0x0){ret=0x0;}
	return(ret);
}
int OPMDRV::CalcLim7(int VoData,int DifData){
	int ret;
	ret=VoData+(((DifData&0x7fff)-0x4000)>>8);
	if(ret>0x7f){ret=0x7f;}
	if(ret<0x0){ret=0x0;}
	return(ret);
}
int OPMDRV::CalcLimTL(int VoData,int DifData){
	int ret;
	ret=VoData+(((DifData&0x7fff)-0x4000)>>8);
	if(ret>0x7f){ret=0x7f;}
	if(ret<0x0){ret=0x0;}
	return(ret);
}
