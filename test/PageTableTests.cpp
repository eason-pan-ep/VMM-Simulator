//
// Created by Eason Pan on 2023-11-13.
//
#include "../src/PageTable/PageTable.h"
#include <iostream>

int main(){
    PageTable* test = new PageTable(40);
    test->printPageTable();
    std::pair addRes = test->addEntry(3489);
    std::cout << "addEntry Result --> VPN: " << addRes.first;
    std::cout << ", opCount: " << addRes.second << "\n";
    test->printPageTable();
    std::cout << "\nPerform a walk: \n";
    test->walk(addRes.first)->printEntry();
    std::cout << "\nRest the added entry: \n";
    test->resetEntry(addRes.first);
    test->printPageTable();

    delete(test);

    return 0;
}