#include "debug.h"

#include <stdarg.h>
#include <stdio.h>

#include "chip8_t.h"

#define DEBUGGER_COLS 16
#define DEBUGGER_ROWS 16
#define DEBUGGER_PAGE_SIZE (DEBUGGER_COLS * DEBUGGER_ROWS)

#define DEBUGGER_MODE_COUNT 2
typedef enum {
    OVERVIEW,
    MEMORY_DUMP,
} debugger_mode_t;
static debugger_mode_t mode = OVERVIEW;

static const int DEBUGGER_PAGE_COUNT = MEMORY_SIZE / DEBUGGER_PAGE_SIZE + (MEMORY_SIZE % DEBUGGER_PAGE_SIZE ? 1 : 0);
static int page = 0;

void printf_at(int i, int j, const char* format, ...) {
    va_list args;
    va_start(args, format);
    printf("\033[%d;%dH", i, j);
    vprintf(format, args);
    va_end(args);
}

void debug_overview(chip8_t* chip8) {
    // Print registers
    printf_at(1, 1, "Registers:");
    for (int i = 0; i < 16; i++) {
        printf_at(i + 2, 1, "V%X: 0x%02X\n", i, chip8->v[i]);
    }

    // Print stack
    printf_at(1, 20, "Stack:");
    for (int i = 0; i < STACK_SIZE; i++) {
        printf_at(i + 2, 20, "0x%04X", chip8->stack[i]);
    }

    // Print PC, I, SP
    printf_at(1, 35, "PC: 0x%04X", chip8->pc);
    printf_at(2, 35, "I:  0x%04X", chip8->i);
    printf_at(3, 35, "SP: 0x%02X", chip8->sp);

    // Print timers
    printf_at(5, 35, "Delay Timer: %d", chip8->delay_timer);
    printf_at(6, 35, "Sound Timer: %d", chip8->sound_timer);

    // Move to new row
    printf_at(STACK_SIZE + 2, 1, "");
}

void debug_memory_dump(chip8_t* chip8, int page) {
    // Header line
    for (int i = 0; i <= 0xF; i++) {
        printf_at(1, 6 + i * 3, "%02X", i);
    }

    int max = DEBUGGER_PAGE_SIZE * (page + 1) < MEMORY_SIZE ? DEBUGGER_PAGE_SIZE * (page + 1) : MEMORY_SIZE;
    for (int i = DEBUGGER_PAGE_SIZE * page; i < max; i++) {
        // Left address column
        if (i % DEBUGGER_COLS == 0) {
            printf_at(i / DEBUGGER_COLS % DEBUGGER_ROWS + 2, 1, "%04X ", i);
        }
        // Data columns
        printf_at(i / DEBUGGER_COLS % DEBUGGER_ROWS + 2, 6 + (i % DEBUGGER_COLS) * 3, "%02X", chip8->memory[i]);
    }

    // Move to new row
    printf_at(DEBUGGER_ROWS + 2, 1, "");
}

void debug_update(chip8_t* chip8) {
    // Clear the screen
    printf("\033[2J");

    switch (mode) {
        case OVERVIEW:
            debug_overview(chip8);
            break;
        case MEMORY_DUMP:
            debug_memory_dump(chip8, page);
            break;
    }

    // Flush output
    fflush(stdout);
}

void debug_handle_key_event(SDL_Event* event, chip8_t* cpu) {
    bool is_key_down = event->type == SDL_EVENT_KEY_DOWN;

    if (is_key_down && event->key.scancode == SDL_SCANCODE_M) {
        mode = (mode + 1) % DEBUGGER_MODE_COUNT;
    }

    if (is_key_down && mode == MEMORY_DUMP && event->key.scancode == SDL_SCANCODE_COMMA) {
        page = (page - 1 + DEBUGGER_PAGE_COUNT) % DEBUGGER_PAGE_COUNT;
    }
    if (is_key_down && mode == MEMORY_DUMP && event->key.scancode == SDL_SCANCODE_PERIOD) {
        page = (page + 1) % DEBUGGER_PAGE_COUNT;
    }
}
