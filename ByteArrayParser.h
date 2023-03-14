
#define byte unsigned char

#include <vector>
using namespace std;

class ByteArrayParser {
   private:
    vector<byte> outputByteArray;
    vector<byte> concatenateTwoArrays(vector<byte>, vector<byte>);
    vector<vector<byte>> divideInto256Chunks(vector<byte>, int);
    void mergeArray(vector<byte>);
    vector<byte> getAndResetOutputByteArray();
};