#pragma once

#include<Kernel-modules.h>
#include<std.h>
#include<Custom-type.h>
#include<Stringz.h>

extern uint32_t SD;
extern SDR* SD_Registers;
extern volatile uint8_t* DAT_buffer;
extern SDCMD CMD;

void SD_card_reinit();
void SD_mode();
void CMD_send(CMDR _CMD);
void read_single_sector(uint32_t _sector);
void read_multi_sector(uint32_t _sector);
void write_sector();

void IRQ_read();
