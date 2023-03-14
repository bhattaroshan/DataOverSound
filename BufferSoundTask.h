
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "BitFrequencyConverter.h"

using namespace std;

#define byte unsigned char

class BufferSoundTask {
   private:
    SDL_AudioDeviceID dev;
    double durationSec = 0.270;
    int sampleRate = 44100;
    vector<byte> message;

   public:
    BufferSoundTask();
    void playTone(double, double);
    void sendData(string str);
    vector<byte> convertStringToByteStream(string);
    void magic();
};