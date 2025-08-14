#include <SDL3/SDL_main.h>
#include <stdio.h>
#include <stdlib.h>

#include "audio.h"
#include "chip8.h"
#include "keyboard.h"
#include "video.h"

#define TARGET_FPS 60
#define FRAME_TIME_MS (1000 / TARGET_FPS)

#define CPU_HZ 800
#define INSTRUCTIONS_PER_FRAME (CPU_HZ / TARGET_FPS)

#define TIMER_HZ 60
#define TIMER_INTERVAL_MS (1000 / TIMER_HZ)

void cleanup(void);

int main(int argc, char* argv[]) {
    // Check if a ROM file was provided
    if (argc < 2) {
        printf("Usage: %s <ROM>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Initialize video and audio
    if (!video_init() || !audio_init()) {
        cleanup();
        return EXIT_FAILURE;
    }

    // Initialize CPU
    chip8_t chip8 = {0};
    init_chip8(&chip8);

    // Try loading the ROM
    if (!load_rom(&chip8, argv[1])) {
        printf("Failed to read ROM: %s\n", argv[1]);
        cleanup();
        return EXIT_FAILURE;
    }

    uint64_t last_frame_update = SDL_GetTicks();
    uint64_t last_timer_update = SDL_GetTicks();

    while (true) {
        uint64_t current_time = SDL_GetTicks();

        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT || (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_ESCAPE)) {
                cleanup();
                return EXIT_SUCCESS;
            }

            if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP) {
                handle_key_event(&event, &chip8);
            }
        }

        // Execute instructions for the current frame
        for (int i = 0; i < INSTRUCTIONS_PER_FRAME; i++) {
            step_chip8(&chip8);
        }

        // Try updating video and audio
        if (!video_update(&chip8) || !audio_update(&chip8)) {
            cleanup();
            return EXIT_FAILURE;
        }

        // Update timers if needed
        if (current_time - last_timer_update >= TIMER_INTERVAL_MS) {
            step_chip8_timer(&chip8);
            last_timer_update = current_time;
        }

        // Wait for the next frame if the current frame completed too quickly
        uint64_t current_frame_time = current_time - last_frame_update;
        if (current_frame_time < FRAME_TIME_MS) {
            SDL_Delay(FRAME_TIME_MS - current_frame_time);
        }
        last_frame_update = SDL_GetTicks();
    }

    cleanup();
    return EXIT_SUCCESS;
}

void cleanup(void) {
    video_cleanup();
    audio_cleanup();
    SDL_Quit();
}
