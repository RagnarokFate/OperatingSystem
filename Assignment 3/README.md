# Assignment 3: Encryption/Decryption Kernel Module

## Overview

This assignment implements a Linux kernel module that provides encryption and decryption functionality through character device files. The module demonstrates kernel programming concepts including device drivers, kernel memory management, and user-kernel space interaction.

## Features

- **Character Device Interface**: Provides `/dev/encdec` device file
- **Multiple Encryption Methods**: Support for various encryption algorithms
- **User-Kernel Communication**: ioctl interface for configuration
- **Memory Management**: Proper kernel memory allocation and deallocation
- **Process Isolation**: Separate encryption contexts per process
- **Key Management**: Secure key handling within kernel space

## File Structure

```
Assignment 3/
├── encdec.c        # Kernel module implementation
├── encdec.h        # Header file (if exists)
├── README.md       # This file
└── submitters.txt  # Submission information
```

## Module Operations

### Device File Operations
- **open()**: Initialize encryption context for process
- **close()**: Clean up process-specific resources
- **read()**: Read and decrypt data
- **write()**: Encrypt and write data
- **ioctl()**: Configure encryption parameters

### Encryption Support
- Caesar cipher
- XOR encryption
- Custom encryption algorithms
- Key-based encryption methods

## Usage

### Building the Module

```bash
# This requires a Linux development environment
# Ensure kernel headers are available for compilation
make

# Or manually compile (Linux environment):
gcc -I/lib/modules/$(uname -r)/build/include -O2 -DMODULE -D__KERNEL__ -c encdec.c
```

**Note**: This kernel module is designed for Linux systems and requires:
- Linux kernel development environment
- Kernel headers installed
- Root privileges for module operations

### Module Operations (Linux Environment)

```bash
# Insert the module (requires root)
insmod encdec.ko

# Verify module is loaded
lsmod | grep encdec

# Create device file
mknod /dev/encdec c <major_number> 0

# Use the device
echo "Hello World" > /dev/encdec
cat /dev/encdec

# Remove the module
rmmod encdec
```

## Implementation Details

### Module Structure

```c
// Module metadata
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Encryption/Decryption Device Driver");

// Device operations structure
static struct file_operations fops = {
    .open = device_open,
    .release = device_close,
    .read = device_read,
    .write = device_write,
    .unlocked_ioctl = device_ioctl
};
```

### Key Functions

1. **module_init()**: Module initialization
2. **module_exit()**: Module cleanup
3. **device_open()**: Handle device file open
4. **device_close()**: Handle device file close
5. **device_read()**: Read and decrypt operation
6. **device_write()**: Encrypt and write operation
7. **device_ioctl()**: Configuration interface

### Memory Management

- **kmalloc()/kfree()**: Kernel memory allocation
- **copy_from_user()/copy_to_user()**: User-kernel data transfer
- **Buffer Management**: Proper buffer allocation and cleanup
- **Error Handling**: Graceful failure handling

### Synchronization

- **Spinlocks**: Protect critical sections
- **Semaphores**: Process synchronization
- **Atomic Operations**: Thread-safe counters
- **Per-Process Data**: Isolated encryption contexts

## Encryption Algorithms

### Caesar Cipher
- Simple character shifting encryption
- Configurable shift value
- Bidirectional operation

### XOR Encryption
- Bitwise XOR with key
- Symmetric encryption/decryption
- Key can be single byte or multi-byte

### Configuration Interface

```c
// ioctl commands (example)
#define ENCDEC_SET_KEY    _IOW('E', 1, int)
#define ENCDEC_SET_METHOD _IOW('E', 2, int)
#define ENCDEC_GET_STATUS _IOR('E', 3, int)
```

## Testing

### Basic Functionality Test

```c
// Example C program to test the device
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd = open("/dev/encdec", O_RDWR);
    write(fd, "test message", 12);
    // Read back encrypted/decrypted data
    char buffer[100];
    read(fd, buffer, sizeof(buffer));
    close(fd);
    return 0;
}
```

### Testing Approach

The kernel module functionality would be tested by:
1. Writing C programs that open the device file
2. Testing different ioctl commands for configuration
3. Verifying encryption/decryption behavior
4. Testing concurrent access from multiple processes

## Key Learning Objectives

- Linux kernel module development
- Character device driver implementation
- Kernel memory management
- User-kernel space communication
- Device file operations
- Kernel synchronization primitives
- Security considerations in kernel space

## Security Considerations

- **Input Validation**: Validate all user input
- **Buffer Overflows**: Prevent kernel buffer overflows
- **Privilege Escalation**: Ensure proper permission checking
- **Memory Protection**: Protect kernel memory from corruption
- **Key Security**: Secure handling of encryption keys

## Debugging

### Kernel Debugging Concepts
- Use `printk()` for kernel-level debugging output
- Kernel messages can be viewed with `dmesg` command
- Dynamic debugging can be enabled for detailed tracing
- Memory debugging tools help detect leaks and corruption

### Common Implementation Issues
- **Device Registration**: Properly register/unregister character device
- **Memory Management**: Correct use of `kmalloc()`/`kfree()`
- **User-Kernel Data Transfer**: Safe copying with `copy_from_user()`/`copy_to_user()`
- **Concurrency**: Proper locking to handle multiple processes

### Development Focus
The assignment focuses on:
- Understanding kernel module structure and lifecycle
- Implementing character device operations
- Managing kernel memory safely
- Handling user-kernel space communication

## Development Environment

### Requirements
- **Target Platform**: Linux kernel development environment
- **Compiler**: GCC with kernel compilation support
- **Headers**: Kernel development headers
- **Testing**: Linux system for module loading and testing

### Development Approach
This assignment is typically developed and tested in:
- Linux virtual machine or container
- University lab computers with Linux
- WSL (Windows Subsystem for Linux) environment
- Remote Linux development server

### Key Concepts
The assignment teaches:
- Kernel module initialization and cleanup
- Character device driver implementation
- Kernel memory management principles
- System call interface design

## Error Handling

- **EINVAL**: Invalid parameters
- **ENOMEM**: Memory allocation failure
- **EBUSY**: Device busy
- **EFAULT**: Bad address (user space)
- **EPERM**: Permission denied

## Performance Considerations

- **Minimize System Call Overhead**: Efficient read/write operations
- **Buffer Management**: Optimal buffer sizes
- **Lock Contention**: Minimize lock duration
- **Memory Allocation**: Avoid frequent allocation/deallocation

## Limitations

- Single character device instance
- Limited encryption algorithm support
- No persistent key storage
- Platform-specific implementation