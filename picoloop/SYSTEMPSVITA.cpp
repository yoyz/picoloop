#ifndef __SYSTEM__
#define __SYSTEM__

#include "SYSTEM.h"


//#include <sys/stat.h>
//#include <sys/types.h>
#include <psp2/io/stat.h>

int MKDIR(const char *pathname)
{
  SceMode mode;
  //mode_t mode;
  mode=0755; 
  //mkdir(pathname,mode);
 sceIoMkdir(pathname,mode);
}

#endif
