#define MINIAUDIO_IMPLEMENTATION
#include "../miniaudio/miniaudio.h"
#include <stdio.h>
#include <stdint.h>

// Define constants for audio format, channels, and sample rate
#define AUDIO_FORMAT       ma_format_f32
#define NUM_CHANNELS       2
#define SAMPLE_RATE        44100
#define BUFFER_SIZE        (SAMPLE_RATE * NUM_CHANNELS * 2) // Adjust buffer size as needed

// Circular buffer to store audio chunks
uint8_t audioBuffer[BUFFER_SIZE];
size_t audioBufferIndex = 0;

void testFFi(){
    printf("ffi working! \n");
}
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

    // Write the audio data to the encoder for saving to a file
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
    getchar();
    
    // Optionally, you can save the recorded audio to a file here
    // ma_encoder_uninit(&encoder);

    ma_device_uninit(&device);

    return 0;
}
