#pragma once
#define DEBUG 1
#define VERISON 002
#include<Custom-type.h>
#include<stdalign.h>
#include<Stringz.h>

extern UART0 UART;
extern GICCv2* GICv2;
extern Periphery All_periphery;
extern Ring_buffer Tx_buffer;
extern Ring_buffer Rx_buffer;

void VBAR_set();
void clear_buffer(char _buffer[]);
int GIC_version_check();
int MMU_IPS_check();
int get_number_length(int _number);
void send();
void receving();
void init_t_buffer();
void get_re_remainder();