#pragma once

#include<Kstd.h>
#include<exFAT.h>
#include<Custom-type.h>
#include<Stringz.h>

extern char CurrentNameDir[256];
extern char KeyboardBufferInput[256];

void shell_init();
void ZeroPoint_logo();
void open_mode();
void help_command();