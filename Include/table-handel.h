#pragma once
#include<Custom-type.h>
#include<Kernel-modules.h>
#include<Stringz.h>
#include<SD.h>
#include<UART.h>
#include<Kstd.h>
#include<ZeroShell.h>

extern void Synch_handel();
extern void IRQh_handel();
extern void SVC_handel();
extern void SyncE_handel();