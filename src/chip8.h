#pragma once

#include "chip8_t.h"

void init_chip8(chip8_t* cpu);
bool load_rom(chip8_t* cpu, const char* filename);
void step_chip8(chip8_t* cpu);
void step_chip8_timer(chip8_t* cpu);
