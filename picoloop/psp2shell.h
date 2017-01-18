#ifndef _psp2_shell_h_
#define _psp2_shell_h_

#ifdef __cplusplus
extern "C" {
#endif

enum colors_t {
    COL_NONE = 0,
    COL_RED = 1,
    COL_YELLOW = 2,
    COL_GREEN = 3
};

// init psp2shell on specified port with delay in seconds.
// setting a delay will pause (delay) the application
// allowing for early debug message to be shown
// and eventually give time to "reload" your app over network.
int psp2shell_init(int port, int delay);

void psp2shell_exit();

void psp2shell_print(const char *fmt, ...);

void psp2shell_print_color(int color, const char *fmt, ...);

// uncomment this to redefine printf
//#define printf(...) psp2shell_print_color(0, __VA_ARGS__)

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // _psp2_shell_h_
