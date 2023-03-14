#ifndef BITFREQUENCYCONVERTER_H
#define BITFREQUENCYCONVERTER_H

#include <vector>

#include "sdlLayer.h"
using namespace std;

#define byte unsigned char

class BitFrequencyConverter {
   private:
    int numberOfBitsInOneTone;
    int startFrequency;
    int endFrequency;
    int padding;
    int handshakeStartFreq;
    int handshakeEndFreq;
    vector<byte> readBytes;
    char currByte;
    int currShift;

   public:
    BitFrequencyConverter(int, int, int);
    void calculateBits(double);
    vector<byte> getAndResetReadBytes();
    int specificFrequency(byte);
    int getBit(byte, int);
    vector<int> calculateFrequency(vector<byte>);
    int getPadding();
    int getHandshakeStartFreq();
    int getHandshakeEndFreq();
};

#endif