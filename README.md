# Operating Systems Course Assignments

This repository contains implementations for various operating systems concepts and programming assignments. Each assignment focuses on different fundamental aspects of operating systems.

## Course Overview

This collection of assignments covers core operating systems concepts including:
- Basic C programming and system calls
- Shell implementation and process management
- Concurrent programming and synchronization
- Kernel module development
- Virtual memory management

## Assignments Structure

### Assignment 0: Calculator
- **Topic**: Basic C programming
- **Files**: `calc.c`
- **Description**: Simple command-line calculator implementation

### Assignment 1: Shell Implementation
- **Topic**: Process management and shell commands
- **Files**: `myshell.c`
- **Description**: Custom shell with command history and background process support

### Assignment 2: Concurrent List
- **Topic**: Thread synchronization and concurrent data structures
- **Files**: `concurrent_list.c`, `concurrent_list.h`, `test.c`, `Makefile`
- **Description**: Thread-safe linked list implementation

### Assignment 3: Encryption/Decryption Kernel Module
- **Topic**: Kernel module development
- **Files**: `encdec.c`
- **Description**: Linux kernel module for encryption and decryption operations

### Assignment 4: Virtual Memory Simulator
- **Topic**: Virtual memory management
- **Files**: `virtmem.c`
- **Description**: Page replacement algorithms and virtual memory simulation

## Additional Resources

The `osExams/` directory contains:
- Past exam questions and solutions
- Reference materials
- Study guides in Hebrew and English

## Building and Running

Each assignment contains its own README with specific build instructions and implementation details. 

**Important Note**: These assignments are designed for Linux/Unix environments and demonstrate low-level system programming:
- Assignments 1, 3, and 4 require Linux system calls and kernel features
- Assignment 2 uses POSIX threads (pthread) for concurrent programming
- Assignment 0 can be compiled on most systems with a standard C compiler
- Assignment 3 specifically requires Linux kernel development environment

For development and testing:
- These assignments are typically developed in Linux environments
- University lab computers or Linux virtual machines provide the required environment
- The focus is on understanding operating systems concepts through C programming

## Requirements

- **Compiler**: C compiler with appropriate standard library support
- **Target Environment**: Linux/Unix systems for full functionality  
- **Libraries**: POSIX threads (pthread) for concurrent programming assignments
- **Development Tools**: Standard C development environment

## Note

This repository is for educational purposes. All implementations follow academic guidelines and focus on understanding core operating systems concepts.