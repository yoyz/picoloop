#ifndef __SYSTEM__
#define __SYSTEM__

#include "SYSTEM.h"
#include "Master.h"
#define MAXHOMEPATH 256

//#include <sys/stat.h>
//#include <sys/types.h>
#include <psp2/io/stat.h>
#include <stdio.h>

char * GETHOME()
{
  static char homedir[MAXHOMEPATH];
  sprintf(homedir,"ux0:/app/PICOLOOP1");
  return homedir;
}

char * GETPICOLOOPUSERSTORAGE()
{
  static char homedir[MAXHOMEPATH];
  sprintf(homedir,"ux0:/data/PICOLOOP1");
  return homedir;
}
char * GETPICOLOOPSYSTEMSTORAGE()
{
  static char homedir[MAXHOMEPATH];
  sprintf(homedir,"ux0:/app/PICOLOOP1");
  return homedir;
}

int MKDIR(const char *pathname)
{
  SceMode mode;
  //mode_t mode;
  mode=0777; 
  //mkdir(pathname,mode);
  DPRINTF("Creating directory %s",pathname);
 sceIoMkdir(pathname,mode);
}

#endif
