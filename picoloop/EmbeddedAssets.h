// PC_DESKTOP only: font.ttf and picoloop-logo.bmp embedded as byte arrays
// so the compiled binary doesn't need those files next to it to run.
// Generated from font.ttf and picoloop-logo.bmp - regenerate (see the
// python snippet in the commit that added this file) if either changes.
#ifndef __EMBEDDED_ASSETS_H__
#define __EMBEDDED_ASSETS_H__

extern const unsigned char g_embedded_font_ttf[];
extern const unsigned int  g_embedded_font_ttf_len;

extern const unsigned char g_embedded_logo_bmp[];
extern const unsigned int  g_embedded_logo_bmp_len;

#endif
