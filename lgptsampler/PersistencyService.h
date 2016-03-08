#ifndef _PERSISTENCY_SERVICE_H_
#define _PERSISTENCY_SERVICE_H_

#include "Service.h"
#include "T_Singleton.h"
 #include "tinyxml.h"

class PersistencyService: public Service,public T_Singleton<PersistencyService> {
public:
	PersistencyService() ;
	void Save() ;
	bool Load() ;
} ;

class PersistencyDocument: public TiXmlDocument {
public:
	PersistencyDocument(const char *filename):TiXmlDocument(filename) { version_=0 ;} ;
	PersistencyDocument(const std::string& filename):TiXmlDocument(filename) { version_=0 ;} ;
	int version_ ;
} ;
#endif
