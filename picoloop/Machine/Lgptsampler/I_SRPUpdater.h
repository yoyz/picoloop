#ifndef _I_SRP_UPDATER_H_
#define _I_SRP_UPDATER_H_

#include "lgptfixed.h"

struct RUParams {
	lgptfixed volumeOffset_ ;
	lgptfixed speedOffset_ ;
	lgptfixed cutOffset_ ;
	lgptfixed resOffset_ ;
	lgptfixed panOffset_ ;
	lgptfixed fbMixOffset_ ;
	lgptfixed fbTunOffset_ ;
} ;

class I_SRPUpdater {
public:
	I_SRPUpdater() {} ;
	virtual ~I_SRPUpdater() {} ;
	virtual void Trigger(bool tableTick)=0 ;
	virtual void UpdateSRP(struct RUParams &rup)=0 ;
	void Enable() { enabled_=true ;} ;
	void Disable() { enabled_=false ;} ;
	bool Enabled() { return enabled_ ; } ;
protected:
	bool enabled_ ;
} ;
#endif
