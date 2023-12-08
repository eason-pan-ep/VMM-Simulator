//
// Created by Eason Pan on 2023-11-11.
// including tests for PTE and TLBEntry
//
#include <iostream>
#include "../src/PageTable/PTE.h"

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



int test_PTE_use_and_rest(){
    std::cout << "PTE use and rest test:\n";
    PTE* test = new PTE(-1);
    test->printEntry();
    test->use(2000022020);
    test->printEntry();
    test->reset();
    test->printEntry();

    delete(test);

}



int main(){
    test_PTE_printEntry();
    std::cout << "\n------------------------------------------------------------------\n";

    return 0;
}
