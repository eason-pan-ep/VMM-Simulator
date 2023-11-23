//
// Created by Eason Pan on 2023-11-22.
//

#include <iostream>
#include "../src/WorkLoads/SimRequest.h"

int main(){
    SimpleRequest* testReq1 = new SimpleRequest(ALLOCATE);
    testReq1->printRequest();
    SimpleRequest* testReq2 = new SimpleRequest(ACCESS, 2);
    testReq2->printRequest();

    delete(testReq1);
    delete(testReq2);

    return 0;
}
