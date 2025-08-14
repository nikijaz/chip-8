#include "audio.h"

#include <SDL3/SDL.h>
#include <math.h>

#define BUFFER_SIZE 1024
#define SAMPLE_RATE 48000
#define AMPLITUDE 0.15  // Volume level (0.0 to 1.0)
#define FREQUENCY 500   // B4 note frequency

#define PHASE_INCREMENT (2 * M_PI * FREQUENCY / SAMPLE_RATE)  // Phase increment for sine wave generation

static SDL_AudioDeviceID audio_device = 0;
static SDL_AudioStream* audio_stream = NULL;

static float current_phase = 0;
static void audio_callback(void* userdata, SDL_AudioStream* stream, int additional_amount, int total_amount) {
    int sample_amount = total_amount / sizeof(float);  // `total_amount` is the total number of bytes, which needs to be converted to number of float samples

    // Generate sine wave samples
    static float buffer[BUFFER_SIZE];  // Static buffer to avoid dynamic allocation
    while (sample_amount > 0) {
        int chunk_size = (sample_amount < BUFFER_SIZE) ? sample_amount : BUFFER_SIZE;

        for (int i = 0; i < chunk_size; i++) {
            buffer[i] = sinf(current_phase) * AMPLITUDE;
            current_phase += PHASE_INCREMENT;
            if (current_phase >= 2 * M_PI) current_phase -= 2 * M_PI;
        }

        SDL_PutAudioStreamData(stream, buffer, chunk_size * sizeof(float));
        sample_amount -= chunk_size;
    }
}

bool audio_init(void) {
    if (!SDL_Init(SDL_INIT_AUDIO)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to initialize SDL audio: %s", SDL_GetError());
        return false;
    }

    // Set audio configuration
    SDL_AudioSpec config = {
        .freq = SAMPLE_RATE,
        .format = SDL_AUDIO_F32,
        .channels = 1,
    };

    // Open audio device
    audio_device = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &config);
    if (!audio_device) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to open audio device: %s", SDL_GetError());
        audio_cleanup();
        return false;
    }

    // Immediately pause the audio device to prevent it from playing noise
    SDL_PauseAudioDevice(audio_device);

    audio_stream = SDL_CreateAudioStream(&config, &config);
    if (!audio_stream) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to create audio stream: %s", SDL_GetError());
        audio_cleanup();
        return false;
    }

    // Set audio callback, which generates sound when requested
    if (!SDL_SetAudioStreamGetCallback(audio_stream, audio_callback, NULL)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to set audio callback: %s", SDL_GetError());
        audio_cleanup();
        return false;
    }

    if (!SDL_BindAudioStream(audio_device, audio_stream)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to bind audio stream: %s", SDL_GetError());
        audio_cleanup();
        return false;
    }

    return true;
}

bool audio_update(chip8_t* cpu) {
    if (cpu->sound_timer > 0) {
        return SDL_ResumeAudioDevice(audio_device);
    } else {
        return SDL_PauseAudioDevice(audio_device);
    }
}

void audio_cleanup(void) {
    if (audio_stream) {
        SDL_DestroyAudioStream(audio_stream);
        audio_stream = NULL;
    }
    if (audio_device) {
        SDL_CloseAudioDevice(audio_device);
        audio_device = 0;
    }
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}
