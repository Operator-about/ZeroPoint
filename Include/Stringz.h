#pragma once
#include<Custom-type.h>
#include<Kernel-modules.h>
int length_s(char _buffer[]);
int compare_s(uint8_t _compe_buffer[], char _src_buffer[]);
void itos(int _number, char _out_buffer[]);
int stoi(char _resource_buffer[]);
int compare_u16_to_ASCII(uint8_t _compare_buffer[], char _src[]);
void utf16_to_ASCII(uint8_t _utf16_buffer[], uint8_t _ASCII_out_buffer[]);
void ASCII_to_utf16(uint8_t _ASCII_buffer[], uint8_t _utf16_to_buffer[]);
void upper(uint8_t _buffer[]);