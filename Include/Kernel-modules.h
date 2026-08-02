#pragma once
#define DEBUG 1
#define VERISON 003
#include<Custom-type.h>
#include<stdalign.h>
#include<Stringz.h>
#include<std.h>

extern UARTPL011R* UART;
extern GICCv2* GICv2;
extern Ring_buffer Tx_buffer;
extern Ring_buffer Rx_buffer;
extern JumpData* OutJump;


void VBAR_set();
void clear_buffer(uint8_t _buffer[]);
int GIC_version_check();
int MMU_IPS_check();
int MMU_TG_check();
int get_number_length(int _number);
void init_t_buffer();
void sec_barrier(int _second);
uint64_t min_uint64_t(uint64_t _buffer[]);