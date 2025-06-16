#include <iostream>
#include "portaudio.h"

#define FRAMES_PER_BUFFER   512
#define SAMPLE_RATE         44100
#define NUM_CHANNELS        2
#define PA_SAMPLE_TYPE      paFloat32

typedef float PA_SAMPLE;

static int patestCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{
    const PA_SAMPLE *in = (const PA_SAMPLE*)inputBuffer;
    PA_SAMPLE *out = (PA_SAMPLE*)outputBuffer;

    if (inputBuffer == NULL) {
        for (unsigned int i = 0; i < framesPerBuffer * NUM_CHANNELS; i++) {
            *out++ = 0;
        }
    } else {
        for (unsigned int i = 0; i < framesPerBuffer * NUM_CHANNELS; i++) {
            *out++ = *in++;
        }
    }

    return paContinue;
}

int main(void)
{
    PaStreamParameters inputParameters, outputParameters;
    PaStream *stream;
    PaError err;

    std::cout << "PortAudio テスト: マイク入力をそのまま出力します。" << std::endl;

    err = Pa_Initialize();
    if (err != paNoError) goto error;

    inputParameters.device = Pa_GetDefaultInputDevice();
    if (inputParameters.device == paNoDevice) {
        std::cerr << "エラー: デフォルトの入力デバイスが見つかりません。" << std::endl;
        goto error;
    }
    inputParameters.channelCount = NUM_CHANNELS;
    inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    outputParameters.device = Pa_GetDefaultOutputDevice();
    if (outputParameters.device == paNoDevice) {
        std::cerr << "エラー: デフォルトの出力デバイスが見つかりません。" << std::endl;
        goto error;
    }
    outputParameters.channelCount = NUM_CHANNELS;
    outputParameters.sampleFormat = PA_SAMPLE_TYPE;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    err = Pa_OpenStream(
              &stream,
              &inputParameters,
              &outputParameters,
              SAMPLE_RATE,
              FRAMES_PER_BUFFER,
              paClipOff,
              patestCallback,
              NULL );
    if (err != paNoError) goto error;

    err = Pa_StartStream( stream );
    if (err != paNoError) goto error;

    std::cout << "マイク入力の出力を開始しました。何か話してみてください。" << std::endl;
    std::cout << "停止するには Enter キーを押してください。" << std::endl;
    getchar();

    err = Pa_StopStream( stream );
    if (err != paNoError) goto error;

    err = Pa_CloseStream( stream );
    if (err != paNoError) goto error;

    Pa_Terminate();
    std::cout << "プログラムを終了しました。" << std::endl;

    return 0;

error:
    Pa_Terminate();
    std::cerr << "PortAudio エラー: " << Pa_GetErrorText(err) << std::endl;
    return 1;
}