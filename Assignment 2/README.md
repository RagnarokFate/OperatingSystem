# Assignment 2: Concurrent List Implementation

## Overview

This assignment implements a thread-safe concurrent linked list data structure using POSIX threads and synchronization primitives. The implementation demonstrates critical concepts in concurrent programming including mutual exclusion, deadlock prevention, and thread synchronization.

## Features

- **Thread-Safe Operations**: All list operations are safe for concurrent access
- **Fine-Grained Locking**: Optimized locking strategy for better performance
- **Deadlock Prevention**: Careful lock ordering to prevent deadlocks
- **Memory Safety**: Proper memory management in multi-threaded environment
- **Stress Testing**: Comprehensive testing under high concurrency

## File Structure

```
Assignment 2/
├── concurrent_list.c      # List implementation
├── concurrent_list.h      # Header file with function declarations
├── test.c                 # Test program and stress testing
├── Makefile              # Build configuration
├── README.md             # This file
├── hw2.pdf               # Assignment specification
├── stress.in             # Stress test input
├── stress.out            # Expected stress test output
├── minimal.supp          # Valgrind suppression file
└── valgrind-3.4.1.tar.bz2  # Valgrind source (for reference)
```

## Usage

### Compilation
C compilation with pthread library support and appropriate compiler flags for thread safety.

### Testing
The assignment includes comprehensive testing:
- Basic functionality testing
- Concurrent access testing  
- Stress testing with multiple threads
- Memory testing for leaks and errors

## Implementation Details

### Data Structures

```c
struct node {
    int value;
    // Additional synchronization fields
};

struct list {
    // List management fields
    // Synchronization primitives
};
```

### Core Operations

1. **list_create()**: Initialize a new concurrent list
2. **list_add()**: Add element to list (thread-safe)
3. **list_remove()**: Remove element from list (thread-safe)
4. **list_contains()**: Check if element exists (thread-safe)
5. **list_size()**: Get current list size (thread-safe)
6. **list_destroy()**: Clean up list and free memory

### Synchronization Strategy

- **Mutual Exclusion**: pthread_mutex_t for protecting critical sections
- **Fine-Grained Locking**: Per-node or per-section locking for performance
- **Lock Ordering**: Consistent ordering to prevent deadlocks
- **Reader-Writer Optimization**: Potential optimization for read-heavy workloads

### Thread Safety Guarantees

- **Atomicity**: All operations appear atomic to other threads
- **Consistency**: List invariants maintained across all operations
- **Isolation**: Concurrent operations don't interfere with each other
- **Durability**: Completed operations persist until explicitly removed

## Key Algorithms

### Insertion Algorithm
1. Acquire necessary locks in consistent order
2. Find correct position for insertion
3. Update pointers atomically
4. Release locks in reverse order

### Deletion Algorithm
1. Acquire locks for target node and predecessors
2. Verify node still exists (avoid ABA problem)
3. Update pointers to bypass node
4. Free memory safely
5. Release locks

### Search Algorithm
1. Acquire read locks as needed
2. Traverse list while maintaining locks
3. Return result
4. Release all acquired locks

## Testing Framework

### Unit Tests
- Basic functionality testing
- Single-threaded correctness
- Edge cases (empty list, single element, etc.)

### Concurrent Tests
- Multiple threads performing same operation
- Mixed operations (add/remove/search simultaneously)
- High contention scenarios
- Race condition detection

### Stress Tests
- Long-running tests with many threads
- Random operation sequences
- Memory pressure testing
- Performance benchmarking

## Building and Dependencies

### Requirements
- **Compiler**: C compiler with C99 support
- **Threading**: POSIX threads (pthread) support
- **Target Platform**: Linux/Unix environment for pthread functionality
- **Build System**: Makefile provided for compilation

### Compilation Considerations
- Requires pthread library linking
- Uses C99 standard
- Includes debug information for development
- Compiler warnings treated as errors for code quality

## Performance Considerations

### Optimization Strategies
- **Lock Granularity**: Balance between safety and performance
- **Lock Duration**: Minimize time locks are held
- **Memory Locality**: Optimize data structure layout
- **Contention Reduction**: Distribute locks to reduce contention

### Scalability
- Performance should scale reasonably with number of threads
- Avoid global locks where possible
- Consider cache line effects in multi-core systems

## Common Issues and Solutions

### Race Conditions
- Use appropriate synchronization primitives
- Careful ordering of operations
- Atomic operations where applicable

### Deadlocks
- Consistent lock ordering
- Timeout mechanisms
- Deadlock detection algorithms

### Memory Leaks
- Proper cleanup in all code paths
- Exception safety considerations
- Valgrind testing for leak detection

## Key Learning Objectives

- Understanding of thread synchronization primitives
- Deadlock prevention and detection
- Race condition identification and resolution
- Performance optimization in concurrent systems
- Memory management in multi-threaded environments
- Testing concurrent programs

## Debugging Tips

1. Use thread-safe debugging tools
2. Add logging with thread IDs
3. Use Valgrind's thread error detection
4. Test with different thread counts
5. Use stress testing to expose race conditions

## Error Handling

- **Memory Allocation**: Handle malloc failures gracefully
- **Lock Failures**: Check return values of pthread functions
- **Invalid Parameters**: Validate input parameters
- **Resource Cleanup**: Ensure proper cleanup on errors