//---------------------------------------------------------------------------
#include "vstgui.h"
#include "vstcontrols.h"
#include "form.h"
#include "disp.h"
#include "Slider.hpp"
#include "stdio.h"
#include "cfileselector.h"
#include "ResVOPM.h"
#include "globhead.h"

#if DEBUG
extern void DebugPrint (char *format, ...);
#endif
//---------------------------------------------------------------------------
int TblOpPrmMax[10]={
//TL,AR,D1R,D1L,D2R,RR,KS,MUL,DT1,DT2
	127,31,31,15,31,15,3,15,7,3
};

int TblProgMax[4]={
	7,3,7,31
};
int TblLfoMax[3]={
	255,127,127
};

#define DISP_X 200
#define DISP_Y 20
#define DISP_WIDTH 280
#define DISP_HEIGHT 100

//---------------------------------------------------------------------------

CForm::~CForm(void)
{
	removeAll();
}

CForm::CForm(CRect &size, void *systemWindow, AEffGUIEditor *editor,class OPMDRV *p,VOPM *e)
		: CFrame(size,systemWindow,editor)
{
	int x,y;

	CPoint offset(0,0);
	pOPMdrv=p;
	pVOPM=e;
	pKnobImage=new CBitmap(IDI_KNOB);
	pSliderBgImage=new CBitmap(IDI_SLIDERBG);
	pBtnImage=new CBitmap(IDI_BTN);
	pHdswImage= new CBitmap(IDI_HDSW);
	pEditor=editor;

	//ディスプレイ登録
#define X_CON 225
#define Y_CON 25
	CRect DispSize (DISP_X,DISP_Y,DISP_X+DISP_WIDTH,DISP_Y+DISP_HEIGHT);
	pDisp= new CDisp(DispSize,p);
	addView(pDisp);

	int i,j,index;
	//Hex-Dec表示選択ボタン登録
	x=15;
	y=42;
	pHdsw=new COnOffButton(
		CRect(x,y,x+pHdswImage->getWidth(),y+pHdswImage->getHeight()/2),
		this,
		kHdsw,
		pHdswImage
	);
	addView(pHdsw);
	//PROG SLIDを登録
	for(index=kFL,j=0;index<=kNFQ;index++,j++){
		x=20+j*42;
		y=220;
		pSlid2[j]=new CDispSlider(
			CRect(x,y,x+pSliderBgImage->getWidth(),y+pSliderBgImage->getHeight()),
			this,index,
			TblProgMax[j],
			0,
			0,
			pKnobImage,
			pSliderBgImage
		);
		addView(pSlid2[j]);
	}
	//LFO SLIDを登録
	for(index=kSPD,j=0;index<=kPMD;index++,j++){
		x=80+j*32;
		y=370;
		pSlid3[j]=new CDispSlider(
			CRect(x,y,x+pSliderBgImage->getWidth(),y+pSliderBgImage->getHeight()),
			this,index,
			TblLfoMax[j],
			0,
			0,
			pKnobImage,
			pSliderBgImage
		);
		addView(pSlid3[j]);
	}

	//WFボタンを登録
	for(index=kWf0,j=0;index<=kWf3;index++,j++){
		x=20;
		y=375+j*20;

		pWFBtn[j]=new class COnOffButton(
			CRect(x,y,x+pBtnImage->getWidth(),y+(pBtnImage->getHeight()/2)),
			this,index,pBtnImage);
		pWFBtn[j]->setTransparency (true);
		addView(pWFBtn[j]);
	}

	//CONボタンを登録
	for(index=kCon0,j=0;index<=kCon7;index++,j++){
		x=196+j*22;
		y=127;
		pConBtn[j]=new class COnOffButton(
			CRect(x,y,x+pBtnImage->getWidth(),y+(pBtnImage->getHeight()/2)),
			this,index,pBtnImage);
		pConBtn[j]->setTransparency (true);
		addView(pConBtn[j]);
	}
	//NEボタンを登録
	x=150;
	y=180;
	pNEBtn = new class COnOffButton(
			CRect(x,y,x+pBtnImage->getWidth(),y+(pBtnImage->getHeight()/2)),
			this,kNE,pBtnImage);
	pNEBtn->setTransparency(true);
	addView(pNEBtn);

	index=kM1TL;
	for(j=0;j<4;j++){
		for(i=0;i<10;i++){
			x=240+i*46;
			y=207+70*j;
			pSlid[j][i] =new CDispSlider(
				CRect(x,y,x+pSliderBgImage->getWidth(),y+pSliderBgImage->getHeight()),
				this,index,
				TblOpPrmMax[i],
				0,
				0,
				pKnobImage,
				pSliderBgImage
			);
			addView(pSlid[j][i]);
			index++;
		}

	//AMEボタンを登録
		x=j*22+84;
		y=156;
		pAMEBtn[j]=new class COnOffButton(
			CRect(x,y,x+pBtnImage->getWidth(),y+(pBtnImage->getHeight()/2)),
			this,index,pBtnImage);
		pAMEBtn[j]->setTransparency (true);
		addView(pAMEBtn[j]);
		index++;
	}

	for(index=kMskM1,j=0;index<=kMskC2;index++,j++){
		//OPMSKボタンを登録
		x=j*22+84;
		y=136;
		pMskBtn[j]=new class COnOffButton(
			CRect(x,y,x+pBtnImage->getWidth(),y+(pBtnImage->getHeight()/2)),
			this,index,pBtnImage);
		pMskBtn[j]->setTransparency (true);
		addView(pMskBtn[j]);
	}
		//Fileボタン
	for(index=kFileLoad,j=0;index<=kFileSave;index++,j++){
		x=15;
		y=10+16*j;
		pFileBtn[j]=new class COnOffButton(
			CRect(x,y,x+pBtnImage->getWidth(),y+(pBtnImage->getHeight()/2)),
			this,index,pBtnImage);
		pFileBtn[j]->setTransparency (true);
		addView(pFileBtn[j]);
	}
	char string[16];
	CRect NumSize(148,90,168,106);	

	pProgDisp=new class CTextLabel(NumSize,"000",NULL,0);
	addView(pProgDisp);
	setDirty();
}

