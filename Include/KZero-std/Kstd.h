#pragma once

#include<UART.h>
#include<Kernel-modules.h>

void print(char _buffer[]);
void input(char _save_buffer[]);

void printh64(uint64_t _src);
void printh(uint8_t _src);