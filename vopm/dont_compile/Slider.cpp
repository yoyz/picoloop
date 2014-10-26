#include "Slider.hpp"
#include <stdio.h>

#define KNOB_W 12
#define KNOB_H 7
#define SEG_W 12
#define SEG_H 19

#define widthDisp 20
#define heightDisp 20
#if DEBUG
extern void DebugPrint (char *format, ...);
#endif
//------------------------------------------------------------------------
// CDispSlider
//------------------------------------------------------------------------
CDispSlider::CDispSlider (const CRect &size, CControlListener *listener, long tag,
	float Max,
	float Min,
	float Pos,
	CBitmap *handle,  // bitmap of slider
	CBitmap *background)//bitmap of Background
:CControl (size, listener, tag, background), pHandle (handle),pOScreen (0)
{
	int dif;

	setDrawTransparentHandle (true);

	if (pHandle)
	{
		pHandle->remember ();
		widthOfSlider  = pHandle->getWidth ();
		heightOfSlider = pHandle->getHeight ();
	}
	else
	{
		widthOfSlider  = 1; 
		heightOfSlider = 1;
	}

	widthControl  = size.width ();
	heightControl = size.height ();

	maxPos = heightDisp;
	minPos = size.height()-heightOfSlider;

	rangeHandle = minPos - maxPos;
	setMax(Max);
	setMin(Min);
	setValue(Pos);
	setDefaultValue(Pos);
	dif=(int)(Max-Min);
	if(dif==0){dif=1;}
	rangeHandle=(float)(minPos-maxPos)/(float)dif;
	setWheelInc(1);
	bMouseEnabled=true;
	bHexText=false;
	bLearn=false;
	setWantsFocus (true);
}


//------------------------------------------------------------------------
CDispSlider::~CDispSlider ()
{
	if (pHandle)
		pHandle->forget ();
}


//-----------------------------------------------------------------------------
bool CDispSlider::attached (CView *parent)
{
	if (pOScreen)
		delete pOScreen;
	pOScreen = new COffscreenContext (getFrame (), widthControl, heightControl, kBlackCColor);
	return CControl::attached (parent);
}

//-----------------------------------------------------------------------------
bool CDispSlider::removed (CView *parent)
{
	if (pOScreen)
	{
		delete pOScreen;
		pOScreen = 0;
	}
	return CControl::removed (parent);
}

//------------------------------------------------------------------------
void CDispSlider::draw (CDrawContext *pContext)
{
	float fValue;
	long dif;
/*
	CRect sizeDisp(
		size.left+(widthControl/2)-widthDisp/2,
		size.top,
		size.left+(widthControl/2)+widthDisp/2,
		size.top+heightDisp);
*/
	CRect sizeDisp(
		(widthControl/2)-widthDisp/2,
		0,
		(widthControl/2)+widthDisp/2,
		heightDisp);
		fValue = value;
	// (re)draw background
	CRect rect (0, 0, widthControl, heightControl);
	if (pBackground)
	{
		if (bTransparencyEnabled)
			pBackground->drawTransparent (pOScreen, rect,CPoint(0,0));
		else
			pBackground->draw (pOScreen, rect,CPoint(0,0));
	}
	
	// calc new coords of slider
	CRect   rectNew;

		rectNew.left   = widthControl/2-widthOfSlider/2;
		rectNew.right  = rectNew.left + widthOfSlider;	

		rectNew.top	= (minPos -fValue*rangeHandle);
		rectNew.top	= (rectNew.top > minPos) ? minPos : rectNew.top;
		rectNew.top	= (rectNew.top < maxPos) ? maxPos : rectNew.top;

		rectNew.bottom = rectNew.top + heightOfSlider;

	// draw slider at new position
	if (pHandle)
	{
		if (bDrawTransparentEnabled)
			pHandle->drawTransparent (pOScreen, rectNew);
		else 
			pHandle->draw (pOScreen, rectNew);
	}

	int i,lineNum,step,y;
	pOScreen->setFrameColor(kWhiteCColor);
	if(vmax!=vmin){
		step=(int)((float)(minPos-maxPos)/(float)(vmax-vmin));
		if(step<4){step=4;}
	}else{step=4;}
	y=minPos+heightOfSlider/2;
	while(y>=(maxPos+heightOfSlider/2-1)){
		pOScreen->moveTo(CPoint(0,y));
		pOScreen->lineTo(CPoint(6,y));
		y-=step;
	}

	char string[32];


	if(bHexText){
		sprintf(string,"%02X",(int)value);
	}else{
		sprintf(string,"%03d",(int)value);
	}

	if(bLearn){
		pOScreen->setFontColor (kRedCColor);
	}else{
		pOScreen->setFontColor (kWhiteCColor);
	}
	pOScreen->setFont(kNormalFont);
	pOScreen->drawString (string,sizeDisp,false,kCenterText);

	pOScreen->copyFrom (pContext, size);
	setDirty (false);
}

