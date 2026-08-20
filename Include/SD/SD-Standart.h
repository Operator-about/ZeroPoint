#pragma once

#include<SD.h>
#include<Custom-type.h>
#include<std.h>
#include<Kernel-modules.h>
#include<exFAT.h>

extern const HALSDF SDAF;
extern SDAR* SDA;

void SDA_IRQ_read();
void SDA_register_init();
void SDA_block_init();
void SDA_CMD_send(CMDR _CMD);
void SDA_wait_command();