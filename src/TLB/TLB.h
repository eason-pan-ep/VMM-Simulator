//
// Created by Eason Pan on 2023-11-13.
//

#ifndef INC_5600_VVM_TLB_H
#define INC_5600_VVM_TLB_H
#include <iostream>
#include "TLBEntry.h"
#include "../PageTable/PTE.h"


class TLB {
private:
    std::unordered_map<int, PTE*> buffer;
    int size;
    int level;

public:
    TLB(int size = 128, int level = 1){
        if(size > 256){
            throw std::invalid_argument("TLB size greater than 256 is too ideal.\n");
        }
        this->size = size;
        this->level = level;
    }

    ~TLB() = default;

    /**
     * Add a new entry
     * @param VPN
     * @param newPTE
     * @return -1 if there is an error, otherwise 0.
     */
    int addEntry(int VPN, PTE* newPTE){
        if(newPTE == nullptr){
            throw std::invalid_argument("Can't add null PTE to TLB\n");
        }
        if(this->buffer.size() == this->size){
            return -1;
        }
        this->buffer.insert({VPN, newPTE});
        return 0;
    }

    int removeEntry(int VPN){
        return 0;
    }

};


#endif //INC_5600_VVM_TLB_H
