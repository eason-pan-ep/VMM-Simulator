//
// Created by Eason Pan on 2023-11-13.
//

#ifndef INC_5600_VVM_TLB_H
#define INC_5600_VVM_TLB_H
#include <iostream>
#include <random>
#include "../PageTable/PTE.h"


class TLB {
private:
    std::unordered_map<int, int> usageTracking; // VPN, lruTimer
    std::unordered_map<int, PTE*> buffer; // VPN, PTE
    int size;
    int lruTimer;
    int level;

public:
    TLB(int size = 64, int level = 1){
        if(size > 1024){
            throw std::invalid_argument("TLB size greater than 1024 is too ideal.\n");
        }
        this->size = size;
        this->level = level;
        this->lruTimer = 0;
    }

    ~TLB(){
            this->buffer.clear();
    }

    /**
     * Add a new entry, if the TLB if full, replace an existing one using the selected policy
     * @param VPN
     * @param newPTE
     * @return 1 if success
     */
    int addEntry(int VPN, PTE* newPTE, std::string mode = "random"){
        if(newPTE == nullptr){
            throw std::invalid_argument("Can't add null PTE to TLB\n");
        }

        if(this->buffer.size() == this->size){ //TLB is full
            this->replace(VPN, newPTE, mode);
        }else{
            this->buffer.insert({VPN, newPTE});
        }
        return 1;
    }

    /**
     * Print the TLB
     */
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
        }else{
            this->updateLRUEntry(VPN);
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
     * @return 1 if success
     */
    int replace(int VPN, PTE* newPTE, std::string mode = "random"){
        int removeVPN = -1;
        if(mode == "random"){
            removeVPN = this->randomPolicy();
        }else if(mode == "lru"){
            removeVPN = this->lruPolicy();
        }else if(mode == "lfu"){

        }else{
            throw std::invalid_argument("Invalid replacement policy\n");
        }

        this->buffer.erase(removeVPN);
        this->deleteLRUEntry(removeVPN);
        this->buffer.insert({VPN, newPTE});

        return 1;
    }


    /**
     * Update the LRU entry in the usageTracking
     * @param VPN
     */
    void updateLRUEntry(int VPN){
        if(this->usageTracking.find(VPN) == this->usageTracking.end()) {
            this->usageTracking.insert({VPN, this->lruTimer});
        }else{
            this->usageTracking[VPN] = this->lruTimer;
        }
        this->lruTimer += 1;
    }


    /**
     * Delete the LRU entry in the usageTracking
     * @param VPN
     */
    void deleteLRUEntry(int VPN){
        this->usageTracking.erase(VPN);
    }



    // ==================== Replacement/Flushing Policies ====================


    /**
     * random replacement policy
     * @return the VPN of the selected entry to delete
     */
    int randomPolicy(){
        std::vector<int> keys;
        for(auto& pair : this->buffer){
            keys.push_back(pair.first);
        }
        //initialize a random seed
        auto seed = std::chrono::high_resolution_clock ::now().time_since_epoch().count();
        std::mt19937 generator(seed);
        std::uniform_int_distribution<int> randomIndex(0, keys.size() - 1);
        int removeIndex = randomIndex(generator);
        return keys.at(removeIndex);
    }


    /**
     * Least Recently Used replacement policy
     * @return the VPN of the selected entry to delete
     */
    int lruPolicy(){
        std::pair<int, int> earliest = {-1, INT_MAX}; //<VPN, lruTimer>
        for(auto pair : this->usageTracking){
            if(pair.second < earliest.second){
                earliest = pair;
            }
        }
        return earliest.first;
    }



    /**
     * Least Frequently Used replacement policy
     * @return the VPN of the selected entry to delete
     */
    int lfuPolicy(){
        return 0;
    }
};


#endif //INC_5600_VVM_TLB_H
