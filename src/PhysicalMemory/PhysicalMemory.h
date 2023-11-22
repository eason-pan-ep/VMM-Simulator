//
// Created by Eason Pan on 2023-11-13.
//

#ifndef INC_5600_VVM_PHYSICALMEMORY_H
#define INC_5600_VVM_PHYSICALMEMORY_H

#include <climits>
#include <iostream>


class PhysicalMemory {
    int memorySize;
    int pageSize;
    int currentAt;
    std::vector<int> memory;

public:
    PhysicalMemory(int memorySize=67108864, int pageSize=4){
        if(memorySize > INT_MAX){
            throw std::invalid_argument("Given memory size exceeds INT_MAX, try something smaller\n");
        }
        if(pageSize > memorySize){
            throw std::invalid_argument("page size cannot be greater than memory size\n");
        }
        if(pageSize < 0 || memorySize < 0){
            throw std::invalid_argument("page size and memory size should both be positive integers\n");
        }
        this->memorySize = memorySize;
        this->pageSize = pageSize;
        this->currentAt = 0;
        this->initializeMemory();


    }

    ~PhysicalMemory(){
        this->memory.clear();
    }

public:
    /**
     * Find a chunk of memory
     * @return PFN, -1 if the memory is full
     */
    int findOneChunk(){
        if(this->currentAt > (this->memorySize / this->pageSize)){
            return -1;
        }
        int res = this->memory.at(this->currentAt);
        this->currentAt += 1;
        return res;
    }

private:
    void initializeMemory(){
        for(int i = 0; i < (this->memorySize / this->pageSize); i++){
            this->memory.at(i) = 4 * i;
        }
    }

};


#endif //INC_5600_VVM_PHYSICALMEMORY_H
