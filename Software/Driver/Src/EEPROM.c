#include "EEPROM.h"
void IapIdle()
{
    IAP_CONTR = 0;                              //??IAP??
    IAP_CMD = 0;                                //???????
    IAP_TRIG = 0;                               //???????
    IAP_ADDRH = 0x80;                           //???????IAP??
    IAP_ADDRL = 0;
}

char IapRead(int addr)
{
    char dat;

    IAP_CONTR = WT_24M;                         //??IAP
    IAP_CMD = 1;                                //??IAP???
    IAP_ADDRL = addr;                           //??IAP???
    IAP_ADDRH = addr >> 8;                      //??IAP???
    IAP_TRIG = 0x5a;                            //?????(0x5a)
    IAP_TRIG = 0xa5;                            //?????(0xa5)
    _nop_();
    dat = IAP_DATA;                             //?IAP??
    IapIdle();                                  //??IAP??

    return dat;
}

void IapProgram(int addr, char dat)
{
    IAP_CONTR = WT_24M;                         //??IAP
    IAP_CMD = 2;                                //??IAP???
    IAP_ADDRL = addr;                           //??IAP???
    IAP_ADDRH = addr >> 8;                      //??IAP???
    IAP_DATA = dat;                             //?IAP??
    IAP_TRIG = 0x5a;                            //?????(0x5a)
    IAP_TRIG = 0xa5;                            //?????(0xa5)
    _nop_();
    IapIdle();                                  //??IAP??
}

void IapErase(int addr)
{
    IAP_CONTR = WT_24M;                         //??IAP
    IAP_CMD = 3;                                //??IAP????
    IAP_ADDRL = addr;                           //??IAP???
    IAP_ADDRH = addr >> 8;                      //??IAP???
    IAP_TRIG = 0x5a;                            //?????(0x5a)
    IAP_TRIG = 0xa5;                            //?????(0xa5)
    _nop_();                                    //
    IapIdle();                                  //??IAP??
}