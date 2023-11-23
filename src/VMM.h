//
// Created by Eason Pan on 2023-11-22.
//

#ifndef INC_5600_VVM_VMM_H
#define INC_5600_VVM_VMM_H

#include <iostream>
#include "./PhysicalMemory/PhysicalMemory.h"
#include "./PageTable/PageTable.h"
#include "./TLB/TLB.h"


class VMM {
private:
    PageTable* pageTable;
    TLB* tlb;
    PhysicalMemory* physicalMemory;
    std::vector<int> validAddresses;
    std::unordered_map<std::string, int> counters; // all different counters for outputs
    std::unordered_map<std::string, int> speedFactors; // how much a specific type of operation should multiply when calculating speed

public:
    VMM(int memorySize=2048, int pageSize=4, int pageTableSize=2048, int TLBSize=64, int TLBTotalLevels=1, std::string TLBReplacePolicy="random" ){

    }

};


#endif //INC_5600_VVM_VMM_H
