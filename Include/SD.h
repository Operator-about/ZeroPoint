#pragma once

#include<Kernel-modules.h>
#include<std.h>
#include<Custom-type.h>
#include<Stringz.h>

void SDC_init();
void SD_CMD(SDCMD _CMD);
void SD_get_status();
void SD_answer();

void SD_barrier(int _seconds_wait);

void read_block(uint32_t _block_number);

uint32_t FS_init();

void ACMD41_check(SDCMD _ACMD41, SDCMD _CMD55);
void CMD8_check();