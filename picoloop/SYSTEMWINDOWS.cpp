#ifndef __SYSTEM__
#define __SYSTEM__

#include "SYSTEM.h"

#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#define MAXHOMEPATH 256

int MKDIR(const char *pathname)
{
  mkdir(pathname);
}
char * GETPICOLOOPUSERSTORAGE()
{
  static char picoloophome[MAXHOMEPATH];
  //snprintf(picoloophome,"%s/.picoloop/",GETHOME());
  sprintf(picoloophome,"./");
  return picoloophome;
}

char * GETHOME()
{
  static char homedir[MAXHOMEPATH];
  //sprintf(homedir, MAXHOMEPATH, "%s", getenv("HOME"));
  sprintf(homedir, MAXHOMEPATH, "./");
  return homedir;
}


#endif
