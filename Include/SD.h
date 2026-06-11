#pragma once

#include<Kernel-modules.h>
#include<std.h>
#include<Custom-type.h>
#include<Stringz.h>

extern SDR* SD_Registers;

void SDC_init();
void SD_CMD(SDCMD _CMD);
void SD_get_status(SDCMD _CMD, int _CMD_index);
void SD_answer();