#include<Kernel-modules.h>
#include<Ella.h>
#include<MMU.h>
#include<Stringz.h>
#include<std.h>

UART0 UART;
GICCv2* GICv2;

int main(void){
    MMU_init();

    init_t_buffer();
    Rx_buffer.head = -1;
    Rx_buffer.end = 0;
    Rx_buffer.tail = 0;

    __asm__("MOV %0, X10" : "=r"(UART.UART_DR));
    __asm__("MOV %0, X11" : "=r"(UART.UART_FR));
    __asm__("MOV %0, X12" : "=r"(UART.UART_MIS));
    __asm__("MOV %0, X13" : "=r"(UART.UART_IMSC));
    __asm__("MOV %0, X14" : "=r"(UART.UART_RIS));
    __asm__("MOV %0, X15" : "=r"(UART.UART_ICR));
    if(GIC_version_check() == 2){
        __asm__("MOV %0, X16" : "=r"(GICv2));
    }
    __asm__("ISB");

    __asm__("MOV X10, XZR");
    __asm__("MOV X11, XZR");
    __asm__("MOV X12, XZR");
    __asm__("MOV X13, XZR");
    __asm__("MOV X14, XZR");
    __asm__("MOV X15, XZR");
    __asm__("MOV X16, XZR");

    __asm__("ISB");

    __asm__("MSR DAIFClr, #2");
    char _keyboard_buffer_input[100];
    clear_buffer(_keyboard_buffer_input);
    char _info_buffer[100];
    print("Welcome! Load OS success completed! Please type - help for get more information\r\n");
    print("Or input command - about. For get information about OS\r\n");
    while(1){
        print(">>");
        input(_keyboard_buffer_input);
        if(compare_s(_keyboard_buffer_input, "about") == 1){
            clear_buffer(_keyboard_buffer_input);
            print("=========================================================\r\n");
            print("_____  _____  _____  _____  _____  _____  +  _____  +    \r\n");
            print("   // ||     ||   ||||   ||||   ||||   || | ||   ||-|-   \r\n");
            print("  //  ||____ ||     ||   ||||___||||   || | ||   || |    \r\n");
            print(" //   ||     ||     ||   ||||     ||   || | ||   || |    \r\n");
            print("//___ ||____ ||     ||___||||     ||___|| | ||   || |___ \r\n");
            print("=========================================================\r\n");
            print("Kernel: v0.0.3(pre-alpha)\r\n");
        }
        else if(compare_s(_keyboard_buffer_input, "help") == 1){
            clear_buffer(_keyboard_buffer_input);
            print("Attention! This list command work only this terminal:\r\n");
            print("about - command for shows name OS and kernel versions\r\n");
            print("help - shows this list\r\n");
        }
        else{
            clear_buffer(_keyboard_buffer_input);
            print("Unknow command. Please input command: help - for more information\r\n");
        }
        __asm__("NOP");
    }
}

