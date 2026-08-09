#pragma once

#include<Kernel-modules.h>
#include<std.h>
#include<Custom-type.h>
#include<Stringz.h>
#include<exFAT.h>

extern uint32_t SD;
extern SDR* SD_Registers;
extern SDCMD CMD;
extern uint32_t DAT_buffer[8192];

void CMD_send(CMDR _CMD);
void read_single_sector(uint32_t _sector);
void read_multi_sector(uint32_t _sector);

void IRQ_read();
