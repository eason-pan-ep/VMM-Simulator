# Virtual Memory Manager Simulator
Part of CS5600 - Computer Systems coursework
<br>Eason Pan, Fall 2023

Demo Video: https://youtu.be/d2g7KoFLhSM

## Key Features
### Iteration 2 (see tag iteration-2)
- LRU TLB flushing policy
- LFU TLB flushing policy
- ML workload, randomized matrix interaction (2 matrices, each takes half of the page table size)

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
  - -w: workload type, default is random, the other 2 options are game and ml 
  - -T: TLB size, default is 64, takes up to 1024, please make sure it can divide the page size (default=4)
  - -m: memory size, default is 8192, please make sure it can divide the page size (default=4)
  - -p: page size, default is 4, takes up to 64, please make sure it can be divided by the memory size and page table size (default=8192)
  - -P: page table size, default is 8192, please make sure it can be divided by the memory size and page size (default=4)
  - -n: total requests, default is 200, since every request may require to access up to 5 different addresses, make sure it's not too much to handle 
  - -M: mode, default is console, the other option is csv 
  - -s: sample size, default is 1, only works in csv mode
  - -f: flushing policy, default is random, the other option are lru (Least Recent Used) and lfu (Least Frequent Used)
- default value of each parameters:
  - Workload Type: random
  - TLB Size: 64
  - Total Requests: 200
  - Memory Size: 8192
  - Page Size: 4
  - Page Table Size: 8192
  - Mode: console
  - Sample Size: 1
  - Flushing Policy: random
