//
// Created by Eason Pan on 2023-11-13.
//
#include "../src/PageTable/PageTable.h"
#include <iostream>



int main(){
    PageTable* testPT = new PageTable(16);
    std::cout << "empty table print: ";
    testPT->printPageTable();

    for(int i = 0; i < 4; i++){
        testPT->addEntry();
    }
    std::cout << "Initial Table: \n";
    testPT->printPageTable();

    testPT->translate(8, 100001);
    testPT->translate(12, 120000);
    std::cout << "8 & 12 translated: \n";
    testPT->printPageTable();

    testPT->translate(8, -1);
    std::cout << "8 reset: \n";
    testPT->printPageTable();

    PTE* find = testPT->walk(12);
    std::cout << "walk with 12: \n";
    find->printEntry();

    delete(testPT);

    return 0;
}