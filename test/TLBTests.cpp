//
// Created by Eason Pan on 2023-11-13.
//
#include <iostream>
#include "../src/TLB/TLB.h"
#include "../src/PageTable/PTE.h"

int main(){
    int TEST_TLB_SIZE = 4;
    std::cout << "------------------- TLB Tests -------------------\n";
    TLB* test = new TLB(TEST_TLB_SIZE);
    std::cout << "------ Empty TLB ------ \n";
    test->printTLB();

    for(int i = 0; i < TEST_TLB_SIZE; i++){
        test->addEntry(i, new PTE(i + 20 + 100 + 50));
    }
    std::cout << "------ Full TLB ------ \n";
    test->printTLB();

    std::cout << "------ Replaced one entry ------ \n";
    test->addEntry(20, new PTE(30));
    test->printTLB();

    std::cout << "------ A TLB Hit ------ \n";
    int res = test->lookup(20);
    std::string TLBT = "TLB Hit";
    if(-1 == res){
        TLBT = "TLB Miss";
    }
    std::cout << "Result: " << TLBT << "\n";

    std::cout << "------ Replaced another entry ------ \n";
    test->addEntry(23, new PTE(14));
    test->printTLB();

    std::cout << "------ Try TLB Miss ------ \n";
    res = test->lookup(10);
    if(-1 == res){
        TLBT = "TLB Miss";
    }
    std::cout << "Result: " << TLBT << "\n";

    delete(test);

    return 0;

}
