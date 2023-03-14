
#include "ByteArrayParser.h"

vector<byte> ByteArrayParser::concatenateTwoArrays(vector<byte> array1, vector<byte> array2) {
    vector<byte> joinedArray = array1;
    for (byte b : array2) joinedArray.push_back(b);
    return joinedArray;
}

vector<vector<byte>> ByteArrayParser::divideInto256Chunks(vector<byte> inputArray, int errorDetBNum) {
    vector<vector<byte>> tempList;
    int startPos = 0;
    int endPos = 256 - errorDetBNum;
    int bytesLeft = inputArray.size();
    while ((bytesLeft + errorDetBNum) > 256) {
        vector<byte> tempArr;
        for (int i = startPos; i < endPos; i++) tempArr.push_back(inputArray[i]);
        tempList.push_back(tempArr);
        startPos = endPos;
        endPos = startPos + 256 - errorDetBNum;
        bytesLeft -= (256 - errorDetBNum);
    }
    vector<byte> tempArr;
    for (int i = startPos; i < inputArray.size(); i++) tempArr.push_back(inputArray[i]);
    tempList.push_back(tempArr);
    return tempList;
}

void ByteArrayParser::mergeArray(vector<byte> inputArray) {
    if (outputByteArray.empty()) {
        outputByteArray = inputArray;
    } else {
        outputByteArray = concatenateTwoArrays(outputByteArray, inputArray);
    }
}

vector<byte> ByteArrayParser::getAndResetOutputByteArray() {
    vector<byte> tempArr = outputByteArray;
    outputByteArray.clear();
    return tempArr;
}