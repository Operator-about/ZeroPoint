.PHONY: all clean

CX := aarch64-none-elf-gcc
AS := aarch64-none-elf-as
LINK := ./
CSRC := ./src/aarch64/c-source/
ASRC := ./src/aarch64/asm/
INC := -I./include/common/ -I./include/exFAT/ -I./include/SD/ -I./include/SD/ -I./include/UART/ -I./include/  

all: ZeroPoint.elf

kernel.o: $(CSRC)kernel.c
	$(CX) $(INC) -c $^ -o $@

startup-asm.o: $(ASRC)/boot/startup.asm
	$(AS) $^ -o $@

table-asm.o: $(ASRC)boot/table-vector.asm
	$(AS) $^ -o $@

MMU.o: $(CSRC)MMU.c
	$(CX) $(INC) -c $^ -o $@

ZerShell.o: $(CSRC)ZeroShell.c
	$(CX) $(INC) -c $^ -o $@

Kernel-modules.o: $(CSRC)Kernel-modules.c
	$(CX) $(INC) -c $^ -o $@

Table-handel.o: $(CSRC)table-handel.c
	$(CX) $(INC) -c $^ -o $@

UARTPL011.o: $(CSRC)UART/PL011.c
	$(CX) $(INC) -c $^ -o $@

UART165050.o: $(CSRC)UART/165050.c
	$(CX) $(INC) -c $^ -o $@

SD.o: $(CSRC)SD/SD.c
	$(CX) $(INC) -c $^ -o $@

SD-DW.o: $(CSRC)SD/SD-DW.c
	$(CX) $(INC) -c $^ -o $@

SD-Standart.o: $(CSRC)SD/SD-Standart.c
	$(CX) $(INC) -c $^ -o $@

exFAT-PHY.o: $(CSRC)exFAT/exFAT-PhysicalLayer.c
	$(CX) $(INC) -c $^ -o $@

exFAT-VIRT.o: $(CSRC)exFAT/exFAT-VirtualLayer.c
	$(CX) $(INC) -c $^ -o $@

MBR.o: $(CSRC)exFAT/MBR.c
	$(CX) $(INC) -c $^ -o $@

std.o: $(CSRC)common/std.c
	$(CX) $(INC) -c $^ -o $@

math.o: $(CSRC)common/Mathz.c
	$(CX) $(INC) -c $^ -o $@

stringz.o: $(CSRC)common/Stringz.c
	$(CX) $(INC) -c $^ -o $@

ZeroPoint.elf: kernel.o stringz.o math.o std.o MBR.o exFAT-VIRT.o exFAT-PHY.o SD-Standart.o SD-DW.o SD.o UART165050.o UARTPL011.o Table-handel.o ZerShell.o Kernel-modules.o MMU.o table-asm.o startup-asm.o
	$(CX) -nostdlib -T $(LINK)linker.ld $^ -o $@
