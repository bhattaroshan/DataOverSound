
#include "RecordTask.h"

SDL_AudioDeviceID playingDev, recordingDev;
SDL_AudioSpec speakerWant, microphoneWant, obtainedSpec;
vector<byte> microphoneData;

static void microphoneCallback(void *userdata, uint8_t *pcm, int len)
{
    if (SDL_GetAudioDeviceStatus(recordingDev) == SDL_AUDIO_PLAYING)
    {
        //SDL_QueueAudio(playingDev, pcm, len);
        for (int i = 0; i < len; ++i)
            microphoneData.push_back(pcm[i]);
    }
}

RecordTask::RecordTask()
{
    SDL_zero(obtainedSpec);

    SDL_Init(SDL_INIT_AUDIO);
    SDL_memset(&speakerWant, 0, sizeof(speakerWant));
    SDL_memset(&microphoneWant, 0, sizeof(microphoneWant));

    speakerWant.freq = 44100;
    speakerWant.format = AUDIO_S16;
    speakerWant.channels = 1;
    speakerWant.samples = 1024;
    playingDev = SDL_OpenAudioDevice(NULL, SDL_FALSE, &speakerWant, &obtainedSpec, 0);

    obtainedSpec.callback = microphoneCallback;
    recordingDev = SDL_OpenAudioDevice(NULL, SDL_TRUE, &obtainedSpec, &obtainedSpec, 0);
    SDL_PauseAudioDevice(recordingDev, SDL_TRUE);
    SDL_PauseAudioDevice(playingDev, SDL_TRUE);
}

void RecordTask::receiveData()
{
    int StartFrequency = 17500;
    int EndFrequency = 20000;
    int BitPerTone = 4;

    vector<ChunkElement> recordedArray;

    BitFrequencyConverter bitConverter = BitFrequencyConverter(StartFrequency, EndFrequency, BitPerTone);

    int HalfPadd = bitConverter.getPadding() / 2;
    int HandshakeStart = bitConverter.getHandshakeStartFreq();
    int HandshakeEnd = bitConverter.getHandshakeEndFreq();

    //recorder elements missing here......
    //BIG CHUNK MISSING HERE

    int listeningStarted = 0;
    int startCounter = 0;
    int endCounter = 0;
    int lastInfo = 2;
    vector<byte> namePartBArray;
    // cout << "I am here" << endl;

    while (work)
    {
        // cout << "i am running continuously" << endl;
        // ChunkElement tempElem;
        // while (recordedA.empty()) {
        //  SDL data from microphone
        //

        SDL_PauseAudioDevice(recordingDev, SDL_FALSE);
        while (1)
        {
            if (microphoneData.size() > 200)
                break;
            SDL_Delay(16);
        }
        SDL_PauseAudioDevice(recordingDev, SDL_TRUE);

        // SDL_PauseAudioDevice(recordingDev, SDL_FALSE);
        // while (microphoneData.empty()) {
        // SDL_Delay(16);
        // }

        // microphoneData.erase(microphoneData.begin());

        // cout << "successful till here" << endl;
        // double currNum = calculate(tempElem.getBuffer(), StartFrequency, EndFrequency, HalfPadd);
        double currNum = calculate(microphoneData, StartFrequency, EndFrequency, HalfPadd);
        microphoneData.clear();
        // cout << "max frequency = " << currNum << endl;
        if (listeningStarted == 0)
        {
            if ((currNum > (HandshakeStart - HalfPadd)) && (currNum < (HandshakeStart + HalfPadd)))
            {
                startCounter++;
                if (startCounter >= 2)
                {
                    listeningStarted = 1;
                    cout << "receiving information from sender" << endl;
                }
            }
            else
            {
                startCounter = 0;
            }
        }
        else
        {
            if ((currNum > (HandshakeStart - HalfPadd)) && (currNum < (HandshakeStart + HalfPadd)))
            {
                lastInfo = 2;
                endCounter = 0;
            }
            else
            {
                if (currNum > (HandshakeEnd - HalfPadd))
                {
                    endCounter++;
                    if (endCounter >= 2)
                    {
                        if (namePartBArray.empty())
                        {
                            vector<byte> cc = bitConverter.getAndResetReadBytes();
                            int len = cc.size();
                            cout << "the length is = " << len << endl;
                            for (int i = 0; i < len; ++i)
                                namePartBArray.push_back(cc[i]);
                            listeningStarted = 0;
                            startCounter = 0;
                            endCounter = 0;
                            cout << "information = ";
                            for (auto c : namePartBArray)
                                cout << c;
                            cout << endl;
                            // for (auto em : namePartBArray)
                            // {
                            //     result += (char)em;
                            // }
                            namePartBArray.clear();
                            // return result;
                        }
                        else
                        {
                            setWorkFalse();
                            // namePartBArray.clear();
                        }
                    }
                }
                else
                {

                    endCounter = 0;
                    if (lastInfo != 0)
                    {

                        lastInfo = 0;
                        bitConverter.calculateBits(currNum);
                    }
                }
            }
        }
    }
}

double RecordTask::calculate(vector<byte> buffer, int StartFrequecy, int EndFrequency, int HalfPad)
{
    int analyzedSize = 1024;
    vector<Complex> fftTempArray1(analyzedSize);
    int tempI = -1;

    /*
        DANGER HERE NO BOUND CHECKING, BUFFER IS CLEARLY OUT OF BOUND EVERY TIME
        SINCE C++ DOESN'T DO BOUND CHECKING THIS ONE SEEMS TO WORK FINE
    */

    for (int i = 0; i < analyzedSize * 2; i += 2)
    {
        short buff = buffer[i + 1];
        short buff2 = buffer[i];
        buff = (short)((buff & 0xff) << 8);
        buff2 = (short)(buff2 & 0xff);
        short tempShort = (short)(buff | buff2);
        tempI++;
        fftTempArray1[tempI] = Complex(tempShort, 0);
    }
    // FFT tfft;
    vector<Complex> fftArray1 = fft(fftTempArray1);
    // cout << "I could reach here too :)" << endl;
    int startIndex1 = ((StartFrequecy - HalfPad) * (analyzedSize)) / 44100;
    int endIndex1 = ((EndFrequency + HalfPad) * (analyzedSize)) / 44100;

    int max_index1 = startIndex1;
    double max_magnitude1 = (int)fftArray1[max_index1].abs();
    double tempMagnitude;

    for (int i = startIndex1; i < endIndex1; ++i)
    {
        tempMagnitude = fftArray1[i].abs();
        if (tempMagnitude > max_magnitude1)
        {
            max_magnitude1 = (int)tempMagnitude;
            max_index1 = i;
        }
    }
    return 44100 * max_index1 / (analyzedSize);
}

void RecordTask::setWorkFalse()
{
    this->work = false;
}