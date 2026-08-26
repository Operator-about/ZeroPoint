#pragma once

#include<Custom-type.h>
#include<SD.h>

extern const HALSDF SDRF;
extern SDRR* SDR;

void SDR_IRQ_read();
void SDR_register_init();
void SDR_block_init(int _block_size, int _block_count);
void SDR_CMD_send(CMDR _CMD);
void SDR_wait_command();