#include "instructions.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>

static const OpFuncPtr NIBLE_TABLE[16] = {
    NULL, op_1NNN, op_2NNN, op_3XNN,
    op_4XNN, op_5XY0, op_6XNN, op_7XNN,
    NULL, op_9XY0, op_ANNN, op_BNNN,
    op_CXNN, op_DXYN, NULL, NULL};

OpFuncPtr get_instruction(uint16_t opcode) {
    uint8_t nibble = (opcode & 0xF000) >> 12;

    if (nibble == 0x0) {
        switch (opcode & 0x00FF) {
            case 0x00E0:
                return op_00E0;
            case 0x00EE:
                return op_00EE;
        }
    }

    if (nibble == 0x8) {
        switch (opcode & 0x000F) {
            case 0x0:
                return op_8XY0;
            case 0x1:
                return op_8XY1;
            case 0x2:
                return op_8XY2;
            case 0x3:
                return op_8XY3;
            case 0x4:
                return op_8XY4;
            case 0x5:
                return op_8XY5;
            case 0x6:
                return op_8XY6;
            case 0x7:
                return op_8XY7;
            case 0xE:
                return op_8XYE;
        }
    }

    if (nibble == 0xE) {
        switch (opcode & 0x00FF) {
            case 0x9E:
                return op_EX9E;
            case 0xA1:
                return op_EXA1;
        }
    }

    if (nibble == 0xF) {
        switch (opcode & 0x00FF) {
            case 0x07:
                return op_FX07;
            case 0x0A:
                return op_FX0A;
            case 0x15:
                return op_FX15;
            case 0x18:
                return op_FX18;
            case 0x1E:
                return op_FX1E;
            case 0x29:
                return op_FX29;
            case 0x33:
                return op_FX33;
            case 0x55:
                return op_FX55;
            case 0x65:
                return op_FX65;
        }
    }

    return NIBLE_TABLE[nibble];
}

void op_00E0(chip8_t* cpu, uint16_t opcode) {
    memset(cpu->display, 0, DISPLAY_WIDTH * DISPLAY_HEIGHT);
    cpu->is_redraw_needed = true;
}

void op_00EE(chip8_t* cpu, uint16_t opcode) {
    cpu->sp--;
    cpu->pc = cpu->stack[cpu->sp];
}

void op_1NNN(chip8_t* cpu, uint16_t opcode) {
    uint16_t address = opcode & 0x0FFF;

    cpu->pc = address;
}

void op_2NNN(chip8_t* cpu, uint16_t opcode) {
    uint16_t address = opcode & 0x0FFF;

    cpu->stack[cpu->sp] = cpu->pc;
    cpu->sp++;
    cpu->pc = address;
}

void op_3XNN(chip8_t* cpu, uint16_t opcode) {
    uint8_t vx = (opcode & 0x0F00) >> 8;
    uint8_t value = opcode & 0x00FF;

    if (cpu->v[vx] == value) {
        cpu->pc += 2;
    }
}

void op_4XNN(chip8_t* cpu, uint16_t opcode) {
    uint8_t vx = (opcode & 0x0F00) >> 8;
    uint8_t value = opcode & 0x00FF;

    if (cpu->v[vx] != value) {
        cpu->pc += 2;
    }
}

void op_5XY0(chip8_t* cpu, uint16_t opcode) {
    uint8_t vx = (opcode & 0x0F00) >> 8;
    uint8_t vy = (opcode & 0x00F0) >> 4;

    if (cpu->v[vx] == cpu->v[vy]) {
        cpu->pc += 2;
    }
}

void op_6XNN(chip8_t* cpu, uint16_t opcode) {
    uint8_t vx = (opcode & 0x0F00) >> 8;
    uint8_t value = opcode & 0x00FF;

    cpu->v[vx] = value;
}

void op_7XNN(chip8_t* cpu, uint16_t opcode) {
    uint8_t vx = (opcode & 0x0F00) >> 8;
    uint8_t value = opcode & 0x00FF;

    cpu->v[vx] += value;
}

void op_8XY0(chip8_t* cpu, uint16_t opcode) {
    uint8_t vx = (opcode & 0x0F00) >> 8;
    uint8_t vy = (opcode & 0x00F0) >> 4;

    cpu->v[vx] = cpu->v[vy];
}

