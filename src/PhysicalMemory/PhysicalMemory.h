//
// Created by Eason Pan on 2023-11-13.
//

#ifndef INC_5600_VVM_PHYSICALMEMORY_H
#define INC_5600_VVM_PHYSICALMEMORY_H

#include <climits>
#include <iostream>


class PhysicalMemory {
private:
    int currentAt;
    int totalChunks;
    int pageSize;
    std::vector<int> memory;

public:
    PhysicalMemory(int memorySize=2048, int pageSize=4){
        if(memorySize > INT_MAX){
            throw std::invalid_argument("Given memory size exceeds INT_MAX, try something smaller\n");
        }
        if(pageSize > memorySize){
            throw std::invalid_argument("page size cannot be greater than memory size\n");
        }
        if(pageSize < 0 || memorySize < 0){
            throw std::invalid_argument("page size and memory size should both be positive integers\n");
        }
        if(memorySize % pageSize != 0){
            throw std::invalid_argument("memory size should be able to be divided by the page size\n");
        }
        this->currentAt = 0;
        this->totalChunks = memorySize / pageSize - 1;
        this->pageSize = pageSize;
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
        if(this->currentAt > this->totalChunks){ //check whether the memory is full
            return -1;
        }
        int res = this->currentAt * this->pageSize;
        this->memory.push_back(res);
        this->currentAt += 1;
        return res;
    }

    void printMemory(){
        std::cout << "Current Memory Status:\n";
        if(0 == this->currentAt){
            std::cout << "Empty Memory\n";
        }else{
            for(int i = 0; i < this->memory.size(); i++){
                std::cout << "Address: " << this->memory.at(i) << "\n";
            }
        }
        std::cout << "------------------\n";
        std::cout << "Current Pointer at: " << this->currentAt << "\n";
        std::cout << "Remains " << this->totalChunks - this->currentAt << " chunks\n";
    }

};


#endif //INC_5600_VVM_PHYSICALMEMORY_H
