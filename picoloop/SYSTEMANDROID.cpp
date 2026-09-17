#ifndef __SYSTEM__
#define __SYSTEM__

#include "SYSTEM.h"
#include "Master.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

#define MAXHOMEPATH 512

// Prefer the public, user-browsable /storage/emulated/0/picoloop folder
// (created and permission-requested by MainActivity.kt, same idea as
// LittlePiggyTracker's own public folder) so bank/ saves are reachable from
// a file manager / another app, not locked in the private app sandbox.
// If that folder isn't there or isn't writable (permission denied, or an
// odd device layout), fall back to the app's private internal storage
// (SDL_AndroidGetInternalStoragePath()) so picoloop still works either way.

static const char * PUBLIC_STORAGE = "/storage/emulated/0/picoloop";

static int isWritableDir(const char * path)
{
  char probe[MAXHOMEPATH];
  snprintf(probe, MAXHOMEPATH, "%s/.picoloop_write_test", path);
  FILE * f = fopen(probe, "w");
  if (!f)
    return 0;
  fclose(f);
  remove(probe);
  return 1;
}

static const char * computeUserStorage()
{
  static char storage[MAXHOMEPATH];
  static int computed = 0;
  if (computed)
    return storage;
  computed = 1;

  struct stat st;
  int havePublicDir = (stat(PUBLIC_STORAGE, &st) == 0 && S_ISDIR(st.st_mode));
  if (!havePublicDir)
    havePublicDir = (mkdir(PUBLIC_STORAGE, 0755) == 0);

  if (havePublicDir && isWritableDir(PUBLIC_STORAGE))
    {
      snprintf(storage, MAXHOMEPATH, "%s", PUBLIC_STORAGE);
    }
  else
    {
      const char * base = SDL_AndroidGetInternalStoragePath();
      snprintf(storage, MAXHOMEPATH, "%s", base ? base : ".");
    }
  return storage;
}

char * GETHOME()
{
  static char homedir[MAXHOMEPATH];
  snprintf(homedir, MAXHOMEPATH, "%s", computeUserStorage());
  return homedir;
}

char * GETPICOLOOPUSERSTORAGE()
{
  static char homedir[MAXHOMEPATH];
  snprintf(homedir, MAXHOMEPATH, "%s", computeUserStorage());
  return homedir;
}

char * GETPICOLOOPSYSTEMSTORAGE()
{
  static char homedir[MAXHOMEPATH];
  snprintf(homedir, MAXHOMEPATH, "%s", computeUserStorage());
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
