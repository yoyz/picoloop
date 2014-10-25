#ifndef __SYSTEM__
#define __SYSTEM__

#include "SYSTEM.h"


#include <sys/stat.h>
#include <sys/types.h>

int MKDIR(const char *pathname)
{
  mode_t mode;
  mode=0755; 
  mkdir(pathname,mode);
}

#endif
