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
