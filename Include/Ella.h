//Execute logical local analization - Ella
/*
    Ella - Система для отловки будущих ошибок в ОС
*/
#pragma Once

#include<Custom-type.h>
#include<Kernel-modules.h>
#include<Interrupts.h>
#include<Stringz.h>

extern struct Ring_buffer Debug_buffer;

void debugf(char _info[]);