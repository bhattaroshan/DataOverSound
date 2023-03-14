#include <vector>
#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>

using namespace std;

class Recorder
{
    SDL_AudioSpec want;

    SDL_Init(SDL_INIT_AUDIO);
    //check if initialization failed, less than zero is error

    SDL_memset(&want, 0, sizeof(want));
    // want.freq = 44100;

    want.freq = 44100;
    want.format = AUDIO_S16;
    want.channels = 1;
    want.samples = 1024;
    dev = SDL_OpenAudioDevice(SDL_GetAudioDeviceName(0, SDL_FALSE), 0, &want, NULL, 0);

private:
    /* data */
    int audioSource =
        public : Recorder(/* args */);
    ~Recorder();
};

Recorder::Recorder(/* args */)
{
}

Recorder::~Recorder()
{
}
