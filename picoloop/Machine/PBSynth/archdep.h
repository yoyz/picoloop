#ifndef __ARCHDEP_H__
#define __ARCHDEP_H__

void initAudio(void (*audio_callback)(void *, unsigned char *, int));
void stopAudio(void);
void initVideo(void);
void printScreen(void);
void refreshScreen(void);
void setPal(int i, unsigned char *c);
void commitPal(int brightness);
unsigned char *getPixels(void);
void moveCursor(int x, int y);
void showCursor(void);
void hideCursor(void);
int pollmachine(void);
void exitApp(void);
void waitKey(void);

void releaseMouseButtonEvent(void);
void moveMouseEvent(int x, int y);
void pressMouseButtonEvent(void);
void moveMouseEvent(int x, int y);

extern int arpMode, arpSpeed, arpOctaveMax, oscBase[2], note[2];

#define SYNTH_OK 			0
#define BUFLEN 512


#ifdef GP32
#define GUI_XINC 240
#define GUI_YINC -1
#define GUI_OFS(x,y) ((239 - (y)) + (240 * (x)))
#else
#define GUI_XINC 1
#define GUI_YINC 320
#define GUI_OFS(x,y) ((320 * (y)) + (x))
#endif


#endif