//---------------------------------------------------------------------------

void CForm::Load(void)
{
	int Res,index;

	char	buf[256];
	static char	InBuf[MAX_PATH];
	
	sprintf(InBuf,"*.opm");

	if (((AudioEffectX *)pEditor->getEffect())->canHostDo ("openFileSelector")==1){
		struct VstFileSelect Filedata;
		VstFileType opmType ("OPM File", "OPM", "opm", "opm");
		CFileSelector OpenFile((AudioEffectX *)pEditor->getEffect());

		Filedata.command=kVstFileLoad;
		Filedata.returnPath=InBuf;
		Filedata.fileTypes=&opmType;
		Filedata.nbFileTypes=1;
		Filedata.type= kVstFileType;
		Filedata.initialPath = 0;
		strcpy (Filedata.title, "Import OPM Tone data by txt");

		OpenFile.run(&Filedata);
		sprintf(buf,"%s",Filedata.returnPath);
	}else{
		#ifdef WIN32
			const char FileOpt[32]="OPM File {*.opm}\0.opm\0\0\0";
			const char FileTitle[32]="Import OPM Tone data by txt";	
				//VST openFileSelectorが未対応の場合,Win32APIで試す
			static OPENFILENAME ofn;
			memset(&ofn,0,sizeof(OPENFILENAME));
			ofn.lStructSize=sizeof(OPENFILENAME);
			ofn.Flags = OFN_HIDEREADONLY;
			ofn.nFilterIndex = 1;
			ofn.hwndOwner=(HWND)getSystemWindow();
			ofn.lpstrFilter =FileOpt;
			ofn.lpstrFile =InBuf;
			ofn.nMaxFile=MAX_PATH;
			ofn.lpstrFileTitle=(CHAR*)FileTitle;
			ofn.nMaxFileTitle=MAX_PATH;
			ofn.lpstrDefExt="opm";

			if(GetOpenFileName((LPOPENFILENAME)&ofn)){
			  InvalidateRect(ofn.hwndOwner,NULL,true);

			}else{

			}
			if(ofn.lpstrFile){
				sprintf(buf,"%s",ofn.lpstrFile);
			}
		#else
				//機種特有のFileSelectorを呼び出し、
				//フルパスのファイル名をchar buf[]に返す
			return;
		#endif
		}
		Res=pOPMdrv->Load(buf);

		switch(Res&0xffff0000){
			
			case ERR_NOERR:
				sprintf(buf,"Load Complite.");
				break;

			case ERR_NOFILE:
				sprintf(buf,"Load Err can't find File.");
				break;

			case ERR_SYNTAX:
				sprintf(buf,"Load Err Syntax in %d",Res&0xffff);
				break;

			case ERR_LESPRM:
				sprintf(buf,"Load Err Less Parameter in %d",Res&0xffff);
				break;
			default :
				sprintf(buf,"Load ???");
		}
#ifdef DEBUG
DebugPrint(buf);
#endif
	setDirty();
(AudioEffectX *)pVOPM->updateDisplay();
}

