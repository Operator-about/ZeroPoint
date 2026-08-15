#pragma once

#include<SD.h>
#include<Custom-type.h>
#include<std.h>
#include<Kernel-modules.h>
#include<exFAT.h>

extern const HALSDF SDAF;
extern SDAR* SDA;

void IRQ_read();
void register_init();
void block_init();
void CMD_send(CMDR _CMD);
void wait_command();