#include "keyboard.h"

typedef struct {
    SDL_Scancode scancode;
    uint8_t value;
} keymap_t;

// clang-format off
static const keymap_t KEYMAP[KEYBOARD_SIZE] = {
    {SDL_SCANCODE_1, 0x1}, {SDL_SCANCODE_2, 0x2}, {SDL_SCANCODE_3, 0x3}, {SDL_SCANCODE_4, 0xC},
    {SDL_SCANCODE_Q, 0x4}, {SDL_SCANCODE_W, 0x5}, {SDL_SCANCODE_E, 0x6}, {SDL_SCANCODE_R, 0xD},
    {SDL_SCANCODE_A, 0x7}, {SDL_SCANCODE_S, 0x8}, {SDL_SCANCODE_D, 0x9}, {SDL_SCANCODE_F, 0xE},
    {SDL_SCANCODE_Z, 0xA}, {SDL_SCANCODE_X, 0x0}, {SDL_SCANCODE_C, 0xB}, {SDL_SCANCODE_V, 0xF}
};
// clang-format on

void handle_key_event(SDL_Event* event, chip8_t* cpu) {
    bool is_key_down = event->type == SDL_EVENT_KEY_DOWN;
    for (int i = 0; i < KEYBOARD_SIZE; i++) {
        if (event->key.scancode == KEYMAP[i].scancode) {
            cpu->keyboard[KEYMAP[i].value] = is_key_down;
            return;
        }
    }
}
