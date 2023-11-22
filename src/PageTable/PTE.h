//
// Created by Eason Pan on 2023-11-11.
//

#ifndef INC_5600_VVM_PTE_H
#define INC_5600_VVM_PTE_H
#include <iostream>


class PTE {
public:
    int PFN; //physical frame name
    bool validBit; // whether the translation is valid (allocated)
    bool protectionBit; // is accessible
    bool presentBit; //is in physical memory
    bool dirtyBit; //is modified
    bool referenceBit; //has been accessed

public:
    PTE(int PFN, bool validBit= false, bool protectionBit= false, bool presentBit= false, bool dirtyBit= false, bool referenceBit= false){
        this->PFN = PFN;
        this->validBit = validBit;
        this->protectionBit = protectionBit;
        this->presentBit = presentBit;
        this->dirtyBit = dirtyBit;
        this->referenceBit = referenceBit;
    }

    ~PTE()= default;

    void printEntry(){
        std::cout << "[PTE - PFN: " << this->PFN << " | validBit: " << (int)this->validBit << " | protectionBit: " << (int)this->protectionBit << " | presentBit: " << (int)this->presentBit << " | dirtyBit: " << (int)this->dirtyBit << " | referenceBit: " << (int)this->referenceBit << " ]\n";
    }

    void use(int PFN){
        this->PFN = PFN;
        this->validBit = true;
        this->presentBit = true;
    }

    void reset(){
        this->PFN = -1;
        this->validBit = false;
        this->protectionBit = false;
        this->presentBit = false;
        this->dirtyBit = false;
        this->referenceBit = false;
    }




};


#endif //INC_5600_VVM_PTE_H
