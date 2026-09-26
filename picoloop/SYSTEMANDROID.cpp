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
//
// The Settings screen (SettingsActivity.kt) can redirect storage elsewhere
// (e.g. a synced folder for auto-backup) - deliberately an app setting, not
// an ini key: the ini lives inside the storage folder itself, so having the
// ini redirect the very folder it lives in was confusing. The setting is
// stored as a plain text file in the app's private internal storage
// (STORAGE_OVERRIDE_MARKER below, same path Kotlin writes to via
// context.filesDir) so native code can read it with a normal fopen(), no
// JNI/SharedPreferences round trip needed. picoloop.ini itself is then read
// from wherever storage ends up (override or default) - see the bottom of
// computeUserStorage().

static const char * PUBLIC_STORAGE = "/storage/emulated/0/picoloop";
static const char * STORAGE_OVERRIDE_MARKER_NAME = "storage_folder.txt";

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

// Reads the override written by SettingsActivity.kt, if any. Returns an
// empty string if there's none (or the internal storage path is unknown).
static void readStorageOverride(char * out, size_t outSize)
{
  out[0] = '\0';
  const char * internalBase = SDL_AndroidGetInternalStoragePath();
  if (!internalBase)
    return;

  char markerPath[MAXHOMEPATH];
  snprintf(markerPath, MAXHOMEPATH, "%s/%s", internalBase, STORAGE_OVERRIDE_MARKER_NAME);
  FILE * f = fopen(markerPath, "r");
  if (!f)
    return;
  if (fgets(out, (int)outSize, f) != NULL)
    {
      size_t len = strlen(out);
      while (len > 0 && (out[len-1] == '\n' || out[len-1] == '\r'))
        out[--len] = '\0';
    }
  fclose(f);
}

static const char * computeUserStorage()
{
  static char storage[MAXHOMEPATH];
  static int computed = 0;
  if (computed)
    return storage;
  computed = 1;

  char overridePath[MAXHOMEPATH];
  readStorageOverride(overridePath, MAXHOMEPATH);
  if (overridePath[0] != '\0')
    {
      mkdir(overridePath, 0755);
      if (isWritableDir(overridePath))
        snprintf(storage, MAXHOMEPATH, "%s", overridePath);
    }

  if (storage[0] == '\0')
    {
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
    }

  char iniPath[MAXHOMEPATH];
  snprintf(iniPath, MAXHOMEPATH, "%s/picoloop.ini", storage);
  loadPicoloopIni(iniPath);

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
