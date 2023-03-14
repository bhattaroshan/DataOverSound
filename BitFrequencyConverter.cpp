
#include "BitFrequencyConverter.h"

#include <cmath>

BitFrequencyConverter::BitFrequencyConverter(int startFrequency, int endFrequency, int numberOfBitsInOneTone) {
    this->numberOfBitsInOneTone = numberOfBitsInOneTone;
    this->padding = (endFrequency - startFrequency) / (4 + (int)pow(2, numberOfBitsInOneTone));
    this->handshakeEndFreq = endFrequency;
    this->handshakeStartFreq = endFrequency - this->padding;
    this->startFrequency = startFrequency;
    this->endFrequency = endFrequency - 2 * this->padding;
    currByte = 0x00;
    currShift = 0;
}

void BitFrequencyConverter::calculateBits(double frequency) {
    byte resultBytes = 0x00;
    bool freqFound = false;
    bool lastPart = false;
    int counter = 0;

    for (int i = (startFrequency); i <= (endFrequency); i += padding, counter++) {
        if (frequency >= (i - (padding / 2)) && (frequency <= (i + (padding / 2)))) {
            if (counter == 0 || counter == 1) {
                lastPart = true;
            } else {
                freqFound = true;
            }
            break;
        } else {
            if (counter != 0 && counter != 1) {
                resultBytes += 0x01;
            }
        }
    }
    if (freqFound) {
        int tempCounter = numberOfBitsInOneTone;
        while (tempCounter > 0) {
            byte mask = 0x01;
            mask <<= (tempCounter - 1);
            currByte <<= 1;
            if ((mask & resultBytes) != 0x00) {
                currByte += 0x01;
            }
            currShift++;
            if (currShift == 8) {
                readBytes.push_back(currByte);
                currShift = 0;
                currByte = 0x00;
            }
            tempCounter--;
        }
    } else {
        if (lastPart) {
            currByte <<= 1;
            if (counter == 1) {
                currByte += 0x01;
            }
            currShift++;
            if (currShift == 8) {
                readBytes.push_back(currByte);
                currByte = 0x00;
                currShift = 0;
            }
        }
    }
}

vector<byte> BitFrequencyConverter::getAndResetReadBytes() {
    vector<byte> retArr;
    // if (currShift != 0) {
    // retArr.reserve(readBytes.size() + 1);
    // retArr = new byte[readBytes.size() + 1];
    // retArr[readBytes.size()] = currByte;
    // } else {
    // retArr = new byte[readBytes.size()];
    // retArr.reserve(readBytes.size());
    // }

    int i = 0;
    for (byte tempB : readBytes) {
        retArr.push_back(tempB);
        // retArr[i] = tempB;
        // i++;
    }
    if (currShift != 0) retArr.push_back(currByte);

    readBytes.clear();
    currByte = 0x00;
    currShift = 0;
    return retArr;
}

int BitFrequencyConverter::specificFrequency(byte sample) {
    int freq = startFrequency + padding * 2;
    int numberOfFreq = (int)pow(2, numberOfBitsInOneTone);
    byte tempByte = 0x00;
    for (int i = 0; i < numberOfFreq; i++) {
        if (tempByte == sample) {
            break;
        }
        tempByte += 0x01;
        freq += padding;
    }
    return freq;
}

int BitFrequencyConverter::getBit(byte check, int position) {
    return (check >> position) & 1;
}

vector<int> BitFrequencyConverter::calculateFrequency(vector<byte> byteArray) {
    vector<int> resultList;
    bool isDataModulo = (byteArray.size() * 8 % numberOfBitsInOneTone) == 0;
    byte currByte = 0x00;
    int currShift = 0;

    for (int i = 0; i < byteArray.size(); i++) {
        byte tempByte = byteArray[i];
        for (int j = 7; j >= 0; j--) {
            if (((currShift + j + 1 + (byteArray.size() - (i + 1)) * 8) < numberOfBitsInOneTone) && (!isDataModulo)) {
                int temp = getBit(tempByte, j);
                if (temp == 1) {
                    resultList.push_back(startFrequency + padding);
                } else {
                    resultList.push_back(startFrequency);
                }
                continue;
            }
            int temp = getBit(tempByte, j);
            currByte <<= 1;
            if (temp == 1) {
                currByte += 0x01;
            }
            currShift++;
            if (currShift == numberOfBitsInOneTone) {
                int currFreq = specificFrequency(currByte);
                resultList.push_back(currFreq);
                currByte = 0x00;
                currShift = 0;
            }
        }
    }
    return resultList;
}

int BitFrequencyConverter::getPadding() {
    return padding;
}

int BitFrequencyConverter::getHandshakeStartFreq() {
    return handshakeStartFreq;
}

int BitFrequencyConverter::getHandshakeEndFreq() {
    return handshakeEndFreq;
}