#ifndef __SYSTEM__
#define __SYSTEM__

#include "SYSTEM.h"
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAXHOMEPATH 256


char * GETPICOLOOPSYSTEMSTORAGE()
{
  //static char * systemstorage="/usr/share/picoloop/";
  static char * systemstorage="./";
  return systemstorage;
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
  snprintf(homedir, MAXHOMEPATH, "%s", getenv("HOME"));
  return homedir;
}

int MKDIR(const char *pathname)
{
  mode_t mode;
  mode=0755; 
  mkdir(pathname,mode);
  return 0;
}

#endif
