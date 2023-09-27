#include <stdio.h>
#include <stdlib.h>
#include <portaudio.h>

#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 512

static int audioCallback(const void *inputBuffer, void *outputBuffer,
                        unsigned long framesPerBuffer,
                        const PaStreamCallbackTimeInfo *timeInfo,
                        PaStreamCallbackFlags statusFlags,
                        void *userData) {
                             // printf("%scbk\n");
    (void)timeInfo;
    (void)statusFlags;
    (void)userData;

    const float *in = (const float *)inputBuffer;
    float *out = (float *)outputBuffer;

    for (unsigned int i = 0; i < framesPerBuffer; i++) {
        // fprintf("%scp\n");
        *out++ = *in++;  // Copy input to output (passthrough)
    }

    return paContinue;
}

int main() {
    PaError err;
    PaStream *stream = NULL;

    err = Pa_Initialize();
    if (err != paNoError) {
        fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(err));
        return 1;
    }

    err = Pa_OpenDefaultStream(&stream, 1, 1, paFloat32, SAMPLE_RATE, FRAMES_PER_BUFFER, audioCallback, NULL);
    if (err != paNoError) {
        fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(err));
        return 1;
    }

    err = Pa_StartStream(stream);
    if (err != paNoError) {
        fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(err));
        return 1;
    }

    // ::::::::::::: if no error is encountered so far, 
    // ::::::::::::: start playing audio

    printf("Recording and playing audio (press Enter to stop)...\n");
    getchar();  // Wait for user input

    err = Pa_StopStream(stream);
    if (err != paNoError) {
        fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(err));
        return 1;
    }

    err = Pa_CloseStream(stream);
    if (err != paNoError) {
        fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(err));
        return 1;
    }

    Pa_Terminate();

    return 0;
}
