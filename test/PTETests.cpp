//
// Created by Eason Pan on 2023-11-11.
// including tests for PTE and TLBEntry
//
#include <iostream>
#include "../src/PTE.h"
#include "../src/TLBEntry.h"

int test_PTE_printEntry(){
    std::cout << "printEntry - PTE test: \n";
    PTE* testPTE = new PTE(110001);
    std::cout << "empty PTE: ";
    testPTE->printEntry();

    std::cout << "\nvalid to true: ";
    testPTE->validBit = true;
    testPTE->printEntry();

    delete testPTE;
}

int test_TLB_printEntry(){
    std::cout << "printEntry - TLBEntry test: \n";
    TLBEntry* testTLBE = new TLBEntry(10001001, 10101000, 1, true, false, true, false, false);
    testTLBE->printEntry();

    delete testTLBE;
}



int main(){
    test_PTE_printEntry();
    std::cout << "\n------------------------------------------------------------------\n";
    test_TLB_printEntry();

    return 0;
}
