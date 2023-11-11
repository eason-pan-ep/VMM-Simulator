//
// Created by Eason Pan on 2023-11-11.
//

#ifndef INC_5600_VVM_PTE_H
#define INC_5600_VVM_PTE_H


class PTE {
private:
    int PFN; //physical frame name
    bool validBit; // whether the translation is valid (allocated_
    bool protectionBit; // is accessible
    bool presentBit; //is in physical memory
    bool dirtyBit; //is modified
    bool referenceBit; //has been accessed

public:
    PTE(int PFN){
        this->PFN = PFN;
        this->validBit = false;
        this->protectionBit = false;
        this->presentBit = false;
        this->dirtyBit = false;
        this->referenceBit = false;
    }

    ~PTE()= default;
};


#endif //INC_5600_VVM_PTE_H
