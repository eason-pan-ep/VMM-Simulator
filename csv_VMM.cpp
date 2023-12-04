//
// Created by Eason Pan on 2023-12-04.
//
#include <iostream>
#include <unistd.h>
#include "./src/VMM.h"

int main( int argc, char *argv[]){
    if(argc != 2){
        std::cout << "Please provide sample size as argument.\n";
        return 1;
    }
    int sampleSize = std::stoi(argv[1]);

    std::cout << "#,Workload Type,Total Access Count,Repeated Addresses Rate,TLB Replacement Policy,TLB Size,TLB Hit Count,TLB Miss Count,TLB Hit Rate,Page Fault Count,Page Fault Rate,TLB Access Count,Memory Access Count,Total Time,TLB Time,Memory Time\n";

    for(int i = 0; i < sampleSize; i++){
        VMM* vmm = new VMM();
        std::cout << i+1 << ",";
        vmm->runSimulation(200, "csv");
        delete(vmm);
        sleep(1);
    }

    return 0;
}
