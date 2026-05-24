#include<Interrupts.h>
#include<Kernel-modules.h>
#include<Ella.h>
#include<MMU.h>
#include<Stringz.h>
#include<stl.h>

struct UART* UART;
struct GICv3 GICv3;
struct GICv2 GICv2;
struct GICv3_registers_data GICv3_registers;

int main(void){
    Rx_buffer.head=-1;
    MMU_init();
    
    __asm__("MOV %0, X1" : "=r"(UART));

    char _keyboard_buffer_input[100];
    char _info_buffer[100];
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

