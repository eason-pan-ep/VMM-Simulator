//
// Created by Eason Pan on 2023-11-09.
//

#ifndef INC_5600_VVM_SIMREQUEST_H
#define INC_5600_VVM_SIMREQUEST_H

#include <iostream>
#include "RequestType.h"

class SimpleRequest{
public:
    RequestType type;
    int VPN;

    SimpleRequest(RequestType type, int VPN=-1){ //VPN default equals to -1, only access operation need specific VPN
        this->type = type;
        this->VPN = VPN;
    }

    ~SimpleRequest() = default;

    /**
     * Print out this request
     */
    void printRequest(){
        std::cout << "Request to " << toString(this->type);
        if(this->type == ACCESS){
            std::cout << ", Address: " << this->VPN;
        }
        std::cout << "\n";
    }
};

#endif //INC_5600_VVM_SIMREQUEST_H