void CForm::Save(void)
{
	struct VstFileSelect Filedata;
	int Res;
	char	buf[256];
	char	 InBuf[256];
	
	sprintf(InBuf,"*.opm");

	if (((AudioEffectX *)pEditor->getEffect())->canHostDo ("openFileSelector")==1){
		VstFileType opmType ("OPM File", "OPM", "opm", "opm");
		CFileSelector SaveFile((AudioEffectX *)pEditor->getEffect());

		Filedata.returnPath=InBuf;
		Filedata.command=kVstFileSave;
		Filedata.fileTypes=&opmType;
		Filedata.nbFileTypes=1;
		Filedata.type				= kVstFileType;
		Filedata.initialPath = 0;
		strcpy (Filedata.title, "Export OPM Tone data by txt");
		SaveFile.run(&Filedata);
		sprintf(buf,"%s",Filedata.returnPath);
	}else{
	#ifdef WIN32
		const char FileOpt[32]="OPM File {*.opm}\0.opm\0\0\0";
		const char FileTitle[32]="Export OPM Tone data by txt";		
		//VST saveFileSelectorが未対応の場合,Win32APIで試す
		OPENFILENAME ofn;
		memset(&ofn,0,sizeof(OPENFILENAME));
		ofn.lStructSize=sizeof(OPENFILENAME);
		ofn.hwndOwner=(HWND)getSystemWindow();
		ofn.lpstrFilter =FileOpt;
		ofn.lpstrFile =InBuf;
		ofn.nMaxFile=MAX_PATH;
		ofn.lpstrFileTitle=(CHAR*)FileTitle;
		ofn.nMaxFileTitle=MAX_PATH;
		ofn.lpstrDefExt="opm";
		if(GetSaveFileName(&ofn)){
			InvalidateRect(ofn.hwndOwner,NULL,true);
		}
		sprintf(buf,"%s",ofn.lpstrFile);
	#else
		//機種特有のFileSelectorを呼び出し、
		//char buf[]にフルパスのファイル名を返す。
		return;
	#endif
	}
	Res=pOPMdrv->Save(buf);

	switch(Res&0xffff0000){
		case ERR_NOERR:
			sprintf(buf,"Save Complite.");
			break;

		case ERR_NOFILE:
			sprintf(buf,"Err can't find File.");
			break;

		case ERR_SYNTAX:
			sprintf(buf,"Err Syntax in %d",Res&0xffff);
			break;

		case ERR_LESPRM:
			sprintf(buf,"Err Less Parameter in %d",Res&0xffff);
			break;
	}
#ifdef DEBUG
DebugPrint(buf);
#endif
	setDirty();
(AudioEffectX *)pVOPM->updateDisplay();
}

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

