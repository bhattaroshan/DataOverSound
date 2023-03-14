#include "BufferSoundTask.h"

BufferSoundTask::BufferSoundTask() {
    SDL_AudioSpec want;

    SDL_Init(SDL_INIT_AUDIO);
    //check if initialization failed, less than zero is error

    SDL_memset(&want, 0, sizeof(want));
    want.freq = 44100;
    want.format = AUDIO_S16;
    want.channels = 1;
    want.samples = 1024;
    dev = SDL_OpenAudioDevice(NULL, 0, &want, NULL, 0);

    //check if device is zero this should be error

    // SDL_QueueAudio(dev, (void*)data, 1500 * sizeof(int));
    SDL_PauseAudioDevice(dev, SDL_TRUE);  //pause the device for now

    // while (SDL_GetQueuedAudioSize(dev) > 0) {
    //     SDL_Delay(1000);
    // }
}

void BufferSoundTask::playTone(double freqOfTone, double duration) {
    double dnumSamples = duration * sampleRate;
    dnumSamples = ceil(dnumSamples);
    int numSamples = (int)dnumSamples;
    vector<double> sample(numSamples);
    vector<byte> generatedSnd(2 * numSamples);
    double anglePadding = (freqOfTone * 2 * M_PI) / (sampleRate);
    double angleCurrent = 0;
    for (int i = 0; i < numSamples; ++i) {
        sample[i] = sin(angleCurrent);
        angleCurrent += anglePadding;
    }
    int idx = 0;
    int i = 0;
    int ramp = numSamples / 20;
    for (i = 0; i < ramp; ++i) {
        double dVal = sample[i];
        short val = (short)((dVal * 32767 * i / ramp));
        generatedSnd[idx++] = (byte)(val & 0x00ff);
        generatedSnd[idx++] = (byte)((val & 0xff00) >> 8);
    }
    for (i = i; i < numSamples - ramp; ++i) {
        double dVal = sample[i];
        short val = (short)((dVal * 32767));
        generatedSnd[idx++] = (byte)(val & 0x00ff);
        generatedSnd[idx++] = (byte)((val & 0xff00) >> 8);
    }
    for (i = i; i < numSamples; ++i) {
        double dVal = sample[i];
        short val = (short)((dVal * 32767 * (numSamples - i) / ramp));
        generatedSnd[idx++] = (byte)(val & 0x00ff);
        generatedSnd[idx++] = (byte)((val & 0xff00) >> 8);
    }

    //play the tone now
    // for (byte d : generatedSnd) playTone(d, durationSec);
    int sz = generatedSnd.size();
    uint8_t data[sz];
    for (int i = 0; i < sz; ++i) data[i] = generatedSnd[i];

    SDL_QueueAudio(dev, (void*)data, sizeof(data));
}

vector<byte> BufferSoundTask::convertStringToByteStream(string str) {
    vector<byte> res;
    int index = 0;
    while (str[index]) {
        res.push_back((byte)str[index]);
        ++index;
    }
    return res;
}

void BufferSoundTask::sendData(string str) {
    int startFreq = 17500;
    int endFreq = 20000;
    int bitsPerTone = 4;
    BitFrequencyConverter bitConverter = BitFrequencyConverter(startFreq, endFreq, bitsPerTone);
    vector<byte> encodedMessage = convertStringToByteStream(str);
    vector<int> freqs = bitConverter.calculateFrequency(encodedMessage);

    //something missing here,,,,,,

    playTone((double)bitConverter.getHandshakeStartFreq(), durationSec);
    playTone((double)bitConverter.getHandshakeStartFreq(), durationSec);

    for (int freq : freqs) {
        playTone((double)freq, durationSec / 2);
        playTone((double)bitConverter.getHandshakeStartFreq(), durationSec);
    }

    playTone((double)bitConverter.getHandshakeEndFreq(), durationSec);
    playTone((double)bitConverter.getHandshakeEndFreq(), durationSec);
}

void BufferSoundTask::magic() {
    SDL_PauseAudioDevice(dev, SDL_FALSE);  //pause the device for now
    while (SDL_GetQueuedAudioSize(dev) > 0) {
        SDL_Delay(1000);
    }
    SDL_PauseAudioDevice(dev, SDL_TRUE);  //just in case
}