void op_8XY1(chip8_t* cpu, uint16_t opcode) {
    uint8_t vx = (opcode & 0x0F00) >> 8;
    uint8_t vy = (opcode & 0x00F0) >> 4;

    cpu->v[vx] |= cpu->v[vy];
    cpu->v[0xF] = 0;  // Quirk, refer to https://github.com/Timendus/chip8-test-suite?tab=readme-ov-file#the-test
}

void op_8XY2(chip8_t* cpu, uint16_t opcode) {
    uint8_t vx = (opcode & 0x0F00) >> 8;
    uint8_t vy = (opcode & 0x00F0) >> 4;

    cpu->v[vx] &= cpu->v[vy];
    cpu->v[0xF] = 0;  // Quirk, refer to https://github.com/Timendus/chip8-test-suite?tab=readme-ov-file#the-test
}

void op_8XY3(chip8_t* cpu, uint16_t opcode) {
    uint8_t vx = (opcode & 0x0F00) >> 8;
    uint8_t vy = (opcode & 0x00F0) >> 4;

    cpu->v[vx] ^= cpu->v[vy];
    cpu->v[0xF] = 0;  // Quirk, refer to https://github.com/Timendus/chip8-test-suite?tab=readme-ov-file#the-test
}

void op_8XY4(chip8_t* cpu, uint16_t opcode) {
    uint8_t vx = (opcode & 0x0F00) >> 8;
    uint8_t vy = (opcode & 0x00F0) >> 4;

    uint16_t sum = cpu->v[vx] + cpu->v[vy];
    cpu->v[vx] = sum & 0xFF;
    cpu->v[0xF] = sum > 0xFF;
}

void op_8XY5(chip8_t* cpu, uint16_t opcode) {
    uint8_t vx = (opcode & 0x0F00) >> 8;
    uint8_t vy = (opcode & 0x00F0) >> 4;

    int8_t sub = cpu->v[vx] - cpu->v[vy];
    cpu->v[vx] = sub;
    cpu->v[0xF] = sub >= 0;
}

void op_8XY6(chip8_t* cpu, uint16_t opcode) {
    uint8_t vx = (opcode & 0x0F00) >> 8;
    uint8_t vy = (opcode & 0x00F0) >> 4;

    uint8_t y = cpu->v[vy];
    cpu->v[vx] = y >> 1;
    cpu->v[0xF] = y & 0x1;
}

void op_8XY7(chip8_t* cpu, uint16_t opcode) {
    uint8_t vx = (opcode & 0x0F00) >> 8;
    uint8_t vy = (opcode & 0x00F0) >> 4;

    int8_t sub = cpu->v[vy] - cpu->v[vx];
    cpu->v[vx] = sub;
    cpu->v[0xF] = sub >= 0;
}

void op_8XYE(chip8_t* cpu, uint16_t opcode) {
    uint8_t vx = (opcode & 0x0F00) >> 8;
    uint8_t vy = (opcode & 0x00F0) >> 4;

    uint8_t y = cpu->v[vy];
    cpu->v[vx] = y << 1;
    cpu->v[0xF] = (y & 0x80) ? 1 : 0;
}

void op_9XY0(chip8_t* cpu, uint16_t opcode) {
    uint8_t vx = (opcode & 0x0F00) >> 8;
    uint8_t vy = (opcode & 0x00F0) >> 4;

    if (cpu->v[vx] != cpu->v[vy]) {
        cpu->pc += 2;
    }
}

void op_ANNN(chip8_t* cpu, uint16_t opcode) {
    uint16_t address = opcode & 0x0FFF;

    cpu->i = address;
}

void op_BNNN(chip8_t* cpu, uint16_t opcode) {
    uint16_t address = opcode & 0x0FFF;

    cpu->pc = cpu->v[0] + address;
}

void op_CXNN(chip8_t* cpu, uint16_t opcode) {
    uint8_t vx = (opcode & 0x0F00) >> 8;
    uint8_t value = opcode & 0x00FF;

    srand(time(NULL));
    uint8_t rand_byte = rand() % 256;
    cpu->v[vx] = rand_byte & value;
}

