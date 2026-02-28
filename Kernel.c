#include"Headers/Interrput.h"

//0 - QEMU
//1 - Qcom
//2 - Md
#define CPU 0

struct UART UART;
struct GIC GIC;


int main(void){
#if CPU == 0
    GIC_common_configure();
    UART_common_configure();
#elif CPU == 1
    UART_qcom_configure();
#elif CPU == 2
    UART_md_configure();
#else
    #error "Error"          
#endif

    write("Hello");

    return 0;
}