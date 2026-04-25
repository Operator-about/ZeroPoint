#define DEBUG 1
#define VERISON 001
#include<Custom-type.h>
#include<stdalign.h>
extern long EL3_to_EL1();
extern long zero_PSTATE();
extern long MMU_active();
extern long VBAR_set();
void clear_buffer(char _buffer[]);
int length(char _buffer[]);
int compare_s(char _comble_buffer[], char _src_buffer[]);