// #Galois Fields
#include <iostream>
class GenericGF
{
private:
    /* data */
public:
    int primitive;
    int size;
    GenericGF();

    GenericGF(int primitive, int size)
    {
        this->primitive = primitive;
        this->size = size;
    }
    GenericGF GenericGF::AZTEC_DATA_12()
    {
        primitive = 0x1069;
        size = 4096;
    }
    ~GenericGF();
};

GenericGF::GenericGF(int primitive, int size)
{
}

GenericGF::~GenericGF()
{
}
