//---------------------------------------------------------------------------
#include "vstgui.h"
#include "vstcontrols.h"
#include "disp.h"
#include "VOPM.hpp"

#include "ResVOPM.h"

#define X_POS 375
#define Y_POS 22
#define EGBSIZE 96
#if DEBUG
extern void DebugPrint (char *format, ...);
#endif
//

CDisp::~CDisp(){
/*	if(pCOffScreen){delete pCOffScreen;}
	if(pConImage){delete pConImage;}
	if(pCOffScreen){delete pCOffScreen;}
	if(pBgImage){delete pBgImage;}
*/
}

CDisp::CDisp(CRect &size,OPMDRV *pO):CView(size){
	pBgImage=new CBitmap(IDI_BG);
	pConImage=new CBitmap(IDI_CON);

	pOPMdrv=pO;
	EGmode=0;
}

void CDisp::draw(CDrawContext *context){

	EGBPaint(context);
	ConPaint(context,pOPMdrv->getPrm(kCon));
}


void CDisp::EGBPaint(CDrawContext *context){
	int num,def;
	int n,m;
	float Ax,Ay;
	float D1x,D1y;
	float D2x,D2y;
	float Sx,Sy;
	float Rx,Ry;
	float div;
	int TL,AR,D1R,D2R,D1L,RR;
	int Base;

#ifdef WIN32
	COffscreenContext *Oftext=new COffscreenContext(context,pBgImage,false);
#else
	COffscreenContext *Oftext=new COffscreenContext(context,pBgImage,true);
#endif
	CColor kPapCColor={0x80,0x00,0x80,0x00};
	CColor kCaCColor={0x80,0x00,0x00,0x00};
	//EGカーブ座標計算
	Base=0;
	CRect DsRect(X_POS,Y_POS,X_POS+EGBSIZE,Y_POS+EGBSIZE);
	CRect DoRect(0,0,EGBSIZE,EGBSIZE);
	pBgImage->draw(Oftext,DoRect,CPoint(X_POS,Y_POS));
	for(num=0;num<4;num++){
		TL=pOPMdrv->getPrm(kM1TL+Base);
		AR=pOPMdrv->getPrm(kM1AR+Base);
		D1R=pOPMdrv->getPrm(kM1D1R+Base);
		D2R=pOPMdrv->getPrm(kM1D2R+Base);
		D1L=pOPMdrv->getPrm(kM1D1L+Base);
		RR=pOPMdrv->getPrm(kM1RR+Base);
		Base+=(kM1F_AME-kM1TL+1);
		if(D1L==0xf){D1L=0x1f;}
	 //ペン色変更

		switch(num){
			case 0:Oftext->setFrameColor(kPapCColor);break;
			case 1:Oftext->setFrameColor(kBlueCColor);break;
			case 2:Oftext->setFrameColor(kCaCColor);break;
			case 3:Oftext->setFrameColor(kRedCColor);break;
		}

	//各座標計算
		if(AR==0 || TL==127){
	//AR==0 || TL==127は無音、直線１本
			Oftext->moveTo(CPoint(0,EGBSIZE));
			Oftext->lineTo(CPoint(EGBSIZE,EGBSIZE));
		}else{
			Ay=(float)TL;
			Ax=(127.0-(float)TL)/(5.0*(float)AR);
			if((D1L*4)>TL){
				D1y=4.0*(float)D1L;
				if(D1R!=0){
					D1x=(D1y-(float)Ay)/(float)D1R+Ax;
				}else{
					D1y=Ay;
					D1x=3.0*127.0/4.0;
				}
			}else{
				D1x=Ax;
				D1y=Ay;
			}

			D2x=3.0*127/4.0;
			
			if(D1R!=0){
				D2y=(float)D2R*(D2x-D1x)+D1y;
			}else{
				D2y=D1y;
			}

			if(RR==0){
				Rx=127.0;
				Ry=D2y;
			}else{
				Rx=(127.0-D2y)/(2.0*(float)RR)+D2x;
				Ry=127.0;
			}

			if(Rx>127.0){
				Rx=127.0;
				Ry=D2y+2.0*(float)RR*(Rx-D2x);
			}
	
	//スケーリング
			if(EGmode==0){
				div=127.0;
				def=0;
			}else{
				div=7.0;
				def=(EGmode-1)*96;
			}

			if(D1y>127.0)D1y=127.0;
			if(D2y>127.0)D2y=127.0;
			if(Ry>127.0)Ry=127.0;
			if(D2x<D1x)D2x=D1x;
			if(Rx<D2x)Rx=D2x;

			Ax=(Ax*(float)EGBSIZE)/div;
			Ay=(Ay*(float)EGBSIZE)/127.0;
			D1x=(D1x*(float)EGBSIZE)/div;
			D1y=(D1y*(float)EGBSIZE)/127.0;
			D2x=(D2x*(float)EGBSIZE)/div;
			D2y=(D2y*(float)EGBSIZE)/127.0;
			Rx=(Rx*(float)EGBSIZE)/div;
			Ry=(Ry*(float)EGBSIZE)/127.0;

	//線描画
			Oftext->moveTo(CPoint(-def,EGBSIZE));
			Oftext->lineTo(CPoint((int)Ax-def,(int)Ay));
			Oftext->lineTo(CPoint((int)D1x-def,(int)D1y));
			Oftext->lineTo(CPoint((int)D2x-def,(int)D2y));
			Oftext->lineTo(CPoint((int)Rx-def,(int)Ry));
		}
	}

	Oftext->copyFrom(context,DsRect,CPoint(0,0));
	delete Oftext;
}

void CDisp::ConPaint(CDrawContext* context,unsigned char Con){

#define X_CON 225
#define Y_CON 25

	CPoint SPoint(96*Con,0);
	CRect DRect(X_CON,Y_CON,X_CON+96,Y_CON+96);
	pConImage->draw(context,DRect,SPoint);
}

/*ＥＧ拡大表示*/
void CDisp::mouse(CDrawContext *context,CPoint &where,long button)
{
	int i,X,Y;
	
//	long button=context->getMouseButtons();

	while(button&kLButton){
		X=where.h;
		Y=where.v;
		for(i=0;i<16;i++){
			if(((X>=X_POS+6*i)&&(X<(X_POS+6*(i+1))))&&((Y>Y_POS)&&(Y<(Y_POS+96)))){
				EGmode=i+1;
			}
		}

		EGBPaint(context);
		getFrame()->doIdleStuff ();
//		getParent()->doIdleStuff ();
		button=context->getMouseButtons();
	}
	
	EGmode=0;
	EGBPaint(context);
}

