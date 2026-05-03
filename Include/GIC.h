#pragma once
#include<Custom-type.h>

extern struct GICv3 GICv3_;
extern struct GICv2 GICv2_;
extern struct GICv3_registers_data GICv3_registers;

void GICDv2_init();
void GICCv2_init();

void GICDv3_init();
void GICRv3_init();
void GICv3_registers_init();
void GICv3_affinity_router();