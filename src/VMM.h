//
// Created by Eason Pan on 2023-11-22.
//

#ifndef INC_5600_VVM_VMM_H
#define INC_5600_VVM_VMM_H

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include "./PhysicalMemory/PhysicalMemory.h"
#include "./PageTable/PageTable.h"
#include "./TLB/TLB.h"


class VMM {
private:
    const int MAX_CHUNK = 5;
    int pageTableSize;
    int pageSize;
    PageTable* pageTable;
    TLB* tlb;
    PhysicalMemory* physicalMemory;
    std::unordered_map<std::string, int> counters; // all different counters for outputs
    std::unordered_map<std::string, int> speedFactors; // how much a specific type of operation should multiply when calculating speed
    std::string TLBReplacePolicy; // the policy used to replace TLB entries
    std::string workloadType; // the type of workload

public:
    VMM(int memorySize=2048, int pageSize=4, int pageTableSize=2048, int TLBSize=64, int TLBTotalLevels=1, std::string TLBReplacePolicy="random", std::string workloadType="random"){
        this->physicalMemory = new PhysicalMemory(memorySize, pageSize);
        this->pageTable = new PageTable(pageTableSize, pageSize);
        this->tlb = new TLB(TLBSize, TLBTotalLevels);
        this->pageTableSize = pageTableSize;
        this->pageSize = pageSize;
        this->TLBReplacePolicy = TLBReplacePolicy;
        this->workloadType = workloadType;

        //initialize counters
        this->counters.insert({"TLB Hit", 0});
        this->counters.insert({"TLB Miss", 0});
        this->counters.insert({"Total Time", 0});
        this->counters.insert({"Memory Access Count", 0});
        this->counters.insert({"TLB Access Count", 0});
        this->counters.insert({"Page Fault Count", 0});
        this->counters.insert({"Total Access Count", 0});

        //initialize speed factors
        this->speedFactors.insert({"Level 1 TLB", 1});
        this->speedFactors.insert({"Level 2 TLB", 14});
        this->speedFactors.insert({"Level 3 TLB", 28});
        this->speedFactors.insert({"Memory", 200});

    }

    ~VMM(){
        delete(this->physicalMemory);
        delete(this->pageTable);
        delete(this->tlb);
        this->counters.clear();
        this->speedFactors.clear();
    }

public:
    void runSimulation(int totalRequets=10){
        std::vector<int> workload;
        //generate different type of workload based on input
        if("random" == this->workloadType){ // random workload
            workload = this->generateRandomWorkLoad(totalRequets);
        }
        this->printWorkLoad(workload);

        for(int VPN : workload){
            if(-1 != this->tlb->lookup(VPN)){ //if it's a TLB hit
                this->counters["TLB Hit"] += 1; //update TLB hit counter
                this->counters["TLB Access Count"] += 1; //update TLB access counter
                continue;
            }
            //if it's a TLB miss
            while(-1 == this->tlb->lookup(VPN)){
                this->counters["TLB Miss"] += 1; //update TLB miss counter
                if(nullptr == this->pageTable->walk(VPN)){ //if it's a page fault
                    this->counters["Page Fault Count"] += 1; //update page fault counter
                    this->counters["Memory Access Count"] += 1; //update memory access counter
                    int newPFN = this->translate();
                    if(-1 == newPFN){ //if the memory is full
                        std::cout << "Memory is full, cannot translate\n";
                        return;
                    }else{ //if the memory is not full
                        //create a new entry in page table
                        this->pageTable->addEntry(VPN, newPFN); //add the new entry to page table
                        this->counters["Memory Access Count"] += 1;
                        this->tlb->addEntry(VPN, this->pageTable->walk(VPN), this->TLBReplacePolicy); //add the new entry to TLB
                        this->counters["TLB Access Count"] += 1;
                    }

                }else{ //if it's not a page fault
                    this->counters["Memory Access Count"] += 1; //update memory access counter
                    this->tlb->addEntry(VPN, this->pageTable->walk(VPN), this->TLBReplacePolicy); //add the new entry to TLB
                    this->counters["TLB Access Count"] += 1;
                }
            }
        }

        //print out the result
        this->printResult();


    }


private:
    std::vector<int> generateRandomWorkLoad(int totalRequests){
        std::srand(std::time(nullptr)); //initialize a seed
        if(totalRequests < 0 ){
            throw std::invalid_argument("Total Request Count should be a positive integer\n");
        }
        std::vector<int> chunks;
        std::vector<int> workload;

        for(int i = 0; i < totalRequests; i++){
            //random pick number of chunks adding to workload
            chunks.push_back(std::rand() % (MAX_CHUNK + 1));
        }
        // randomly add accessing address to workload list
        for( int chunkCount : chunks){
            for(int i = 0; i < chunkCount; i++){
                workload.push_back(std::rand() % (this->pageTableSize + 1) / this->pageSize); //random address
                this->counters["Total Access Count"] += 1;
            }
        }
        return workload;
    }

