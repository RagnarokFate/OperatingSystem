# Assignment 4: Virtual Memory Simulator

## Overview

This assignment implements a virtual memory management system simulator that demonstrates page replacement algorithms, address translation, and memory management concepts. The simulator includes a Translation Lookaside Buffer (TLB) and implements various page replacement strategies.

## Features

- **Address Translation**: Virtual to physical address mapping
- **TLB Implementation**: Translation Lookaside Buffer for fast address translation
- **Page Replacement Algorithms**: Multiple algorithms for page fault handling
- **Memory Management**: Physical memory allocation and deallocation
- **Statistics Tracking**: Page fault rates, TLB hit/miss ratios
- **Configurable Parameters**: Adjustable memory sizes and algorithm selection

## File Structure

```
Assignment 4/
├── virtmem.c       # Virtual memory simulator implementation
├── README.md       # This file
└── submitters.txt  # Submission information
```

## Implementation Details

### Memory Configuration

```c
#define TLB_SIZE 16        // TLB entries
#define PAGES 256          // Number of pages
#define PAGE_SIZE 256      // Size of each page
#define OFFSET_BITS 8      // Bits for page offset
#define OFFSET_MASK 255    // Mask for extracting offset
```

### Core Components

1. **TLB (Translation Lookaside Buffer)**
   - Fast cache for recent address translations
   - Implements LRU replacement policy
   - Configurable size (default: 16 entries)

2. **Page Table**
   - Maps virtual pages to physical frames
   - Tracks page validity and reference information
   - Supports demand paging

3. **Physical Memory**
   - Simulated physical memory frames
   - Frame allocation and deallocation
   - Memory access simulation

4. **Page Replacement Algorithms**
   - FIFO (First In, First Out)
   - LRU (Least Recently Used)
   - Optimal (Belady's Algorithm)
   - Clock Algorithm

## Usage

### Compilation
Standard C compilation.

### Program Input
The program reads virtual addresses from a file, one address per line:
```
16384
32768
19986
8192
...
```

### Sample Output
```
Virtual address: 16384, Physical address: 20
Virtual address: 32768, Physical address: 16
Virtual address: 19986, Physical address: 62
...

Statistics:
Page faults: 250
TLB hits: 54
TLB misses: 946
Page fault rate: 25.0%
TLB hit rate: 5.4%
```

## Address Translation Process

### Step-by-Step Translation

1. **Extract Page Number and Offset**
   ```c
   int page_number = (address >> OFFSET_BITS) & PAGE_MASK;
   int offset = address & OFFSET_MASK;
   ```

2. **TLB Lookup**
   - Check if page translation exists in TLB
   - If TLB hit: return physical address directly
   - If TLB miss: proceed to page table lookup

3. **Page Table Lookup**
   - Check if page is in physical memory
   - If page hit: update TLB and return address
   - If page fault: trigger page replacement

4. **Page Replacement (if needed)**
   - Select victim page using replacement algorithm
   - Load new page from storage
   - Update page table and TLB

## Page Replacement Algorithms

### FIFO (First In, First Out)
- Replaces the oldest page in memory
- Simple to implement but may not be optimal
- Can suffer from Belady's Anomaly

### LRU (Least Recently Used)
- Replaces the page that hasn't been used for the longest time
- Good performance but requires tracking access times
- Approximates optimal behavior

### Optimal (Belady's Algorithm)
- Replaces the page that will be accessed furthest in the future
- Provides optimal page fault rate
- Used as benchmark (requires future knowledge)

### Clock Algorithm
- Approximates LRU using reference bits
- Circular scanning of pages
- Efficient implementation of LRU approximation

## Data Structures

### TLB Entry
```c
typedef struct {
    int page_number;
    int frame_number;
    int valid;
    int last_used;
} tlb_entry;
```

### Page Table Entry
```c
typedef struct {
    int frame_number;
    int valid;
    int reference_bit;
    int modify_bit;
    int last_accessed;
} page_entry;
```

### Statistics Structure
```c
typedef struct {
    int page_faults;
    int tlb_hits;
    int tlb_misses;
    int total_accesses;
} statistics;
```

## Testing

### Test Cases

1. **Sequential Access Pattern**
   ```
   0, 1, 2, 3, 4, ...
   ```

2. **Random Access Pattern**
   ```
   Random virtual addresses
   ```

3. **Locality Test**
   ```
   Addresses with temporal and spatial locality
   ```

4. **Worst Case Scenario**
   ```
   Pattern that causes maximum page faults
   ```

### Performance Analysis

Testing involves running the simulator with different algorithms and comparing results. The implementation allows for:

- Testing sequential access patterns
- Testing random access patterns  
- Testing patterns with temporal/spatial locality
- Analyzing worst-case scenarios for page faults
- Comparing effectiveness of different replacement algorithms

## Key Learning Objectives

- Understanding virtual memory concepts
- Address translation mechanisms
- TLB operation and management
- Page replacement algorithm implementation
- Memory hierarchy performance analysis
- Working set and locality principles

## Configuration Options

### Compile-time Configuration

```c
// Modify these defines to change system parameters
#define TLB_SIZE 16        // TLB entries (4, 8, 16, 32)
#define PAGES 256          // Virtual pages (128, 256, 512)
#define PAGE_SIZE 256      // Page size in bytes
#define PHYSICAL_FRAMES 128 // Physical memory frames
```

### Runtime Options

- `-a ALGORITHM`: Choose replacement algorithm (FIFO, LRU, OPTIMAL, CLOCK)
- `-s`: Display detailed statistics
- `-v`: Verbose output showing each translation
- `-t`: Display TLB contents after each access

## Performance Metrics

### Key Measurements
- **Page Fault Rate**: Percentage of memory accesses causing page faults
- **TLB Hit Rate**: Percentage of addresses resolved by TLB
- **Average Access Time**: Weighted average of memory access times
- **Working Set Size**: Number of unique pages accessed in time window

### Optimization Strategies
- **TLB Size Tuning**: Balance cost vs. performance
- **Algorithm Selection**: Choose based on access patterns
- **Prefetching**: Anticipate future page needs
- **Page Size Optimization**: Balance internal vs. external fragmentation

## Common Issues and Solutions

### High Page Fault Rate
- **Cause**: Insufficient physical memory or poor algorithm choice
- **Solution**: Increase physical frames or use better algorithm

### Poor TLB Performance
- **Cause**: Large working set or frequent context switches
- **Solution**: Increase TLB size or improve locality

### Thrashing
- **Cause**: Working set larger than physical memory
- **Solution**: Increase memory or reduce working set

## Advanced Features

### Extended Implementation
- **Second Chance Algorithm**: Enhanced FIFO with reference bits
- **Working Set Model**: Dynamic page set management
- **Prepaging**: Predictive page loading
- **Page Clustering**: Group related pages

### Real-world Considerations
- **Translation Caches**: Multi-level TLBs
- **Memory Protection**: Read/write/execute permissions
- **Shared Memory**: Pages shared between processes
- **Memory Mapping**: File-backed memory regions

## Debugging and Analysis

### Debug Output
```c
#ifdef DEBUG
printf("Page fault: virtual page %d loaded to frame %d\n", page, frame);
#endif
```

### Trace Analysis
- Track page access patterns
- Identify hot pages and cold pages
- Analyze temporal and spatial locality
- Measure algorithm effectiveness

## Limitations

- Simplified memory model (no I/O delays)
- Single process simulation
- No memory protection mechanisms
- Fixed page size
- No support for variable-sized pages