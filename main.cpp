#include <iostream>
#include "./src/VMM.h"


int main(int argc, char *argv[]){
    if(argc != 2){
        std::cout << "Please provide workload type as the argument.\n";
        return 1;
    }
    std::string workloadType = argv[1];

    std::cout << "*******************************************************************************\n";
    std::cout << "Virtual Memory Manager Simulation Starts\n";
    std::cout << "========================================\n";
    std::cout << "Initializing VMM Simulator..........\n";
    VMM* vmm = new VMM(64, workloadType);
    std::cout << "Simulator Initialized\n";
    std::cout << "========================================\n";
    std::cout << "Running VMM Simulator..........\n";
    vmm->runSimulation(200);
    std::cout << "VMM Simulation Ends\n";
    delete(vmm);
    std::cout << "*******************************************************************************\n";
    return 0;

}
