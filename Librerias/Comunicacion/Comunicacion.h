#include <stddef.h>
#include <stdint.h>

#ifndef Comunicacion_h
#define Comunicacion_h

class Comunicacion{
    public:

        Comunicacion();
        void recive1();
        void send1(uint8_t info1 = 0, uint8_t info2 = 0, uint8_t info3 = 0, uint8_t info4 = 0, uint8_t info5 = 0, uint8_t info6 = 0, uint8_t info7 = 0, uint8_t info8 = 0);
        void request();
    
    private:

    int status = 0;
    long timeLimit = 833;
    const size_t dataLength1 = 8;
    const size_t dataLength2 = 8;
    const size_t dataLength3 = 8;
    const size_t dataLength4 = 8;
    const size_t dataLength5 = 8;
    uint8_t data1[8];
    uint8_t data2[8];
    uint8_t data3[8];
    uint8_t data4[8];
    uint8_t data5[8];
};
#endif