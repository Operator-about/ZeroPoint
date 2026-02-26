#include<Headers/Interrput.h>

struct UART UART;
struct GIC GIC;


int main(){

    GIC_common_configure();

    UART_common_configure();

    write("Hello");
    
    return 0;
}