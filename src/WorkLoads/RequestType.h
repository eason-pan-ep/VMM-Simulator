//
// Created by Eason Pan on 2023-11-22.
//

#ifndef INC_5600_VVM_REQUESTTYPE_H
#define INC_5600_VVM_REQUESTTYPE_H

#include <iostream>

enum RequestType{
    ALLOCATE,
    ACCESS
};

std::string toString(RequestType requestType){
    switch (requestType) {
        case ALLOCATE: return "Allocate";
        case ACCESS: return "Access";
        default: "Unknown Request Type";
    }
}

#endif //INC_5600_VVM_REQUESTTYPE_H


