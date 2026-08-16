#pragma once

#include<Custom-type.h>
#include<Kernel-modules.h>
#include<SD.h>

extern MBR MBRM;
extern uint32_t LBA;

void init_MBR();
void get_LBA_for_exFAT();