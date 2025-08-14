#include "video.h"

#include <SDL3/SDL.h>

#define SCREEN_SCALE_FACTOR 10
#define PIXEL_ON_COLOR 0xFFFFFFFF
#define PIXEL_OFF_COLOR 0x00000000

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Texture* texture = NULL;

bool video_init(void) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("CHIP-8", DISPLAY_WIDTH * SCREEN_SCALE_FACTOR, DISPLAY_HEIGHT * SCREEN_SCALE_FACTOR, SDL_WINDOW_RESIZABLE);
    if (!window) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to create window: %s", SDL_GetError());
        video_cleanup();
        return false;
    }

    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to create renderer: %s", SDL_GetError());
        video_cleanup();
        return false;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    if (!texture) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to create texture: %s", SDL_GetError());
        video_cleanup();
        return false;
    }

    // Set fixed aspect ratio
    if (!SDL_SetRenderLogicalPresentation(renderer, DISPLAY_WIDTH, DISPLAY_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to set logical presentation: %s", SDL_GetError());
        video_cleanup();
        return false;
    }

    // Use nearest neighbor scaling
    if (!SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to set texture scale mode: %s", SDL_GetError());
        video_cleanup();
        return false;
    }

    return true;
}

bool video_update(chip8_t* cpu) {
    if (!cpu->is_redraw_needed) {
        return true;
    }

    // Convert monochrome display to color buffer
    uint32_t buffer[DISPLAY_WIDTH * DISPLAY_HEIGHT];
    for (int i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT; i++) {
        buffer[i] = cpu->display[i] ? PIXEL_ON_COLOR : PIXEL_OFF_COLOR;
    }

    if (!SDL_UpdateTexture(texture, NULL, buffer, DISPLAY_WIDTH * sizeof(uint32_t))) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to update texture: %s", SDL_GetError());
        return false;
    }
    if (!SDL_RenderClear(renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to clear renderer: %s", SDL_GetError());
        return false;
    }
    if (!SDL_RenderTexture(renderer, texture, NULL, NULL)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to render texture: %s", SDL_GetError());
        return false;
    }
    if (!SDL_RenderPresent(renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to present renderer: %s", SDL_GetError());
        return false;
    }

    cpu->is_redraw_needed = false;
    return true;
}

void video_cleanup(void) {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = NULL;
    }
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = NULL;
    }
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}
