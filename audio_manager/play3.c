#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio/miniaudio.h"
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h> // Include this for memcpy

// Define constants for audio format, channels, and sample rate
#define AUDIO_FORMAT       ma_format_f32
#define NUM_CHANNELS       2
#define SAMPLE_RATE        44100
#define BUFFER_SIZE        (SAMPLE_RATE * NUM_CHANNELS * 2) // Adjust buffer size as needed

// Circular buffer to store audio chunks
uint8_t audioBuffer[BUFFER_SIZE];
size_t audioBufferIndex = 0;

// Control to enable/disable noise cancellation
int enableNoiseCancellation = 1; // Set to 1 to enable, 0 to disable

// Equalization frequencies and gains (initialize to 0 dB)
float equalizationFrequencies[] = {250.0f, 500.0f, 1000.0f, 2000.0f, 4000.0f};
float equalizationGains[] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    ma_encoder* pEncoder = (ma_encoder*)pDevice->pUserData;
    MA_ASSERT(pEncoder != NULL);

    // Store the incoming audio data in the buffer
    size_t bytesToCopy = frameCount * NUM_CHANNELS * sizeof(float);
    if (audioBufferIndex + bytesToCopy <= BUFFER_SIZE) {
        memcpy(audioBuffer + audioBufferIndex, pInput, bytesToCopy);
        audioBufferIndex += bytesToCopy;
    } else {
        // Handle buffer overflow (optional)
    }

    // Apply noise cancellation (simple noise gate) if enabled
    if (enableNoiseCancellation) {
        float* input = (float*)pInput;
        for (ma_uint32 i = 0; i < frameCount * NUM_CHANNELS; ++i) {
            if (fabsf(input[i]) < 0.1f) {
                input[i] = 0.0f; // Suppress audio below the threshold
            }
        }
    }

    // Apply equalization (adjust gain at specific frequencies)
    float* output = (float*)pOutput;
    const float* input = (const float*)pInput;

    for (ma_uint32 i = 0; i < frameCount * NUM_CHANNELS; ++i) {
        float sample = input[i];

        // Apply gain adjustment at each specified frequency
        for (int j = 0; j < sizeof(equalizationFrequencies) / sizeof(equalizationFrequencies[0]); ++j) {
            float frequency = equalizationFrequencies[j];
            float gain = powf(10.0f, equalizationGains[j] / 20.0f); // Convert dB to linear gain

            // Apply equalization for each channel (left and right)
            output[i] = sample * gain;
        }
    }

    // Write the audio data to the encoder for saving to a file (optional)
    ma_encoder_write_pcm_frames(pEncoder, pInput, frameCount, NULL);
    
    // Playback the audio by copying the input to the output
    ma_copy_pcm_frames(pOutput, pInput, frameCount, AUDIO_FORMAT, NUM_CHANNELS);
}

int main(int argc, char** argv)
{
    ma_result result;
    ma_encoder_config encoderConfig;
    ma_encoder encoder;
    ma_device_config deviceConfig;
    ma_device device;

    if (argc < 2) {
        printf("No output file.\n");
        return -1;
    }

    encoderConfig = ma_encoder_config_init(ma_encoding_format_wav, ma_format_f32, 2, 44100);

    if (ma_encoder_init_file(argv[1], &encoderConfig, &encoder) != MA_SUCCESS) {
        printf("Failed to initialize output file.\n");
        return -1;
    }

    deviceConfig = ma_device_config_init(ma_device_type_duplex);
    deviceConfig.capture.format   = encoder.config.format;
    deviceConfig.capture.channels = encoder.config.channels;
    deviceConfig.sampleRate       = encoder.config.sampleRate;
    deviceConfig.playback.format  = AUDIO_FORMAT;
    deviceConfig.playback.channels = NUM_CHANNELS;
    deviceConfig.dataCallback     = data_callback;
    deviceConfig.pUserData        = &encoder;

    result = ma_device_init(NULL, &deviceConfig, &device);
    if (result != MA_SUCCESS) {
        printf("Failed to initialize capture device.\n");
        return -2;
    }

    result = ma_device_start(&device);
    if (result != MA_SUCCESS) {
        ma_device_uninit(&device);
        printf("Failed to start device.\n");
        return -3;
    }

    printf("Press Enter to stop recording...\n");

    // Continuously read user input to toggle noise cancellation on/off and adjust equalization
    while (1) {
        char userInput[32];
        if (fgets(userInput, sizeof(userInput), stdin) == NULL) {
            break; // Exit the loop if there's an error or end of input
        }

        // Toggle noise cancellation on/off when the user presses 'n' key
        if (userInput[0] == 'n') {
            enableNoiseCancellation = !enableNoiseCancellation;
            printf("Noise cancellation %s\n", enableNoiseCancellation ? "enabled" : "disabled");
        }

        // Adjust equalization gain when the user enters a frequency and gain in the format "frequency gain" (e.g., "1000 3")
        int frequency;
        float gain;
        if (sscanf(userInput, "%d %f", &frequency, &gain) == 2) {
            // Find the index of the specified frequency
            for (int i = 0; i < sizeof(equalizationFrequencies) / sizeof(equalizationFrequencies[0]); ++i) {
                if (equalizationFrequencies[i] == (float)frequency) {
                    // Set the gain for the specified frequency
                    equalizationGains[i] = gain;
                    printf("Equalization gain at %d Hz set to %.2f dB\n", frequency, gain);
                    break;
                }
            }
        }
    }
    
    // Optionally, you can save the recorded audio to a file here
    // ma_encoder_uninit(&encoder);

    ma_device_uninit(&device);

    return 0;
}
