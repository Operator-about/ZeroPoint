#pragma once

#include<SD.h>
#include<Custom-type.h>
#include<Kernel-modules.h>
#include<exFAT.h>

extern const HALSDF SDAF;
extern SDAR* SDA;

void SDA_IRQ_read();
void SDA_register_init();
void SDA_block_init(int _block_size, int _block_count);
void SDA_CMD_send(CMDR _CMD);
void SDA_wait_command();