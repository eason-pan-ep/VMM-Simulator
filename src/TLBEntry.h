//
// Created by Eason Pan on 2023-11-13.
//

#ifndef INC_5600_VVM_TLBENTRY_H
#define INC_5600_VVM_TLBENTRY_H
#include <iostream>
#include "PTE.h"


class TLBEntry : public PTE{
public:
    int VPN;
    int ASID;
public:
    TLBEntry(int VPN, int PFN, int ASID, bool validBit, bool protectionBit, bool presentBit, bool dirtyBit, bool referenceBit)
    : PTE(PFN, validBit, protectionBit, presentBit, dirtyBit, referenceBit)
    {
        this->VPN = VPN;
        this->ASID = ASID;
    };
    ~TLBEntry()= default;

    void printEntry(){
        std::cout << "[TLB Entry - VPN" << this->VPN << " | PFN: " << this->PFN << " | ASID: " << this->ASID << " | validBit: " << (int)this->validBit << " | protectionBit: " << (int)this->protectionBit << " | presentBit: " << (int)this->presentBit << " | dirtyBit: " << (int)this->dirtyBit << " | referenceBit: " << (int)this->referenceBit << " ]";
    }
};


#endif //INC_5600_VVM_TLBENTRY_H
