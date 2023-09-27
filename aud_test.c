#include <stdio.h>
#include <stdlib.h>
#include <portaudio.h>
#include <speex/speex_preprocess.h>

// Constants for audio configuration
#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 256

// Define the frequency bands and gain controls (for frequency selection)
typedef struct {
    float centerFrequency;
    float gain;
} FrequencyBand;

typedef struct {
    FrequencyBand bands[5]; // Assuming 5 bands for simplicity
} FilterSet;

// Define filter sets for different listening modes
static FilterSet indoorsFilterSet = {
    {{250.0, 1.0}, {500.0, 1.0}, {1000.0, 1.0}, {2000.0, 1.0}, {4000.0, 1.0}}
};

static FilterSet outdoorsFilterSet = {
    {{250.0, 0.5}, {500.0, 0.5}, {1000.0, 0.5}, {2000.0, 0.5}, {4000.0, 0.5}}
};

static FilterSet partyFilterSet = {
    {{250.0, 1.5}, {500.0, 1.5}, {1000.0, 1.5}, {2000.0, 1.5}, {4000.0, 1.5}}
};

// Define noise suppression settings for different modes
typedef struct {
    int noiseCancellationEnabled;
    float noiseSuppressionLevel;
} NoiseSuppressionSettings;

static NoiseSuppressionSettings indoorsNoiseSettings = {1, 0.5}; // Enable noise cancellation with moderate level
static NoiseSuppressionSettings outdoorsNoiseSettings = {0, 0.0}; // Disable noise cancellation
static NoiseSuppressionSettings partyNoiseSettings = {1, 0.8}; // Enable noise cancellation with high level

// Global variables to hold the current filter set and noise suppression settings
static FilterSet currentFilterSet = indoorsFilterSet; // Default to indoors mode
static NoiseSuppressionSettings currentNoiseSettings = indoorsNoiseSettings; // Default to indoors noise settings

// Define SpeexDSP preprocessing state
static SpeexPreprocessState *preprocess_state;

// Define audio playback variables
PaStream *outputStream;
float playbackBuffer[FRAMES_PER_BUFFER * 2]; // Stereo audio buffer

// Initialize PortAudio and set up the audio stream
PaError initializePortAudio() {
    PaError err;
    err = Pa_Initialize();
    if (err != paNoError) {
        fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(err));
        return err;
    }

    PaStream *stream;
    err = Pa_OpenDefaultStream(&stream, 1, 2, paFloat32, SAMPLE_RATE, FRAMES_PER_BUFFER, audioCallback, NULL);
    if (err != paNoError) {
        fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(err));
        return err;
    }

    err = Pa_StartStream(stream);
    if (err != paNoError) {
        fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(err));
        return err;
    }

    return paNoError;
}

// Audio callback function
static int audioCallback(const void *inputBuffer, void *outputBuffer,
                        unsigned long framesPerBuffer,
                        const PaStreamCallbackTimeInfo *timeInfo,
                        PaStreamCallbackFlags statusFlags,
                        void *userData) {
    (void) timeInfo; /* Prevent unused variable warnings */
    (void) statusFlags;

    float *out = (float *) outputBuffer;
    float *in = (float *) inputBuffer;
    unsigned int i;

    // Apply the selected filter set and noise suppression settings to the audio
    for (i = 0; i < framesPerBuffer; i++) {
        float leftSample = *in++;
        float rightSample = *in++;

        // Apply filters and gain adjustments based on the currentFilterSet
        for (int band = 0; band < 5; band++) { // Assuming 5 bands
            // Implement bandpass filter using your audio processing library (e.g., SpeexDSP)
            // Apply gain adjustment using currentFilterSet.bands[band].gain
        }

        // Apply noise suppression based on the currentNoiseSettings
        if (currentNoiseSettings.noiseCancellationEnabled) {
            // Apply noise suppression with the specified level using SpeexDSP
            speex_preprocess_run(preprocess_state, &leftSample, &rightSample);
        }

        // Send the processed audio to the audio playback buffer
        playbackBuffer[i * 2] = leftSample;
        playbackBuffer[i * 2 + 1] = rightSample;
    }

    // Send the processed audio to the audio playback system
    Pa_WriteStream(outputStream, playbackBuffer, framesPerBuffer);

    return paContinue;
}

int main() {
    // Initialize PortAudio and set up the audio stream
    PaError err = initializePortAudio();
    if (err != paNoError) {
        return 1;
    }

    // Initialize SpeexDSP preprocessing state
    preprocess_state = speex_preprocess_state_init(FRAMES_PER_BUFFER, SAMPLE_RATE);
    if (preprocess_state == NULL) {
        fprintf(stderr, "Failed to initialize SpeexDSP preprocessing state\n");
        return 1;
    }

    // Initialize audio playback using PortAudio
    err = Pa_OpenDefaultStream(&outputStream, 0, 2, paFloat32, SAMPLE_RATE, FRAMES_PER_BUFFER, NULL, NULL);
    if (err != paNoError) {
        fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(err));
        return 1;
    }

    err = Pa_StartStream(outputStream);
    if (err != paNoError) {
        fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(err));
        return 1;
    }

    printf("Audio Processing App\n");
    printf("Press Enter to stop...\n");

    char mode;
    while (1) {
        printf("Select Mode (I: Indoors, O: Outdoors, P: Party, Q: Quit): ");
        scanf(" %c", &mode);

        if (mode == 'I' || mode == 'i') {
            currentFilterSet = indoorsFilterSet;
            currentNoiseSettings = indoorsNoiseSettings;
        } else if (mode == 'O' || mode == 'o') {
            currentFilterSet = outdoorsFilterSet;
            currentNoiseSettings = outdoorsNoiseSettings;
        } else if (mode == 'P' || mode == 'p') {
            currentFilterSet = partyFilterSet;
            currentNoiseSettings = partyNoiseSettings;
        } else if (mode == 'Q' || mode == 'q') {
            break;
        } else {
            printf("Invalid mode selection. Please try again.\n");
        }
    }

    // Stop and clean up the PortAudio streams and resources
    Pa_StopStream(outputStream);
    Pa_CloseStream(outputStream);
    Pa_Terminate();

    speex_preprocess_state_destroy(preprocess_state);

    return 0;
}