    void printResult(){
        std::cout << "================== Result ==================\n";
        std::cout << "Total Access: " << std::dec << this->counters["Total Access Count"]  << " times" << "\n";
        std::cout << "Workload Type: " << this->workloadType << "\n";
        std::cout << "TLB Replacement Policy: " << this->TLBReplacePolicy << "\n";
        std::cout << "----------- TLB Hit Rate -----------\n";
        std::cout << "TLB Hit Count: " << this->counters["TLB Hit"] << "\n";
        std::cout << "TLB Miss Count: " << this->counters["TLB Miss"] << "\n";
        std::cout << "TLB Hit Rate: " << (double)this->counters["TLB Hit"] / (double)(this->counters["TLB Hit"] + this->counters["TLB Miss"]) << "\n";
        std::cout << "----------- Page Fault Rate -----------\n";
        std::cout << "Page Fault Count: " << this->counters["Page Fault Count"] << "\n";
        std::cout << "Page Fault Rate: " << (double)this->counters["Page Fault Count"] / (double)(this->counters["TLB Hit"] + this->counters["TLB Miss"]) << "\n";

        //calculate the total time based on access count and speed factors
        int TLBTime = 0;
        int memoryTime = 0;
        for(auto const& [key, val] : this->counters){
            if("TLB Access Count" == key){
                TLBTime += val * this->speedFactors["Level 1 TLB"];
            }else if("Memory Access Count" == key){
                memoryTime += val * this->speedFactors["Memory"];
            }
        }
        int totalTime = TLBTime + memoryTime;
        std::cout << "----------- Total Time -----------\n";
        std::cout << "Speed Factors -- " << "Memory Access Speed : TLB Speed = " << this->speedFactors["Memory"] << " : " << this->speedFactors["Level 1 TLB"] <<"\n";
        std::cout << "TLB Access Count: " << this->counters["TLB Access Count"] << "\n";
        std::cout << "Memory Access Count: " << this->counters["Memory Access Count"] << "\n";
        std::cout << "Total Time: " << totalTime << "\n";
        std::cout << "TLB Time: " << TLBTime << "\n";
        std::cout << "Memory Time: " << memoryTime << "\n";

        std::cout << "============================================\n";
    }

    void printWorkLoad(std::vector<int> workload){
        std::cout << "Workload (VPN to access): \n[ ";
        for (int work: workload)
            std::cout << std::hex << std::setw(8) << std::setfill('O') << work << ", ";

        std::cout << "]\n";
    }

    /**
     * Mimicing translation
     * @param VPN
     * @return PFN, if the memory is full return -1
     */
    int translate(){
        int PFN = this->physicalMemory->findOneChunk();
        if(-1 == PFN){
            return -1;
        }
        this->counters["Memory Access Count"] += 1;
        return PFN;
    }




};


#endif //INC_5600_VVM_VMM_H
