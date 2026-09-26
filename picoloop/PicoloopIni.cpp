// PC_DESKTOP and __ANDROID__: optional picoloop.ini overriding key-repeat
// speed, font, and default theme. See picoloop.ini.example. The storage
// folder itself is NOT in here (Android only) - see SYSTEMANDROID.cpp,
// it's an app setting (Settings screen), not an ini key, since the ini
// lives inside that same folder and bootstrapping it from itself would be
// confusing.

#include "Master.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

int KEY_REPEAT_INTERVAL_SMALLEST = 8;
int KEY_REPEAT_INTERVAL_SMALL    = 16;
int KEY_REPEAT_INTERVAL_MIDDLE   = 64;
int KEY_REPEAT_INTERVAL_LONG     = 128;
int KEY_REPEAT_INTERVAL_LONGEST  = 256;

char g_ini_font_path[256] = ""; // empty = embedded font
int  g_ini_default_theme  = -1; // -1 = engine default

static char *trim(char *s)
{
  while (*s==' ' || *s=='\t') s++;
  char *end = s + strlen(s);
  while (end > s && (end[-1]=='\n' || end[-1]=='\r' || end[-1]==' ' || end[-1]=='\t'))
    *--end = '\0';
  return s;
}

void loadPicoloopIni(const char *path)
{
  FILE *f = fopen(path, "r");
  if (f == NULL)
    return;

  char line[256];
  while (fgets(line, sizeof(line), f) != NULL)
    {
      char *s = trim(line);
      if (s[0]=='\0' || s[0]=='#' || s[0]==';' || s[0]=='[')
        continue;

      char *eq = strchr(s, '=');
      if (eq == NULL)
        continue;
      *eq = '\0';
      char *key = trim(s);
      char *val = trim(eq+1);
      if (val[0]=='\0')
        continue;

      if (strcmp(key,"smallest")==0 || strcmp(key,"8")==0)
        { int v=atoi(val); if (v>0) KEY_REPEAT_INTERVAL_SMALLEST=v; }
      else if (strcmp(key,"small")==0 || strcmp(key,"16")==0)
        { int v=atoi(val); if (v>0) KEY_REPEAT_INTERVAL_SMALL=v; }
      else if (strcmp(key,"middle")==0 || strcmp(key,"64")==0)
        { int v=atoi(val); if (v>0) KEY_REPEAT_INTERVAL_MIDDLE=v; }
      else if (strcmp(key,"long")==0 || strcmp(key,"128")==0)
        { int v=atoi(val); if (v>0) KEY_REPEAT_INTERVAL_LONG=v; }
      else if (strcmp(key,"longest")==0 || strcmp(key,"256")==0)
        { int v=atoi(val); if (v>0) KEY_REPEAT_INTERVAL_LONGEST=v; }
      else if (strcmp(key,"font")==0)
        {
          strncpy(g_ini_font_path, val, sizeof(g_ini_font_path)-1);
          g_ini_font_path[sizeof(g_ini_font_path)-1] = '\0';
        }
      else if (strcmp(key,"theme")==0)
        { int v=atoi(val); if (v>=0 && v<=5) g_ini_default_theme=v; }
    }

  fclose(f);
}
