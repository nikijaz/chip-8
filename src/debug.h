#pragma once

#include <stdbool.h>

#include "SDL3/SDL_events.h"
#include "chip8_t.h"

void debug_update(chip8_t* chip8);
void debug_handle_key_event(SDL_Event* event, chip8_t* cpu);
