#include <iostream>
#include "./src/VMM.h"

void helperInfo(){
    std::cout << "*********************************************************************************************************\n";
    std::cout << "Welcome to VMM Simulator!\n";
    std::cout << "Default value for each parameters are listed below: \n";
    std::cout << "- Workload Type: random\n";
    std::cout << "- TLB Size: 64\n";
    std::cout << "- Total Requests: 200\n";
    std::cout << "- Memory Size: 8192\n";
    std::cout << "- Page Size: 4\n";
    std::cout << "- Page Table Size: 8192\n";
    std::cout << "- Mode: console\n";
    std::cout << "---------------------------------------------------------------------------------------------------------\n";
    std::cout << "The simulator takes up to 6 different parameters at listed below: \n";
    std::cout << "   -w: workload type, default is random, the other option is game\n";
    std::cout << "   -T: TLB size, default is 64, takes up to 512, please make sure it can divide the page size (default=4)\n";
    std::cout << "   -m: memory size, default is 8192, suggesting not make it greater than the default, please make sure it can divide the page size (default=4)\n";
    std::cout << "   -p: page size, default is 4, takes up to 64, please make sure it can be divided by the memory size and page table size (default=8192)\n";
    std::cout << "   -P: page table size, default is 8192, please make sure it can be divided by the memory size and page size (default=4)\n";
    std::cout << "   -n: total requests, default is 200, since every request may require to access up to 5 different addresses, make sure it's not too much to handle\n";
    std::cout << "   -M: mode, default is console, the other option is csv\n";
}


int main(int argc, char *argv[]){
    // default values
    std::string workloadType = "random";
    int TLBSize = 64;
    int totalRequests = 200;
    int memorySize = 8192;
    int pageSize = 4;
    int pageTableSize = 8192;
    std::string mode = "console";


    // parsing arguments
    for(int i = 1; i < argc; i++){
        // helper
        if(strcmp(argv[i], "-h") == 0){
            helperInfo();
            return 0;
        }
        // workload type
        if(strcmp(argv[i], "-w") == 0){
            if(i + 1 < argc){
                if(strcmp(argv[i+1], "random") == 0 || strcmp(argv[i+1], "game") == 0){
                    workloadType = argv[i+1];
                }else{
                    std::cout << "Invalid workload type, please use random or game\n";
                    return 1;
                }
            }else{
                std::cout << "Invalid workload type, please use random or game\n";
                return 1;
            }
        }
        // TLB size
        if(strcmp(argv[i], "-T") == 0){
            if(i + 1 < argc){
                try{
                    TLBSize = std::stoi(argv[i+1]);
                }catch (std::invalid_argument& e){
                    std::cout << "Invalid TLB size, please use an integer\n";
                    return 1;
                }
                if(TLBSize > 512 || TLBSize < 1){
                    std::cout << "Invalid TLB size, please use an integer between 1 and 512\n";
                    return 1;
                }
            }else{
                std::cout << "Invalid TLB size, please use an integer between 1 and 512\n";
                return 1;
            }
        }
        // total requests
        if(strcmp(argv[i], "-n") == 0){
            if(i + 1 < argc){
                try{
                    totalRequests = std::stoi(argv[i+1]);
                }catch (std::invalid_argument& e){
                    std::cout << "Invalid total requests, please use an integer\n";
                    return 1;
                }
                if(totalRequests < 1){
                    std::cout << "Invalid total requests, please use an integer greater than 0\n";
                    return 1;
                }
            }else{
                std::cout << "Invalid total requests, please use an integer greater than 0\n";
                return 1;
            }
        }
        // memory size
        if(strcmp(argv[i], "-m") == 0){
            if(i + 1 < argc){
                try{
                    memorySize = std::stoi(argv[i+1]);
                }catch (std::invalid_argument& e){
                    std::cout << "Invalid memory size, please use an integer\n";
                    return 1;
                }
                if(memorySize < 1){
                    std::cout << "Invalid memory size, please use an integer greater than 0\n";
                    return 1;
                }
            }else{
                std::cout << "Invalid memory size, please use an integer greater than 0\n";
                return 1;
            }
        }
        // page size
        if(strcmp(argv[i], "-p") == 0){
            if(i + 1 < argc){
                try{
                    pageSize = std::stoi(argv[i+1]);
                }catch (std::invalid_argument& e){
                    std::cout << "Invalid page size, please use an integer\n";
                    return 1;
                }
                if(pageSize > 64 || pageSize < 1){
                    std::cout << "Invalid page size, please use an integer between 1 and 64\n";
                    return 1;
                }
            }else{
                std::cout << "Invalid page size, please use an integer between 1 and 64\n";
                return 1;
            }
        }
        // page table size
        if(strcmp(argv[i], "-P") == 0){
            if(i + 1 < argc){
                try{
                    pageTableSize = std::stoi(argv[i+1]);
                }catch (std::invalid_argument& e){
                    std::cout << "Invalid page table size, please use an integer\n";
                    return 1;
                }
                if(pageTableSize < 1){
                    std::cout << "Invalid page table size, please use an integer greater than 0\n";
                    return 1;
                }
            }else{
                std::cout << "Invalid page table size, please use an integer greater than 0\n";
                return 1;
            }
        }
        // mode
        if(strcmp(argv[i], "-M") == 0){
            if(i + 1 < argc){
                if(strcmp(argv[i+1], "console") == 0 || strcmp(argv[i+1], "csv") == 0){
                    mode = argv[i+1];
                }else{
                    std::cout << "Invalid mode, please use console or csv\n";
                    return 1;
                }
            }else{
                std::cout << "Invalid mode, please use console or csv\n";
                return 1;
            }
        }

    }

    // running simulation - console mode
    if(mode == "console"){
        std::cout << "*******************************************************************************\n";
        std::cout << "Virtual Memory Manager Simulation Starts\n";
        std::cout << "========================================\n";
        std::cout << "Initializing VMM Simulator..........\n";
        VMM* vmm = new VMM(TLBSize, workloadType, "random", memorySize, pageSize, pageTableSize);
        std::cout << "Simulator Initialized\n";
        std::cout << "========================================\n";
        std::cout << "Running VMM Simulator..........\n";
        vmm->runSimulation(totalRequests, mode);
        std::cout << "VMM Simulation Ends\n";
        delete(vmm);
        std::cout << "*******************************************************************************\n";
    }else{
        // running simulation - csv mode
        std::cout << "#,Workload Type,Total Access Count,Repeated Addresses Rate,TLB Replacement Policy,TLB Size,TLB Hit Count,TLB Miss Count,TLB Hit Rate,Page Fault Count,Page Fault Rate,TLB Access Count,Memory Access Count,Total Time,TLB Time,Memory Time\n";
        for(int i = 0; i < totalRequests; i++){
            VMM* vmm = new VMM(TLBSize, workloadType, "random", memorySize, pageSize, pageTableSize);
            std::cout << i+1 << ",";
            vmm->runSimulation(200, "csv");
            delete(vmm);
        }
    }

    return 0;

}
