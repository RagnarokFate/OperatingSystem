# Assignment 0: Simple Calculator

## Overview

This assignment implements a basic command-line calculator in C. The program performs simple arithmetic operations on single-digit numbers and demonstrates fundamental C programming concepts including input handling, string processing, and basic arithmetic operations.

## Features

- **Basic Operations**: Addition (+), Subtraction (-), Multiplication (*), Division (/)
- **Input Validation**: Checks for proper format and valid single-digit numbers
- **Exit Command**: Type "exit" to quit the program
- **Error Handling**: Invalid input format results in error messages

## File Structure

```
Assignment 0/
├── calc.c          # Main calculator implementation
├── README.md       # This file
└── submitters.txt  # Submission information
```

## Usage

### Compilation
Standard C compilation with any C compiler.

### Input Format

The program expects input in the following format:
```
<digit> <space> <operator> <space> <digit>
```

**Examples:**
```
3 + 5
7 - 2
4 * 6
8 / 2
exit
```

### Sample Session

```
5 + 3
8
9 - 4
5
6 * 7
42
8 / 2
4
invalid input
2 * 3
6
exit
```

## Implementation Details

### Input Validation

- Checks that first and last characters are single digits (0-9)
- Validates that positions 1 and 3 contain spaces
- Ensures position 2 contains a valid operator (+, -, *, /)
- Handles division by zero

### Error Handling

- Invalid format: "invalid input"
- Division by zero: Appropriate error message
- Non-digit characters in number positions

### Program Flow

1. Read input line using `fgets()`
2. Check if input is "exit" command
3. Parse and validate input format
4. Extract operands and operator
5. Perform calculation
6. Display result or error message
7. Continue until "exit" is entered

## Key Learning Objectives

- Basic C input/output operations
- String manipulation and character processing
- Input validation techniques
- Error handling in C programs
- ASCII to integer conversion
- Basic arithmetic operations implementation

## Compilation Notes

- Standard C library functions used
- No external dependencies required
- Compatible with GCC and most C compilers
- Follows C99 standard

## Limitations

- Only supports single-digit numbers (0-9)
- No support for floating-point arithmetic
- No support for parentheses or operator precedence
- Fixed input format required