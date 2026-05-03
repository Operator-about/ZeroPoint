#include<Custom-type.h>
#include<GIC.h>
#include<Kernel-modules.h>
#include<Stringz.h>

extern struct Ring_buffer Tx_buffer;
extern struct Ring_buffer Rx_buffer;
extern struct UART* UARTPL011;

void UARTPL011_init();
struct BRR_UART BRR_calculate(int _BRR, int _GHZ);
void write(char _buffer[]);
void read(char _buffer[]);