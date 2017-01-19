//---------------------------------------------------------------------------

#ifndef FORMH
#define FORMH
//---------------------------------------------------------------------------

#include "OPMdrv.hpp"
#include "disp.h"
#include "vstcontrols.h"
#include "Slider.hpp"

//---------------------------------------------------------------------------
class CForm : public CFrame,public CControlListener
{
public:
	CForm(CRect &size, void *systemWindow, AEffGUIEditor *editor,class OPMDRV *,VOPM *);
	~CForm(void);
	virtual void valueChanged (CDrawContext* context, CControl* control);
	virtual void draw(CDrawContext *context);
	virtual long onKeyDown (VstKeyCode& keyCode);
	virtual void setFocusView(CView *pView);
private:	// ÉÜÅ[ÉUÅ[êÈåæ

	CBitmap *pKnobImage;
	CBitmap *pBtnImage;
	CBitmap *pSliderBgImage;
	CBitmap *pHdswImage;
	CDisp *pDisp;
	AEffGUIEditor *pEditor;

	void Load(void);
	void Save(void);
	class CDispSlider *pSlid[4][10];//for OP
	class CDispSlider *pSlid2[4];//for PROG
	class CDispSlider *pSlid3[3];//for LFO
	class COnOffButton *pMskBtn[4];
	class COnOffButton *pAMEBtn[4];
	class COnOffButton *pFileBtn[2];
	class COnOffButton *pConBtn[8];
	class COnOffButton *pWFBtn[4];
	class COnOffButton *pNEBtn;
	class COnOffButton *pHdsw;
	class CTextLabel *pProgDisp;

	class OPMDRV *pOPMdrv;
	class VOPM *pVOPM;

};

#endif
