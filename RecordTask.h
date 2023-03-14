
#ifndef RECORDTASK_H
#define RECORDTASK_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>

#include <iostream>
#include <vector>

#include "BitFrequencyConverter.h"
// #include "SDL2/include/SDL.h"
// #include "SDL2/include/SDL_audio.h"
#include "complex.h"
#include "fft.h"

using namespace std;

#define byte unsigned char

class ChunkElement {
   private:
    vector<byte> buffer;

   public:
    ChunkElement(){};  //nothing in Constructor for now
    ChunkElement(vector<byte> buffer) { this->buffer = buffer; }
    vector<byte> getBuffer() { return buffer; }
    void setBuffer(vector<byte> buffer) { this->buffer = buffer; }
};

class RecordTask {
   private:
    bool work = true;

   public:
    RecordTask();
    void receiveData();
    double calculate(vector<byte>, int, int, int);
    void setWorkFalse();
};

#endif