//------------------------------------------------------------------------
void CDispSlider::mouse (CDrawContext *pContext, CPoint &where,long button)
{
	int cnt=0;
	float oldVal	= value;
	int ivalue;
#if DEBUG
	DebugPrint("exec CDispSlider::mouse");
#endif
	if (!bMouseEnabled)
		return;

	if(button==-1) button = pContext->getMouseButtons ();

	// set the default value
	if (button == (kAlt|kLButton))
	{
		/*
		value = getDefaultValue ();
		if(value!=oldVal){
			setDirty(true);
		}
		if (isDirty () && listener)
			listener->valueChanged (pContext, this);
		*/
		setLearn(true);
		getFrame()->setFocusView(this);
		draw(pContext);
#if DEBUG
	DebugPrint("CDispSlider::SetFocusView button=%08x Tag=%x",button,getTag());
#endif

		return;
	}
	setLearn(false);
	// allow left mousebutton only
	if (!(button & kLButton))
		return;

	// begin of edit parameter

	getFrame ()->beginEdit (tag);
	while (1)
	{
		oldVal=value;
		button = pContext->getMouseButtons ();
		if (!(button & kLButton))
			break;

		if(!(button & kShift)){
			value =(float)(minPos-(where.v-size.top)) / (float)rangeHandle+vmin;
		}else{
			long pos;
			pos=(int)(minPos-value*(float)rangeHandle);
			if(cnt==0){
				if(((where.v-size.top)-pos)<0){
					value++;
				}else{
					value--;
				}
			}
			cnt=1;
		}
		ivalue=(int)value;
		value=ivalue;
		bounceValue();
		draw(pContext);
		
		if(value!=oldVal){
			setDirty(true);
		}
		if (isDirty () && listener)
			listener->valueChanged (pContext, this);

		pContext->getMouseLocation (where);

		doIdleStuff ();
	}

	// end of edit parameter
	getFrame ()->endEdit (tag);	
	draw(pContext);
	#if DEBUG
		DebugPrint("CDispSlider::unsetFocusView button=%08x Tag=%x",button,getTag());
	#endif
}

//------------------------------------------------------------------------
bool CDispSlider::onWheel (CDrawContext *pContext, const CPoint &where, float distance)
{
	#if DEBUG
		DebugPrint("enter CDispSlider::onWheel");
	#endif
	if (!bMouseEnabled)
		return false;

	value += distance;
	setDirty(true);
	bounceValue();
	#if DEBUG
		DebugPrint("exec CDispSlider::onWheel value=%f distance=%f",value,distance);
	#endif
	if (isDirty () && listener){
		#if DEBUG
			DebugPrint("exec CDispSlider::onWheel ValuChanged");
		#endif 
		listener->valueChanged (pContext, this);
	}
	getFrame()->setDirty(true);
	return true;
}

//------------------------------------------------------------------------
long CDispSlider::onKeyDown (VstKeyCode& keyCode)
{
	int oldval;
	int cuval;
	char In;

		In=keyCode.character;
/*
	#if DEBUG
	DebugPrint("exec CDispSlider::onKeyDown char=%04x virt=%02x mod=%02x",
		keyCode.character,
		keyCode.virt,
		keyCode.modifier
		);
	#endif
*/
	if(keyCode.virt==VKEY_ALT){
		getFrame()->setFocusView(this);
		setLearn(true);
		getFrame()->setDirty(true);
#if DEBUG
	DebugPrint("CDispSlider::onKeyDown SetFocusView Tag=%x",getTag());
#endif
		return(1);
	}

	if(In=='+' || keyCode.virt==VKEY_ADD){value+=1.0;bounceValue();}
	if(In=='-' || keyCode.virt==VKEY_SUBTRACT){value-=1.0;bounceValue();}
	if(bHexText){
		cuval=(int)value;
		if(In>='0'&&In<='9'){
			cuval<<=4;
			cuval+=(In-'0');
		}
		if (In>='a' && In<='f'){
			cuval<<=4;
			cuval+=(In-'a'+0xa);
		}
		if (In>='A' && In<='F'){
			cuval<<=4;
			cuval+=(In-'A'+0xa);
		}
		if (keyCode.virt==VKEY_DELETE|| keyCode.virt==VKEY_BACK){
			cuval>>=4;
		}
		if((float)cuval>vmax){
			 cuval&=0x0f;
		}
		value=cuval;
		bounceValue();
	}else{
		cuval=(int)value;
		if(In>='0'&&In<='9'){
			cuval*=10;
			cuval+=(In-'0');
		}
		if (keyCode.virt==VKEY_DELETE || keyCode.virt==VKEY_BACK){
			cuval/=10;
		}
		if(cuval>vmax){
			cuval%=100;
			if(cuval>vmax){
				cuval%10;
				if(cuval>vmax){
					cuval=(In-'0');
				}
			}
		}
		value=cuval;
		bounceValue();
	}
			if (isDirty () && listener)
			{
				// begin of edit parameter
				beginEdit ();
				listener->valueChanged (0, this);
				// end of edit parameter
				endEdit ();
			}
			getFrame()->setFocusView(NULL);
			setLearn(false);
			getFrame()->setDirty(true);
	return 1;
}

//------------------------------------------------------------------------
void CDispSlider::setHandle (CBitmap *_pHandle)
{
	if (pHandle)
		pHandle->forget ();
	pHandle = _pHandle;
	if (pHandle)
	{
		pHandle->remember ();
		widthOfSlider  = pHandle->getWidth ();
		heightOfSlider = pHandle->getHeight ();
	}
}

void CDispSlider::setHexText(bool stat){
	bHexText=stat;
}

void CDispSlider::setLearn(bool stat){
	bLearn=stat;
}


