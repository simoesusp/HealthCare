#include "bleLocalizador.h"
int main()
{
    while(1)
    {
        char msgString[1000];
        int qtdDevice;
        qtdDevice=receiveString(msgString);
        insertDatabase(msgString,qtdDevice);
        delay(1000);
    }
}

