#pragma once

#include <stdbool.h>
#include <stdint.h>

#define FONTSET_SIZE 80
#define MEMORY_SIZE 4096
#define STACK_SIZE 16
#define REGISTERS_COUNT 16
#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define KEYBOARD_SIZE 16

#define FONTSET_START_ADDR 0x50
#define PC_START_ADDR 0x200

static const uint8_t FONTSET[FONTSET_SIZE] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0
    0x20, 0x60, 0x20, 0x20, 0x70,  // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0,  // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0,  // 3
    0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
    0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0,  // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0,  // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
    0xF0, 0x80, 0x80, 0x80, 0xF0,  // C
    0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
    0xF0, 0x80, 0xF0, 0x80, 0x80   // F
};

typedef struct {
    uint8_t memory[MEMORY_SIZE];  // 4KB RAM memory
    uint16_t pc;                  // Program counter register

    uint16_t stack[STACK_SIZE];  // Call stack for subroutines
    uint8_t sp;                  // Stack pointer

    uint8_t v[REGISTERS_COUNT];  // 16 general purpose registers V0-VF
    uint16_t i;                  // Index register for memory operations

    uint8_t delay_timer;  // Delay timer, decrements at 60Hz
    uint8_t sound_timer;  // Sound timer, decrements at 60Hz, beeps when >0

    bool display[DISPLAY_WIDTH * DISPLAY_HEIGHT];  // 64x32 monochrome display buffer
    bool is_redraw_needed;                         // Display refresh flag

    bool keyboard[KEYBOARD_SIZE];  // 16-key hexadecimal keypad state
} chip8_t;
