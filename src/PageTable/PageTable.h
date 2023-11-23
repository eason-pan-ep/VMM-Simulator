//
// Created by Eason Pan on 2023-11-13.
//

#ifndef INC_5600_VVM_PAGETABLE_H
#define INC_5600_VVM_PAGETABLE_H
#include <iostream>
#include <sstream>
#include "PTE.h"
#include "../PhysicalMemory/PhysicalMemory.h"

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
    PageTable(int tableSize=2048, int pageSize=4){
        if(tableSize < pageSize){
            throw std::invalid_argument("tableSize should be greater than pageSize!\n");
        }
        if(tableSize % pageSize != 0){
            throw std::invalid_argument("tableSize should be dividable by pageSize(default=4)\n");
        }
        this->tableSize = tableSize;
        this->pageSize = pageSize;
        this->initializePageTable();
    }

    ~PageTable(){
        for(PTE* entry : this->pageTable){
            delete(entry);
        }
        this->pageTable.clear();
    }

private:
    void initializePageTable(){
        int chunks = this->tableSize / this->pageSize;
        for(int i = 0; i < chunks; i++){
            PTE* newPTE = new PTE(-1);
            this->pageTable.push_back(newPTE);
        }
    }

public:
    /**
     * Add a new Page Table Entry
     * @return (VPN, opCount)
     */
    std::pair<int, int> addEntry(int PFN){
        std::pair<int, int> output;
        output.first = -1;
        output.second = 0;
        for(int i = 0; i < this->pageTable.size(); i++){
            output.second += 1;
            if(-1 == this->pageTable.at(i)->PFN){
                this->pageTable.at(i)->use(PFN);
                output.first = i;
                break;
            }
        }
        return output;
    }


    /**
     * Reset the entry at given VPN
     * @param VPN
     * @return -1 if failed, otherwise 1;
     */
    int resetEntry(int VPN){
        if(!this->isInTable(VPN)){
            return -1;
        }
        if(!this->pageTable.at(VPN)->validBit){
            return -1;
        }
        this->pageTable.at(VPN)->reset();
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

    void printPageTable(){
        if(0 == this->pageTable.size()){
            std::cout << "Empty Page Table\n";
        }

        std::cout << "Page Table: \n";
        for(int i = 0; i < this->pageTable.size(); i++){
            std::cout << "VPN: " << i <<  " --> ";
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
