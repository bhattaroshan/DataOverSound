
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <stdio.h>
#include <string.h>

#include <cmath>
#include <cstdio>
#include <iostream>
#include <vector>

#include "BufferSoundTask.h"
#include "RecordTask.h"
#include "sdlLayer.h" //sdlLayer
#include "reed-solomon/rs.hpp"

using namespace std;

#define SEND_DATA 0
#define MSG "test data"

#define ECC_LENGTH 4
RS::ReedSolomon *rsData = nullptr;

int main(int argc, char *argv[])
{
    string a = argv[1];
    string b = argv[2];

    const int msglen = b.length();

    char encoded[msglen + ECC_LENGTH];
    rsData = new RS::ReedSolomon(msglen, ECC_LENGTH);

    cout << a << endl;
    if (a == "s")
    {
        BufferSoundTask bst;
        rsData->Encode(b.c_str(), encoded);
        bst.sendData(b);
        bst.magic();
    }
    else
    {

        RecordTask rt;
        rt.receiveData();
        SDL_Delay(1000);
    }

    return 0;
}
