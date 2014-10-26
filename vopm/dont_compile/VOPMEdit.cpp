/*-----------------------------------------------------------------------------
VOPMEdit.cpp
2002.3.17 copyright sam
VOPM tone GUI Editor for MS-Windows  build by Dev C++,no use vstgui.lib
-----------------------------------------------------------------------------*/

#ifndef __AEffGUIEditor
#include "aeffguieditor.h"
#endif
#include "audioeffect.h"

#include <stdio.h>
#include "VOPMEdit.hpp"
#include "VOPM.hpp"
#include "OPMdrv.hpp"
#include "form.h"
#include "vstgui.h"
#include "vstcontrols.h"

//#if WIN32
#include "ResVOPM.h"
//#endif
#if DEBUG
extern void DebugPrint (char *format, ...);
#endif
#define VERSION "Version 0.16(2008.01.05)"
//-----------------------------------------------------------------------------
// resource id's


//-----------------------------------------------------------------------------
// prototype string convert float -> percent
void percentStringConvert (float value, char* string);
void percentStringConvert (float value, char* string)
{
	 sprintf (string, "%d%%", (int)(100 * value));
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// CAbout Declaration
//-----------------------------------------------------------------------------
class CAbout : public CSplashScreen{
public:
	CAbout (CRect &size, CControlListener *listener, int tag, CBitmap *handle,
		CRect &toDisplay, CPoint &offset, CPoint &textOffset,OPMDRV *p);

	void draw (CDrawContext *pContext);
	void mouse (CDrawContext *context, CPoint&where);
private:
	CPoint textOffset;
	OPMDRV *pOPMdrv;
};
//-----------------------------------------------------------------------------
// CAbout implementation
//-----------------------------------------------------------------------------
CAbout::CAbout (CRect &size, CControlListener *listener, int tag, CBitmap *handle,
				CRect &toDisplay, CPoint &offset, CPoint &textOffset,OPMDRV *p)
				: CSplashScreen (size, listener, tag, handle, toDisplay, offset),
				textOffset (textOffset)
{
	pOPMdrv=p;
}
//-----------------------------------------------------------------------------
void CAbout::draw (CDrawContext *pContext){

	if (getValue ())	{
		CSplashScreen::draw (pContext);
		CRect rect (0, 0, getWidth (), 15);
		rect.offset (toDisplay.left + textOffset.h, toDisplay.top + textOffset.v);

		char text[128];
		sprintf (text, "%s",VERSION);
		pContext->setFont (kNormalFontSmall);
		pContext->setFontColor (kBlackCColor);
		pContext->drawString (text, rect, 0, kLeftText);

	}



 }
void CAbout::mouse (CDrawContext *context, CPoint&where){

	int i;
	pOPMdrv->ForceAllOff();
	CSplashScreen::mouse (context, where);
}

//-----------------------------------------------------------------------------
// VOPMEdit class implementation
//-----------------------------------------------------------------------------
//VOPMEdit::VOPMEdit (AudioEffect *effect)
VOPMEdit::VOPMEdit (VOPM *effect)
 : AEffGUIEditor (effect)
{
//	DebugStr((const unsigned char *)" VOPM Edit inited");
	effect->setEditor(this);
	pVoTbl=effect->pOPMdrv->VoTbl;
	pOPMdrv=effect->pOPMdrv;
	Effect=effect;
	// load the background bitmap
	// we don't need to load all bitmaps, this could be done when open is called
	hBackground = new CBitmap (IDI_BG);
//	if(hBackground == NULL){
//		::MessageBox(NULL,"faile load IDI_BG","edit",MB_OK);
//	}
	// init the size of the plugin
	rect.left	 = 0;
	rect.top	= 0;
	rect.right	= (short)hBackground->getWidth ();
	rect.bottom = (short)hBackground->getHeight ();

}

//-----------------------------------------------------------------------------
VOPMEdit::~VOPMEdit ()
{
	// free the background bitmap
	if (hBackground)
		hBackground->forget();
	hBackground=0;
}

//-----------------------------------------------------------------------------

bool VOPMEdit::open (void *ptr)

{
	AEffGUIEditor::open (ptr);
	systemWindow = ptr;
//	if(ptr==NULL){::MessageBox(NULL,"open","fail ptr==NULL",MB_OK);}
	//--init background frame-----------------------------------------------
	CRect size (0, 0, hBackground->getWidth (), hBackground->getHeight ());
	frame = new CForm(size,ptr,this,pOPMdrv,Effect);
	frame->setBackground (hBackground);
	//	CRect sizeS(ScX,ScY,ScX+ScW,ScY+ScH);
	//	pDisp = new class DISP(size,pChip);
	//	frame->addView (pDisp);

	CBitmap *pAboutBitmap = new CBitmap (IDI_ABOUT);
	CPoint point (0, 0);
	CRect toDisplay (100, 50, pAboutBitmap->getWidth ()+100, pAboutBitmap->getHeight ()+50);
	size (0, 0, 170, 45);
	size.offset (510, 25);
	CPoint textOffset (40,60);
	CAbout *pAbout = new CAbout (size,(CControlListener *)0,99, pAboutBitmap, toDisplay, point, textOffset,pOPMdrv);
	frame->addView (pAbout);
	pAboutBitmap->forget ();
	if(frame){
		frame->draw((CView*)0);
		frame->setDirty();
	}
	return true;
}

//-----------------------------------------------------------------------------
void VOPMEdit::close ()
{
	delete frame;
	frame=0;

}
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

void VOPMEdit::update()
{
//#if DEBUG
//    DebugPrint("exec VOPMEdit::update");
//#endif
	if(frame){
//		frame->draw((CView*)0);
		frame->setDirty();
	}
}

long VOPMEdit::getTag(){
	long Tag; 
	if(frame){
		CControl *pCtr;
		pCtr=(CControl *)frame->getFocusView();
 #if DEBUG
 DebugPrint("VOPMEdit::update View=%08x",pCtr);
 #endif
		if(pCtr) {
			Tag=pCtr->getTag();
		if(
			(Tag>=kFL && Tag<=kNFQ)||
			(Tag>=kSPD && Tag<=kPMD)||
			(Tag>=kM1TL && Tag<=kM1DT2)||
			(Tag>=kC1TL && Tag<=kC1DT2)||
			(Tag>=kM2TL && Tag<=kM2DT2)||
			(Tag>=kC2TL && Tag<=kC2DT2)
		){
			((CDispSlider *)pCtr)->setLearn(false);
		}					
					frame->setFocusView(NULL);
					return(Tag);
				}
	}
	return(-1);
}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

	bool VOPMEdit::getRect (ERect **erect)
{
	static struct ERect r={0,0,493,692};
	*erect =&r;
	return(true);
}

