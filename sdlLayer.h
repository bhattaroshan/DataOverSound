
#ifndef SDLLAYER_H
#define SDLLAYER_H

// #define SDL_MAIN_HANDLED

const int AMPLITUDE = 28000;
const int FREQUENCY = 44100;

#include <cmath>
#include <queue>

// #include "SDL2/include/SDL.h"
// #include "SDL2/include/SDL_audio.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>

struct BeepObject {
    double freq;
    int samplesLeft;
};

class Beeper {
   private:
    double v;
    std::queue<BeepObject> beeps;

   public:
    Beeper();
    ~Beeper();
    void beep(double freq, int duration);
    void generateSamples(Sint16 *stream, int length);
    void wait();
};

#endif
