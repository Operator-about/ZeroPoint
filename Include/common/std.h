#pragma once

#include<PL011.h>
#include<Kernel-modules.h>

void print(char _buffer[]);
void input(char _save_buffer[]);
void println(char _buffer[]);

void printh64(uint64_t _src);
void printh(uint8_t _src);