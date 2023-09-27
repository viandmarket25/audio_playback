#include <stdio.h>
#include <stdlib.h>
#include <portaudio.h>
#include <speex/speex_echo.h>
#include <speex/speex_preprocess.h>

#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 256

SpeexEchoState *echoState;
SpeexPreprocessState *preprocessState;

static int audioCallback(const void *inputBuffer, void *outputBuffer,
                        unsigned long framesPerBuffer,
                        const PaStreamCallbackTimeInfo *timeInfo,
                        PaStreamCallbackFlags statusFlags,
                        void *userData) {
    (void)timeInfo;
    (void)statusFlags;
    (void)userData;

    const int16_t *in = (const int16_t *)inputBuffer;
    int16_t *out = (int16_t *)outputBuffer;

    for (unsigned int i = 0; i < framesPerBuffer; i++) {
        // Read audio input from the microphone
        int16_t mic_input = in[i];

        // Apply echo cancellation
        speex_echo_cancellation(echoState, &mic_input, NULL, &mic_input);

        // Apply pre-processing (noise cancellation)
        speex_preprocess_run(preprocessState, &mic_input);

        // Write processed audio to the output buffer
        out[i] = mic_input;
    }

    return paContinue;
}

int main() {
    PaError err;
    PaStream *stream = NULL;
    int sampleRate = SAMPLE_RATE; // Store the sample rate in an integer variable

    err = Pa_Initialize();
    if (err != paNoError) {
        fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(err));
        return 1;
    }

    echoState = speex_echo_state_init(FRAMES_PER_BUFFER, FRAMES_PER_BUFFER * 10);
    speex_echo_ctl(echoState, SPEEX_ECHO_SET_SAMPLING_RATE, &sampleRate); // Pass the address of the integer variable

    preprocessState = speex_preprocess_state_init(FRAMES_PER_BUFFER, SAMPLE_RATE);

    err = Pa_OpenDefaultStream(&stream, 1, 1, paInt16, SAMPLE_RATE, FRAMES_PER_BUFFER, audioCallback, NULL);
    if (err != paNoError) {
        fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(err));
        return 1;
    }

    err = Pa_StartStream(stream);
    if (err != paNoError) {
        fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(err));
        return 1;
    }

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

    speex_echo_state_destroy(echoState);
    speex_preprocess_state_destroy(preprocessState);

    Pa_Terminate();

    return 0;
}
