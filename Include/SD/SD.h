#pragma once

#include<Kernel-modules.h>
#include<std.h>
#include<Custom-type.h>
#include<Stringz.h>
#include<exFAT.h>
#include<SD-Standart.h>
#include<SD-DW.h>

extern SDCMD CMD;
extern FileBuffer File;
extern HALSD SD;

void read_single_sector(uint32_t _sector);
void read_multi_sector(uint32_t _sector);