void	CForm::draw(CDrawContext *context)
{
#ifdef WIN32
	COffscreenContext Off(context,getBackground(),false);
#else
	COffscreenContext Off(context,getBackground(),true);
#endif
	CFrame::draw(&Off);
//#if DEBUG
//	DebugPrint("exec CForm::draw");
//#endif

	//SliderRePaint
	int i,j,index;
	//CON ボタンを描く
	for(index=kCon0,i=0;index<=kCon7;index++,i++){
		pConBtn[i]->setValue(pOPMdrv->getPrm(index));
		pConBtn[i]->draw(&Off);
	}

	//PROG SLIDを描く
	for(index=kFL,i=0;index<=kNFQ;index++,i++){
		pSlid2[i]->setValue(pOPMdrv->getPrm(index));
		pSlid2[i]->draw(&Off);
	}

	//LFO SLIDを描く
	for(index=kSPD,i=0;index<=kPMD;index++,i++){
		pSlid3[i]->setValue(pOPMdrv->getPrm(index));
		pSlid3[i]->draw(&Off);
	}

	//WFボタンを描く
	for(index=kWf0,i=0;index<=kWf3;index++,i++){
		pWFBtn[i]->setValue(pOPMdrv->getPrm(index));
		pWFBtn[i]->draw(&Off);
	}


	//NEボタンを描く
	pNEBtn->setValue(pOPMdrv->getPrm(kNE));
	pNEBtn->draw(&Off);

	index=kM1TL;
	for(j=0;j<4;j++){
		for(i=0;i<10;i++){
			pSlid[j][i]->setValue(pOPMdrv->getPrm(index));
			pSlid[j][i]->draw(&Off);
			index++;
		}
		//F_AMEボタンを描く
		pAMEBtn[j]->setValue(pOPMdrv->getPrm(index));
		pAMEBtn[j]->draw(&Off);
		index++;
	}

	for(index=kMskM1,i=0;index<=kMskC2;index++,i++){
		//OPMSKボタンを描く
		pMskBtn[i]->setValue(pOPMdrv->getPrm(index));
		pMskBtn[i]->draw(&Off);
	}

	pFileBtn[0]->draw(&Off);
	pFileBtn[1]->draw(&Off);
	pDisp->EGBPaint(&Off);

	//アルゴリズムの図を描く
	pDisp->ConPaint(&Off,pOPMdrv->getPrm(kCon));

	//プログラムナンバーを書く
	char string[8];
	sprintf(string,"%03d",pOPMdrv->getProgNum()+1);
//#ifdef DEBUG
//::MessageBox(NULL,string,"VOPM",MB_OK);
//#endif

	//drawString (string,NumSize,false,kCenterText);
	Off.copyFrom(context,size,CPoint(0,0));
	pProgDisp->setFontColor (kWhiteCColor);
	pProgDisp->setFont(kNormalFont);
	pProgDisp->setText(string);
	pProgDisp->draw(context);
}

