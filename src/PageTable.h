//
// Created by Eason Pan on 2023-11-13.
//

#ifndef INC_5600_VVM_PAGETABLE_H
#define INC_5600_VVM_PAGETABLE_H
#include <iostream>
#include "PTE.h"

/**
 * This class represents a VM Page Table.
 * using vector as its primary data structure.
 */
class PageTable {
private:
    std::vector<PTE*> pageTable;
    int tableSize;
    int pageSize;

public:
    PageTable(int tableSize, int pageSize=4){
        if(tableSize < pageSize){
            throw std::invalid_argument("tableSize should be greater than pageSize!\n");
        }
        if(tableSize % pageSize != 0){
            throw std::invalid_argument("tableSize should be dividable by pageSize(default=4)\n");
        }
        this->tableSize = tableSize;
        this->pageSize = pageSize;

    }

    ~PageTable(){
        for(PTE* entry : this->pageTable){
            delete(entry);
        }
        this->pageTable.clear();
    }

    /**
     * Add a new Page Table Entry
     * @param PFN
     * @return operation count
     */
    int addEntry(int PFN=-1){
        PTE* newEntry = new PTE(PFN);
        this->pageTable.push_back(newEntry);
        return 1;
    }


    /**
     * perform a page table walk
     * @param VPN the Virtual page name
     * @return
     */
    PTE* walk(int VPN){
        if(!this->isInTable(VPN)){
            return nullptr;
        }else{
            int index = this->convertVPNToIndex(VPN);
            if(index + 1 > this->pageTable.size()){
                return nullptr;
            }else if(this->pageTable.at(index)->validBit){
                return this->pageTable.at(index);
            }
        }
        return nullptr;
    }

    /**
     * update page at certain VPN to new PFN
     * PFN will be -1, if it's a de-allocation operation
     * @param VPN
     * @param PFN
     * @return -1 for error, 0 for success
     */
    int translate(int VPN, int PFN){
        if(!this->isInTable(VPN)){
            return -1;
        }
        int index = this->convertVPNToIndex(VPN);
        if(-1 == PFN){
            this->pageTable.at(index)->reset();
        }else{
            this->pageTable.at(index)->PFN = PFN;
            this->pageTable.at(index)->validBit = true;
            this->pageTable.at(index)->presentBit = true;
        }
        return 0;

    }

    void printPageTable(){
        if(0 == this->pageTable.size()){
            std::cout << "Empty Page Table\n";
        }

        std::cout << "Page Table: \n";
        for(int i = 0; i < this->pageTable.size(); i++){
            std::cout << "Index: " << i << " --> ";
            this->pageTable.at(i)->printEntry();
        }
    }

private:
    bool isInTable(int VPN){
        if(VPN > this->tableSize){
            return false;
        }
        return true;
    }

    int convertVPNToIndex(int VPN){
        return VPN / this->pageSize;
    }
};


#endif //INC_5600_VVM_PAGETABLE_H
