#include "chip8.h"

#include <stdio.h>
#include <string.h>

#include "instructions.h"

void init_chip8(chip8_t* cpu) {
    // Copy fontset to memory
    memcpy(cpu->memory + FONTSET_START_ADDR, FONTSET, FONTSET_SIZE);

    // Set program counter to start address
    cpu->pc = PC_START_ADDR;
}

bool load_rom(chip8_t* cpu, const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        return false;
    }

    size_t bytes_read = fread(cpu->memory + PC_START_ADDR, sizeof(uint8_t), MEMORY_SIZE - PC_START_ADDR, file);
    fclose(file);

    if (bytes_read == 0) {
        return false;
    }
    return true;
}

void step_chip8(chip8_t* cpu) {
    uint16_t opcode = (cpu->memory[cpu->pc] << 8) | cpu->memory[cpu->pc + 1];
    cpu->pc += 2;
    OpFuncPtr instruction = get_instruction(opcode);
    if (instruction) {
        instruction(cpu, opcode);
    }
}

void step_chip8_timer(chip8_t* cpu) {
    if (cpu->delay_timer > 0) {
        cpu->delay_timer--;
    }
    if (cpu->sound_timer > 0) {
        cpu->sound_timer--;
    }
}
