# Assignment 1: Custom Shell Implementation

## Overview

This assignment implements a custom Unix shell that supports basic command execution, background processes, and command history management. The shell demonstrates process creation, management, and inter-process communication concepts fundamental to operating systems.

## Features

- **Command Execution**: Execute standard Unix commands
- **Background Processes**: Run commands in background using '&' suffix
- **Command History**: Track and display previously executed commands
- **Built-in Commands**: 
  - `cd` - Change directory
  - `history` - Display command history
  - `exit` - Exit the shell
- **Process Management**: Handle child processes and zombie prevention
- **Signal Handling**: Proper cleanup of background processes

## File Structure

```
Assignment 1/
├── myshell.c       # Main shell implementation
├── README.md       # This file
├── submitters.txt  # Submission information
├── dry.pdf         # Theoretical questions
└── test units/     # Test cases directory
    ├── test1.in    # Input test file 1
    ├── test1.out   # Expected output 1
    ├── test2.in    # Input test file 2
    ├── test2.out   # Expected output 2
    ├── test3.in    # Input test file 3
    ├── test3.out   # Expected output 3
    ├── test5.in    # Input test file 5
    ├── test5.out   # Expected output 5
    ├── test6.in    # Input test file 6
    └── test6.out   # Expected output 6
```

## Usage

## Usage

### Compilation
Standard C compilation with appropriate libraries for system calls.

### Command Examples

The shell accepts and executes commands like:
```
myshell$ ls -l
myshell$ pwd
myshell$ cd /tmp
myshell$ ls &
myshell$ history
myshell$ exit
```

**Note**: This shell implementation is designed to run in a Linux/Unix environment where it can execute standard Unix commands like `ls`, `pwd`, `cat`, etc. The shell itself is a C program that manages process creation and command execution using system calls like `fork()`, `exec()`, and `wait()`.

## Implementation Details

### Core Components

1. **Command Parser**: Parses user input and tokenizes commands
2. **Process Creation**: Uses `fork()` and `exec()` family for command execution
3. **Background Process Handler**: Manages processes running in background
4. **History Manager**: Maintains list of executed commands with execution order
5. **Built-in Command Handler**: Implements shell built-in commands

### Data Structures

```c
typedef struct node {
    int execution_order;
    char command[BUFFER_SIZE];
    bool is_background;
} history_node;
```

### Key System Calls Used

- `fork()` - Create child processes
- `execvp()` - Execute commands
- `wait()` / `waitpid()` - Wait for child processes
- `chdir()` - Change directory (for cd command)
- `signal()` - Handle signals

### Background Process Management

- Commands ending with '&' are executed in background
- Parent process doesn't wait for background processes
- Zombie processes are prevented through proper signal handling
- Background process completion is tracked

### Command History

- Each command is assigned an execution order number
- History maintains both foreground and background commands
- Commands are stored in a linked list structure
- `history` command displays all previous commands with their execution order

## Built-in Commands

### `cd [directory]`
- Changes current working directory
- Supports both absolute and relative paths
- No arguments defaults to home directory

### `history`
- Displays all previously executed commands
- Shows execution order and command text
- Indicates if command was run in background

### `exit`
- Terminates the shell
- Performs cleanup of any remaining processes
- Frees allocated memory

## Testing

The `test units/` directory contains comprehensive test cases with input and expected output files. Testing involves comparing program output with expected results.

Test files include:
- **test1**: Basic command execution
- **test2**: Background process handling  
- **test3**: Built-in commands testing
- **test5**: Command history functionality
- **test6**: Complex scenarios and edge cases

## Key Learning Objectives

- Process creation and management in Unix/Linux
- Understanding of `fork()`, `exec()`, and `wait()` system calls
- Signal handling and zombie process prevention
- Command parsing and tokenization
- Background process management
- Memory management in C
- Linked list implementation and manipulation

## Error Handling

- Invalid commands: Display appropriate error messages
- Failed process creation: Handle `fork()` failures
- Memory allocation failures: Graceful degradation
- Signal handling: Proper cleanup on interruption

## Compilation Notes

- Requires POSIX-compliant system (Linux/Unix)
- Links with standard C library
- No external dependencies required
- Compatible with GCC

## Limitations

- Limited command line length (100 characters)
- No support for pipes or redirection
- No command line editing features
- Simple tokenization (space-separated only)