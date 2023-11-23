//
// Created by Eason Pan on 2023-11-13.
//
#include <iostream>
#include "../src/PhysicalMemory/PhysicalMemory.h"

int main(){
    int TEST_M_SIZE = 64;
    int TEST_P_SIZE = 4;
    int TEST_CHUNKS = TEST_M_SIZE / TEST_P_SIZE;
    std::cout << "------ Initialized Memory ------ \n";
    PhysicalMemory* test = new PhysicalMemory(TEST_M_SIZE, TEST_P_SIZE);
    test->printMemory();

    std::cout << "------ Get half of the chunks ------\n";
    for(int i = 0; i < TEST_CHUNKS/2; i++){
        test->findOneChunk();
    }
    test->printMemory();

    std::cout << "------ Get the rest of the chunks ------\n";
    for(int i = 0; i < TEST_CHUNKS/2; i++){
        test->findOneChunk();
    }
    test->printMemory();

    std::cout << "------ Try to get a chunk when the memory is full ------\n";
    int res = test->findOneChunk();
    if(-1 == res){
        std::cout << "Passed Test\n";
    }else{
        std::cout << "Failed Test\n";
    }

    delete(test);

    return 0;
}