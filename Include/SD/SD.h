#pragma once

#include<Kernel-modules.h>
#include<std.h>
#include<Custom-type.h>
#include<Stringz.h>
#include<exFAT.h>

extern SDCMD CMD;

void read_single_sector(uint32_t _sector);
void read_multi_sector(uint32_t _sector);

