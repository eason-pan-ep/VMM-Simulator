#include <iostream>
#include "./src/VMM.h"

int main() {
    std::cout << "Virtual Memory Manager Simulation Starts\n";
    std::cout << "========================================\n";
    std::cout << "Initializing VMM..........\n";
    VMM* vmm = new VMM();
    std::cout << "VMM Initialized\n";
    std::cout << "========================================\n";
    std::cout << "Running VMM..........\n";
    vmm->runSimulation(300);
    std::cout << "VMM Finished\n";
    std::cout << "========================================\n";
    std::cout << "VMM Simulation Ends\n";
    delete(vmm);
    return 0;

}
