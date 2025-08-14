#pragma once

#include "chip8_t.h"

bool audio_init(void);
bool audio_update(chip8_t* cpu);
void audio_cleanup(void);
