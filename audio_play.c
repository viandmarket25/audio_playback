#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio/miniaudio.h"
#include <stdio.h>
#include <stdlib.h>

// Define constants for audio format, channels, and sample rate
#define AUDIO_FORMAT       ma_format_f32
#define NUM_CHANNELS       2
#define SAMPLE_RATE        44100

// Global variables to manage the microphone capture and playback
ma_device playbackDevice;
ma_device captureDevice;

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
    // Process audio data here (e.g., apply noise cancellation or other processing)
    // For simplicity, we just pass audio data from the capture to the playback
    ma_decoder_read_pcm_frames((ma_decoder*)pInput, pOutput, frameCount, NULL);
}

int main(int argc, char** argv) {
    ma_result result;
    ma_decoder decoder;

    if (argc < 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return -1;
    }

    // Initialize Mini Audio
    if (ma_context_init(NULL, 0, NULL, NULL) != MA_SUCCESS) {
        printf("Failed to initialize Mini Audio.\n");
        return -2;
    }

    // Initialize audio playback device
    ma_device_config playbackConfig = ma_device_config_init(ma_device_type_playback);
    playbackConfig.playback.format   = AUDIO_FORMAT;
    playbackConfig.playback.channels = NUM_CHANNELS;
    playbackConfig.sampleRate        = SAMPLE_RATE;
    playbackConfig.dataCallback      = data_callback;

    result = ma_decoder_init_file(argv[1], NULL, &decoder);
    if (result != MA_SUCCESS) {
        printf("Could not load file: %s\n", argv[1]);
        ma_context_uninit(NULL);
        return -3;
    }

    playbackConfig.pUserData = &decoder;

    if (ma_device_init(NULL, &playbackConfig, &playbackDevice) != MA_SUCCESS) {
        printf("Failed to open playback device.\n");
        ma_decoder_uninit(&decoder);
        ma_context_uninit(NULL);
        return -4;
    }

    // Initialize audio capture (microphone) device
    ma_device_config captureConfig = ma_device_config_init(ma_device_type_capture);
    captureConfig.capture.format   = AUDIO_FORMAT;
    captureConfig.capture.channels = NUM_CHANNELS;
    captureConfig.sampleRate      = SAMPLE_RATE;
    captureConfig.dataCallback    = data_callback;

    if (ma_device_init(NULL, &captureConfig, &captureDevice) != MA_SUCCESS) {
        printf("Failed to open capture device (microphone).\n");
        ma_device_uninit(&playbackDevice);
        ma_decoder_uninit(&decoder);
        ma_context_uninit(NULL);
        return -5;
    }

    // Start audio devices
    if (ma_device_start(&playbackDevice) != MA_SUCCESS || ma_device_start(&captureDevice) != MA_SUCCESS) {
        printf("Failed to start audio devices.\n");
        ma_device_uninit(&playbackDevice);
        ma_device_uninit(&captureDevice);
        ma_decoder_uninit(&decoder);
        ma_context_uninit(NULL);
        return -6;
    }

    printf("Press Enter to quit...");
    getchar();

    // Cleanup and shutdown
    ma_device_uninit(&playbackDevice);
    ma_device_uninit(&captureDevice);
    ma_decoder_uninit(&decoder);
    ma_context_uninit(NULL);

    return 0;
}
