#include <iostream>
#include "./src/VMM.h"


int main() {

    std::cout << "*******************************************************************************\n";
    std::cout << "Virtual Memory Manager Simulation Starts\n";
    std::cout << "========================================\n";
    std::cout << "Initializing VMM Simulator..........\n";
    VMM* vmm = new VMM();
    std::cout << "Simulator Initialized\n";
    std::cout << "========================================\n";
    std::cout << "Running VMM Simulator..........\n";
    vmm->runSimulation(300);
    std::cout << "VMM Simulation Ends\n";
    delete(vmm);
    std::cout << "*******************************************************************************\n";
    return 0;

}