void op_DXYN(chip8_t* cpu, uint16_t opcode) {
    // Don't draw if we're waiting for a redraw
    // Quirk, refer to https://github.com/Timendus/chip8-test-suite?tab=readme-ov-file#the-test
    if (cpu->is_redraw_needed) {
        cpu->pc -= 2;  // Stay on this instruction
        return;
    }

    uint8_t vx = (opcode & 0x0F00) >> 8;
    uint8_t vy = (opcode & 0x00F0) >> 4;
    uint8_t height = opcode & 0x000F;

    uint8_t init_x = cpu->v[vx] % DISPLAY_WIDTH;
    uint8_t init_y = cpu->v[vy] % DISPLAY_HEIGHT;

    cpu->v[0xF] = 0;
    for (int row = 0; row < height; row++) {
        uint8_t* sprite_row = &cpu->memory[cpu->i + row];
        for (int col = 0; col < 8; col++) {
            if (!(*sprite_row & (0x80 >> col))) {
                continue;  // Skip if pixel is not set
            }

            int x = init_x + col;
            int y = init_y + row;

            if (x >= DISPLAY_WIDTH || y >= DISPLAY_HEIGHT) {
                continue;  // Clip sprites at screen boundaries, instead of wrapping
                           // Quirk, refer to https://github.com/Timendus/chip8-test-suite?tab=readme-ov-file#the-test
            }

            if (cpu->display[y * DISPLAY_WIDTH + x]) {
                cpu->v[0xF] = 1;  // Set collision flag if pixel is already set
            }

            cpu->display[y * DISPLAY_WIDTH + x] ^= true;
        }
    }
    cpu->is_redraw_needed = true;
}

void op_EX9E(chip8_t* cpu, uint16_t opcode) {
    uint8_t vx = (opcode & 0x0F00) >> 8;

    if (cpu->keyboard[cpu->v[vx]]) {
        cpu->pc += 2;
    }
}

void op_EXA1(chip8_t* cpu, uint16_t opcode) {
    uint8_t vx = (opcode & 0x0F00) >> 8;

    if (!cpu->keyboard[cpu->v[vx]]) {
        cpu->pc += 2;
    }
}

void op_FX07(chip8_t* cpu, uint16_t opcode) {
    uint8_t vx = (opcode & 0x0F00) >> 8;

    cpu->v[vx] = cpu->delay_timer;
}

void op_FX0A(chip8_t* cpu, uint16_t opcode) {
    uint8_t vx = (opcode & 0x0F00) >> 8;

    for (int i = 0; i < 16; i++) {
        if (cpu->keyboard[i]) {
            cpu->v[vx] = i;
            return;
        }
    }
    cpu->pc -= 2;  // Revert program counter if no key is pressed
}

void op_FX15(chip8_t* cpu, uint16_t opcode) {
    uint8_t vx = (opcode & 0x0F00) >> 8;

    cpu->delay_timer = cpu->v[vx];
}

void op_FX18(chip8_t* cpu, uint16_t opcode) {
    uint8_t vx = (opcode & 0x0F00) >> 8;

    cpu->sound_timer = cpu->v[vx];
}

void op_FX1E(chip8_t* cpu, uint16_t opcode) {
    uint8_t vx = (opcode & 0x0F00) >> 8;

    cpu->i += cpu->v[vx];
}

void op_FX29(chip8_t* cpu, uint16_t opcode) {
    uint8_t vx = (opcode & 0x0F00) >> 8;

    cpu->i = FONTSET_START_ADDR + (cpu->v[vx] * 5);
}

void op_FX33(chip8_t* cpu, uint16_t opcode) {
    uint8_t vx = (opcode & 0x0F00) >> 8;

    cpu->memory[cpu->i] = cpu->v[vx] / 100;
    cpu->memory[cpu->i + 1] = (cpu->v[vx] / 10) % 10;
    cpu->memory[cpu->i + 2] = cpu->v[vx] % 10;
}

void op_FX55(chip8_t* cpu, uint16_t opcode) {
    uint8_t vx = (opcode & 0x0F00) >> 8;

    for (int i = 0; i <= vx; i++) {
        cpu->memory[cpu->i + i] = cpu->v[i];
    }
    cpu->i += vx + 1;  // Quirk, refer to https://github.com/Timendus/chip8-test-suite?tab=readme-ov-file#the-test
}

void op_FX65(chip8_t* cpu, uint16_t opcode) {
    uint8_t vx = (opcode & 0x0F00) >> 8;

    for (int i = 0; i <= vx; i++) {
        cpu->v[i] = cpu->memory[cpu->i + i];
    }
    cpu->i += vx + 1;  // Quirk, refer to https://github.com/Timendus/chip8-test-suite?tab=readme-ov-file#the-test
}