void CForm::valueChanged (CDrawContext* context, CControl* control){
	int i,j,index;

//#if DEBUG
//	DebugPrint("exec CForm::valueChanged");
//#endif
	long tag = control->getTag ();
	
	switch (tag){
		case kFileLoad:
			Load();
			control->setValue(0);
			if(context)control->draw(context);
			break;

		case kFileSave:Save();
			control->setValue(0);
			if(context)control->draw(context);
			break;

		case kWf0: case kWf1: case kWf2: case kWf3:

			for(i=kWf0;i<=kWf3;i++){
				if(i!=tag){
					pWFBtn[i-kWf0]->setValue(0);
				}else{
					pWFBtn[i-kWf0]->setValue(1);
				}

				if(context)pWFBtn[i-kWf0]->draw(context);
			}
			pOPMdrv->setPrm(kWF,(tag-kWf0));
			if(context)update(context);
			break;

		case kCon0: case kCon1: case kCon2: case kCon3:
		case kCon4: case kCon5: case kCon6: case kCon7:
			for(i=kCon0;i<=kCon7;i++){
				if(i!=tag){
					pConBtn[i-kCon0]->setValue(0);
				}else{
					pConBtn[i-kCon0]->setValue(1);
				}
				if(context)pConBtn[i-kCon0]->draw(context);
			}
			pOPMdrv->setPrm(kCon,(tag-kCon0));
			if(pDisp&&context)pDisp->ConPaint(context,pOPMdrv->getPrm(kCon));
			if(context)update(context);
			break;

		case kM1TL: case kM1AR: case kM1D1R: case kM1D2R: case kM1D1L: case kM1RR:
		case kM2TL: case kM2AR: case kM2D1R: case kM2D2R: case kM2D1L: case kM2RR:
		case kC1TL: case kC1AR: case kC1D1R: case kC1D2R: case kC1D1L: case kC1RR:
		case kC2TL: case kC2AR: case kC2D1R: case kC2D2R: case kC2D1L: case kC2RR:
			pOPMdrv->setPrm(tag,(unsigned char)control->getValue());
			if(pDisp&&context)pDisp->EGBPaint(context);
			break;

		case kHdsw:
		//FL等のスライダを描く
			for(index=kFL,i=0;index<=kNFQ;index++,i++){
				pSlid2[i]->setHexText(control->getValue()==1.0?true:false);
				if(context)pSlid2[i]->draw(context);
			}

		//LFO SLIDを描く
			for(index=kSPD,i=0;index<=kPMD;index++,i++){
				pSlid3[i]->setHexText(control->getValue()==1.0?true:false);
				if(context)pSlid3[i]->draw(context);
			}

			//オペレータパラメータのスライダを描く
			for(j=0;j<4;j++){
				for(i=0;i<10;i++){
					pSlid[j][i]->setHexText(control->getValue()==1.0?true:false);
					if(context)pSlid[j][i]->draw(context);
				}
			}
			//表示切替スイッチを描く
			if(context)pHdsw->draw(context);
			break;

			default:
				pOPMdrv->setPrm(tag,(unsigned char)control->getValue());
				if(context)update(context);
				break;
	}

}

long CForm::onKeyDown (VstKeyCode& keyCode)
{
	CView *pCuView;
	long result = -1;
//#if DEBUG
//	DebugPrint("exec CForm::onKeyDown");
//#endif	

	//マウスカーソルがさしているViewに対してのみディスパッチ
	if(pCuView=getCurrentView ()){
		if((result=pCuView->onKeyDown(keyCode))!=-1){
			return(-1);
		}
	}
	return result;
}
void CForm::setFocusView (CView *pView)
{
	int Tag;
#if DEBUG
	if(pFocusView){
		Tag=((CControl *)pFocusView)->getTag();		
		DebugPrint("CForm::setFocusView pFocusView Tag=%d",Tag);	
	}else{
		DebugPrint("CForm::setFocusView pFocusView is NULL");
	}
#endif
	if (pFocusView){
		//前回のFocusViewのindexを取得
		Tag=((CControl *)pFocusView)->getTag();
		if(
			(Tag>=kFL && Tag<=kNFQ)||
			(Tag>=kSPD && Tag<=kPMD)||
			(Tag>=kM1TL && Tag<=kM1DT2)||
			(Tag>=kC1TL && Tag<=kC1DT2)||
			(Tag>=kM2TL && Tag<=kM2DT2)||
			(Tag>=kC2TL && Tag<=kC2DT2)
		){
			((CDispSlider *)pFocusView)->setLearn(false);
		}
	}
	CFrame::setFocusView(pView);

}

