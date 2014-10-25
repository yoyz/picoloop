#ifndef __SYSTEM__
#define __SYSTEM__

#include "SYSTEM.h"

#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>

int MKDIR(const char *pathname)
{
  mkdir(pathname);
}

#endif
