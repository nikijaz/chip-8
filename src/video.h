#pragma once

#include "chip8_t.h"

bool video_init(void);
bool video_update(chip8_t* cpu);
void video_cleanup(void);
