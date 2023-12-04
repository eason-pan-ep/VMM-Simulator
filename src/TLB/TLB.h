//
// Created by Eason Pan on 2023-11-13.
//

#ifndef INC_5600_VVM_TLB_H
#define INC_5600_VVM_TLB_H
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "TLBEntry.h"
#include "../PageTable/PTE.h"


class TLB {
private:
    std::unordered_map<int, PTE*> buffer; // VPN, PTE
    int size;
    int level;

public:
    TLB(int size = 64, int level = 1){
        if(size > 1024){
            throw std::invalid_argument("TLB size greater than 1024 is too ideal.\n");
        }
        this->size = size;
        this->level = level;
    }

    ~TLB(){
            this->buffer.clear();
    }

    /**
     * Add a new entry, if the TLB if full, replace an existing one using the selected policy
     * @param VPN
     * @param newPTE
     * @return opCount.
     */
    int addEntry(int VPN, PTE* newPTE, std::string mode = "random"){
        if(newPTE == nullptr){
            throw std::invalid_argument("Can't add null PTE to TLB\n");
        }
        if(this->buffer.size() == this->size){ //TLB is full
            return this->replace(VPN, newPTE);
        }
        this->buffer.insert({VPN, newPTE});
        return 1;
    }

    void printTLB(){
        std::cout << "TLB Info: \n";
        std::cout << "TLB Size: " << this->size << "\n";
        if(0 == this->buffer.size()){
            std::cout << "Empty TLB\n";
        }
        for(auto& pair : this->buffer){
            std::cout << "VPN: " << pair.first << " | PTE: ";
            pair.second->printEntry();
        }
    }

    /**
     * lookup for a specific entry based on the given VPN
     * @param VPN
     * @return opCount, if return -1 means a TLB miss
     */
    int lookup(int VPN){
        if(this->buffer.find(VPN) == this->buffer.end()){
            return -1; // TLB Miss
        }
        return 1; //TLB Hit
    }

    /**
     * Get the size of the TLB
     * @return
     */
    int getSize() const{
        return this->size;
    }

private:
    /**
     * Remove an entry
     * @param VPN
     * @return opCount
     */
    int replace(int VPN, PTE* newPTE, std::string mode = "random"){
        if(newPTE == nullptr){
            throw std::invalid_argument("newPTE is null [TLB->replace()]\n");
        }
        int opCount = 0;
        int removeVPN = -1;
        if(mode == "random"){
            removeVPN = this->randomPolicy();
            opCount += 1;
        }


        this->buffer.erase(removeVPN);
        opCount += 1;

        this->buffer.insert({VPN, newPTE});
        opCount += 1;

        return opCount;
    }

    /**
     * random replacement policy
     * @return the VPN of the selected entry to delete
     */
    int randomPolicy(){
        std::vector<int> keys;
        for(auto& pair : this->buffer){
            keys.push_back(pair.first);
        }
        std::srand(std::time(nullptr));
        int removeIndex = std::rand() % (keys.size() + 1);
        return keys.at(removeIndex);
    }

};


#endif //INC_5600_VVM_TLB_H
