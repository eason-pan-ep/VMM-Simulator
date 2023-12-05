# Virtual Memory Manager Simulator
Part of CS5600 - Computer Systems coursework
<br>Eason Pan, Fall 2023

## Key Features
### Iteration 1 (see tag iteration-1)
- Standard VMM (page table + physical memory(free space))
- Single-level TLB
- Random TLB replacement policy
- Random workload, pure random access addresses
- Game workload, at least 80% repeated access addresses

### Statistic Features
- Total Access Count
- Repeated Addresses Rate
- TLB Hit & Miss
- TLB Hit Rate
- Page Fault Count & Rate
- TLB Access Count
- Memory Access Count
- Total Time, TLB Time, Memory Time (Speed factor --> TLB : Memory = 200 : 1, TLB speed is 200x faster than Memory speed)

### Utility Features
- Export CSV formatted results (can directly write output to a csv file with clean data)

## How to Run
- compile: `clang++ -std=c++17 -o vmm main.cpp`
- run: `./vmm [options]`
- options:
  - -h: helper info
  - -w: workload type, default is random, the other option is game
  - -T: TLB size, default is 64, takes up to 512, please make sure it can divide the page size (default=4)
  - -m: memory size, default is 8192, suggesting not make it greater than the default, please make sure it can divide the page size (default=4)
  - -p: page size, default is 4, takes up to 64, please make sure it can be divided by the memory size and page table size (default=8192)
  - -P: page table size, default is 8192, please make sure it can be divided by the memory size and page size (default=4)
  - -n: total requests, default is 200, since every request may require to access up to 5 different addresses, make sure it's not too much to handle
  - -M: mode, default is console, the other option is csv
- default value of each parameters:
    - Workload Type: random
  - TLB Size: 64
  - Total Requests: 200
  - Memory Size: 8192
  - Page Size: 4
  - Page Table Size: 8192
  - Mode: console
