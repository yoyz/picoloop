#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "T_Singleton.h"
#include "VariableContainer.h"
#include "Trace.h"
#include "tinyxml.h"

class Config: public T_Singleton<Config>,public VariableContainer {
public:
	Config() ;
	~Config() ;
	const char *GetValue(const char *key) ;
	void ProcessArguments(int argc,char **argv) ;
} ;

#endif
