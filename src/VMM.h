//
// Created by Eason Pan on 2023-11-22.
//

#ifndef INC_5600_VVM_VMM_H
#define INC_5600_VVM_VMM_H

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <random>
#include "./PhysicalMemory/PhysicalMemory.h"
#include "./PageTable/PageTable.h"
#include "./TLB/TLB.h"


class VMM {
private:
    const int GAME_PROPORTION = 80;
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
    VMM(int TLBSize=64, std::string workloadType="random", std::string TLBReplacePolicy="random", int memorySize=8192, int pageSize=4, int pageTableSize=8192, int TLBTotalLevels=1){
        if(TLBSize > 1024){
            throw std::invalid_argument("TLB size greater than 1024 is too ideal.\n");
        }
        if(workloadType != "random" && workloadType != "game" && workloadType != "ml"){
            throw std::invalid_argument("Invalid workload type, please use random, game or ml\n");
        }
        if(TLBReplacePolicy != "random" && TLBReplacePolicy != "lru" && TLBReplacePolicy != "lfu"){
            throw std::invalid_argument("Invalid TLB replacement policy, please use random, lru or lfu\n");
        }
        if(memorySize < 1){
            throw std::invalid_argument("Memory size should be greater than 0\n");
        }
        if(pageSize < 1){
            throw std::invalid_argument("Page size should be greater than 0\n");
        }
        if(pageTableSize < 1){
            throw std::invalid_argument("Page table size should be greater than 0\n");
        }
        if(TLBTotalLevels < 1){
            throw std::invalid_argument("TLB total levels should be greater than 0\n");
        }
        if(pageTableSize % pageSize != 0){
            throw std::invalid_argument("Page table size should be dividable by page size\n");
        }
        if(memorySize % pageSize != 0){
            throw std::invalid_argument("Memory size should be dividable by page size\n");
        }

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
        this->counters.insert({"TLB Time", 0});
        this->counters.insert({"Memory Time", 0});

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
    /**
     * Run the simulation
     * @param totalRequests
     */
    void runSimulation(int totalRequests=10, std::string exportMode="console"){
        if(totalRequests < 1 ){
            throw std::invalid_argument("Total Request Count should be a positive integer\n");
        }
        if("console" != exportMode && "csv" != exportMode){
            throw std::invalid_argument("Invalid mode, please use console or csv\n");
        }
        std::vector<int> workload;
        //generate different type of workload based on input
        if("random" == this->workloadType){ // random workload
            workload = this->generateRandomWorkLoad(totalRequests);
        }else if("game" == this->workloadType){ // game workload
            workload = this->generateGameWorkLoad(totalRequests);
        }else if("ml" == this->workloadType) { // machine learning workload
            workload = this->generateMLWorkload(totalRequests);
        }else{
            throw std::invalid_argument("Invalid workload type, please use random, game or ml\n");
        }

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
        if("console" == exportMode){
            this->printWorkLoad(workload);
            this->printResult();
        }else if("csv" == exportMode){
            this->CSVFriendlyResults();
        }
    }


private:
    //*********** Simulation Results Print out & related Helper Methods ***********//

    /**
     * Print out the workload
     * @param workload
     */
        void printWorkLoad(std::vector<int> workload){
            std::cout << "----------- Workload Summary -----------\n";
            std::cout << "Workload Type: " << this->workloadType << "\n";
            std::cout << "Total Access: " << this->counters["Total Access Count"]  << " times" << "\n";

            std::cout << "Workload (VPN to access): \n[ ";
            for (int work: workload)
                std::cout << std::hex << std::setw(8) << std::setfill('0') << work << ", ";
            std::cout << "]\n";
        }



    /**
     * Print out the result
     */
    void printResult(){
        std::cout << "================== Result ==================\n";
        std::cout << "TLB Flushing Policy: " << this->TLBReplacePolicy << "\n";
        std::cout << "TLB Size: " << std::dec << this->tlb->getSize() << " entries" << "\n";
        std::cout << "----------- TLB Hit Rate -----------\n";
        std::cout << "TLB Hit Count: " << this->counters["TLB Hit"] << "\n";
        std::cout << "TLB Miss Count: " << this->counters["TLB Miss"] << "\n";
        std::cout << "TLB Hit Rate: " << (double)this->counters["TLB Hit"] / (double)this->counters["Total Access Count"] << "\n";
        std::cout << "----------- Page Fault Rate -----------\n";
        std::cout << "Page Fault Count: " << this->counters["Page Fault Count"] << "\n";
        std::cout << "Page Fault Rate: " << (double)this->counters["Page Fault Count"] / (double)this->counters["Total Access Count"] << "\n";

        //calculate the total time based on access count and speed factors
        this->calculateTime();

        int totalTime = this->counters["TLB Time"] + this->counters["Memory Time"];
        std::cout << "----------- Total Time -----------\n";
        std::cout << "Speed Factors -- " << "TLB Speed : Memory Speed = " << this->speedFactors["Memory"] << " : " << this->speedFactors["Level 1 TLB"] <<"\n";
        std::cout << "TLB Access Count: " << this->counters["TLB Access Count"] << "\n";
        std::cout << "Memory Access Count: " << this->counters["Memory Access Count"] << "\n";
        std::cout << "Total Time: " << totalTime << "\n";
        std::cout << "TLB Time: " << this->counters["TLB Time"] << "\n";
        std::cout << "Memory Time: " << this->counters["Memory Time"] << "\n";

        std::cout << "------------------------------------------\n";
    }



    /**
     * Calculate the total time based on access count and speed factors
     */
    void calculateTime(){
        int TLBTime = 0;
        int memoryTime = 0;
        for(auto const& [key, val] : this->counters){
            if("TLB Access Count" == key){
                TLBTime += val * this->speedFactors["Level 1 TLB"];
            }else if("Memory Access Count" == key){
                memoryTime += val * this->speedFactors["Memory"];
            }
        }
        this->counters["TLB Time"] = TLBTime;
        this->counters["Memory Time"] = memoryTime;
    }



    /**
     * Print out the result in CSV friendly format
     * the columns are:
     * Workload Type, Total Access Count, Repeated Addresses Rate, TLB Replacement Policy, TLB Size, TLB Hit Count, TLB Miss Count, TLB Hit Rate, Page Fault Count, Page Fault Rate, TLB Access Count, Memory Access Count, Total Time, TLB Time, Memory Time
     */
    void CSVFriendlyResults(){
        this->calculateTime();
        std::cout << this->workloadType << "," << this->counters["Total Access Count"] << "," << this->TLBReplacePolicy << "," << this->tlb->getSize() << "," << this->counters["TLB Hit"] << "," << this->counters["TLB Miss"] << "," << (double)this->counters["TLB Hit"] / (double)this->counters["Total Access Count"] << "," << this->counters["Page Fault Count"] << "," << (double)this->counters["Page Fault Count"] / (double)(this->counters["Total Access Count"]) << "," << this->counters["TLB Access Count"] << "," << this->counters["Memory Access Count"] << "," << this->counters["TLB Time"] + this->counters["Memory Time"] << "," << this->counters["TLB Time"] << "," << this->counters["Memory Time"] << "\n";
    }



    //*********** Simulation Running Helper Methods ***********//

    /**
     * Mimicking translation
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



    //*********** Workload Generators Helper Methods ***********//


    /**
     * Generate a random workload
     * @param totalRequests
     * @return a vector of VPNs
     */
    std::vector<int> generateRandomWorkLoad(int totalRequests){
        //initialize a random seed
        auto seed = std::chrono::high_resolution_clock ::now().time_since_epoch().count();
        std::mt19937 generator(seed);
        std::uniform_int_distribution<int> randomVPN(1, this->pageTableSize / this->pageSize + 1);

        if(totalRequests < 0 ){
            throw std::invalid_argument("Total Request Count should be a positive integer\n");
        }
        std::vector<int> workload;
        for(int i = 0; i < totalRequests; i++){
            //random pick a VPN adding to workload
            workload.push_back(randomVPN(generator)); //random address
            this->counters["Total Access Count"] += 1;
        }
        return workload;
    }



    /**
     * Generate a workload with high locality, 80% of the addresses are repeated
     * @param totalRequests
     * @return a vector of VPNs
     */
    std::vector<int> generateGameWorkLoad(int totalRequests){
        //initialize a random seed
        auto now = std::chrono::high_resolution_clock ::now();
        auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
        std::mt19937 generator(nanoseconds);
        std::uniform_int_distribution<int> randomPage(1, this->pageTableSize / this->pageSize + 1);
        std::uniform_int_distribution<int> randomProportion(1, 101);

        if(totalRequests < 1 ){
            throw std::invalid_argument("Total Request Count should be a positive integer\n");
        }
        std::vector<int> workload;

        // randomly add accessing address to workload list with 80% of the addresses are repeated

        for(int i = 0; i < totalRequests; i++){
            if(randomProportion(generator) % 100 < GAME_PROPORTION){ // 80% of the addresses are repeated
                if(workload.empty()){
                    workload.push_back(randomPage(generator)); //random address
                }else{
                    std::uniform_int_distribution<int> randomExistWork(0, workload.size() - 1);
                    workload.push_back(workload.at(randomExistWork(generator)));
                }
            }else{
                workload.push_back(randomPage(generator)); //random address
            }
            this->counters["Total Access Count"] += 1;
        }

        return workload;
    }



    /**
     * Generate a workload mimicking machine learning - matrix multiplication
     * @param totalRequests
     * @return a vector of VPNs
     */
    std::vector<int> generateMLWorkload(int totalRequests){
        //initialize a random seed
        auto now = std::chrono::high_resolution_clock ::now();
        auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
        std::mt19937 generator(nanoseconds);
        // 3 generators for two matrix
        std::uniform_int_distribution<int> matrix_A_random(1, (this->pageTableSize / this->pageSize) / 2); //for matrix A
        std::uniform_int_distribution<int> matrix_B_random((this->pageTableSize / this->pageSize) / 2 + 1, this->pageTableSize / this->pageSize + 1); // for matrix B
        std::uniform_int_distribution<int> matrix_random(0, 1); // randomly pick a matrix

        std::vector<int> workload;
        for(int i = 0; i < totalRequests; i++){
            // picking two random VPN from each matrix
            int VPN_A = matrix_A_random(generator);
            int VPN_B = matrix_B_random(generator);
            // randomly pick a VPN from the two VPNs
            int VPN = matrix_random(generator) == 0 ? VPN_A : VPN_B;
            workload.push_back(VPN_A);
            workload.push_back(VPN_B);
            workload.push_back(VPN);
            this->counters["Total Access Count"] += 3;
        }
        return workload;
    }




};


#endif //INC_5600_VVM_VMM_H
