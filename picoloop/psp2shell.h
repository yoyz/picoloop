/*
	PSP2SHELL
	Copyright (C) 2016, Cpasjuste

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _psp2_shell_h_
#define _psp2_shell_h_

#ifdef __cplusplus
extern "C" {
#endif

#include "p2s_cmd.h"
#include "p2s_msg.h"

#ifdef DEBUG

int sceClibPrintf(const char *, ...);

#define printf sceClibPrintf
#endif

// init psp2shell on specified port
int psp2shell_init(int port);

void psp2shell_exit();

void psp2shell_print_color(int color, const char *fmt, ...);

#define psp2shell_print(...) psp2shell_print_color(COL_NONE, __VA_ARGS__)
#define PRINT(...) psp2shell_print_color(COL_NONE, __VA_ARGS__)
#define PRINT_ERR(...) psp2shell_print_color(COL_RED, __VA_ARGS__)
#define PRINT_OK(...) psp2shell_print_color(COL_GREEN, __VA_ARGS__)

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // _psp2_shell_h_
