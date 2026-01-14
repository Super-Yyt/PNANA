# RISC-V Assembly Hello World Example
# Demonstrates RISC-V RV64I instruction set

    .data
# String constants
hello_msg:
    .string "Hello, World from RISC-V!\n"
name_msg:
    .string "RISC-V Programmer\n"
result_msg:
    .string "Result: %d\n"
fib_msg:
    .string "Fibonacci(%d) = %d\n"

# Global variables
    .globl global_counter
    .align 8
global_counter:
    .quad 0

    .text
    .globl main
    .type main, @function

# Main function
main:
    # Function prologue
    addi sp, sp, -32      # Allocate stack space
    sd ra, 24(sp)         # Save return address
    sd s0, 16(sp)         # Save frame pointer
    addi s0, sp, 32       # Set up frame pointer

    # Print hello message
    la a0, hello_msg      # Load address of hello message
    call puts             # Call puts function

    # Load and increment global counter
    la t0, global_counter # Load address of counter
    ld t1, 0(t0)          # Load counter value
    addi t1, t1, 1        # Increment counter
    sd t1, 0(t0)          # Store back to memory

    # Print counter value
    la a0, result_msg     # Load format string
    mv a1, t1             # Move counter to argument register
    call printf           # Call printf

    # Call custom functions
    call print_name       # Print name
    li a0, 5              # Argument for factorial
    call factorial        # Calculate 5!
    mv s1, a0             # Save result
    mv a0, s1             # Move result to print
    call print_number     # Print result

    # Calculate and print Fibonacci
    li a0, 10             # Calculate fib(10)
    call fibonacci
    mv s2, a0             # Save fib result
    la a0, fib_msg        # Load format string
    li a1, 10             # First argument
    mv a2, s2             # Second argument
    call printf

    # Demonstrate array operations
    call array_demo

    # Demonstrate conditional logic
    li a0, 42
    li a1, 24
    call max_function
    call print_number

    # Function epilogue
    ld ra, 24(sp)         # Restore return address
    ld s0, 16(sp)         # Restore frame pointer
    addi sp, sp, 32       # Deallocate stack space
    li a0, 0              # Return 0
    ret

# Function to print name
print_name:
    addi sp, sp, -16      # Allocate stack space
    sd ra, 8(sp)          # Save return address

    la a0, name_msg       # Load name message
    call puts             # Print it

    ld ra, 8(sp)          # Restore return address
    addi sp, sp, 16       # Deallocate stack space
    ret

# Recursive factorial function
factorial:
    addi sp, sp, -16      # Allocate stack space
    sd ra, 8(sp)          # Save return address
    sd s1, 0(sp)          # Save s1

    li t0, 1              # Load 1 for comparison
    ble a0, t0, factorial_base  # If n <= 1, return 1

    mv s1, a0             # Save n
    addi a0, a0, -1       # n - 1
    call factorial        # Recursive call
    mul a0, s1, a0        # n * factorial(n-1)
    j factorial_end

factorial_base:
    li a0, 1              # Return 1

factorial_end:
    ld ra, 8(sp)          # Restore return address
    ld s1, 0(sp)          # Restore s1
    addi sp, sp, 16       # Deallocate stack space
    ret

# Fibonacci function (iterative)
fibonacci:
    li t0, 0              # a = 0
    li t1, 1              # b = 1
    li t2, 1              # counter = 1

fib_loop:
    bge t2, a0, fib_done  # If counter >= n, done
    add t3, t0, t1        # next = a + b
    mv t0, t1             # a = b
    mv t1, t3             # b = next
    addi t2, t2, 1        # counter++
    j fib_loop

fib_done:
    mv a0, t0             # Return result
    ret

# Function to print a number
print_number:
    addi sp, sp, -16      # Allocate stack space
    sd ra, 8(sp)          # Save return address

    mv t0, a0             # Save number to print
    la a0, result_msg     # Load format string
    mv a1, t0             # Move number to argument
    call printf           # Print it

    ld ra, 8(sp)          # Restore return address
    addi sp, sp, 16       # Deallocate stack space
    ret

# Max function
max_function:
    bge a0, a1, max_done  # If a >= b, return a
    mv a0, a1             # Else return b
max_done:
    ret

# Array demonstration
array_demo:
    addi sp, sp, -16      # Allocate stack space
    sd ra, 8(sp)          # Save return address
    sd s1, 0(sp)          # Save s1

    # Create array on stack (5 elements)
    addi sp, sp, -40      # Allocate array space
    mv s1, sp             # Save array base address

    # Initialize array [1, 2, 3, 4, 5]
    li t0, 1
    sd t0, 0(s1)
    li t0, 2
    sd t0, 8(s1)
    li t0, 3
    sd t0, 16(s1)
    li t0, 4
    sd t0, 24(s1)
    li t0, 5
    sd t0, 32(s1)

    # Print array elements
    ld a0, 0(s1)
    call print_number
    ld a0, 8(s1)
    call print_number
    ld a0, 16(s1)
    call print_number
    ld a0, 24(s1)
    call print_number
    ld a0, 32(s1)
    call print_number

    # Calculate sum
    ld t0, 0(s1)
    ld t1, 8(s1)
    add t0, t0, t1
    ld t1, 16(s1)
    add t0, t0, t1
    ld t1, 24(s1)
    add t0, t0, t1
    ld t1, 32(s1)
    add t0, t0, t1

    mv a0, t0
    call print_number     # Print sum

    addi sp, sp, 40       # Deallocate array space
    ld ra, 8(sp)          # Restore return address
    ld s1, 0(sp)          # Restore s1
    addi sp, sp, 16       # Deallocate function space
    ret

# Loop demonstration
loop_demo:
    addi sp, sp, -16      # Allocate stack space
    sd ra, 8(sp)          # Save return address

    li t0, 0              # Loop counter
    li t1, 5              # Loop limit

loop_start:
    bge t0, t1, loop_end  # If counter >= limit, exit
    mv a0, t0             # Print current counter
    call print_number
    addi t0, t0, 1        # Increment counter
    j loop_start          # Loop back

loop_end:
    ld ra, 8(sp)          # Restore return address
    addi sp, sp, 16       # Deallocate stack space
    ret

# Bit manipulation demo
bit_demo:
    li a0, 0b10101010     # Binary literal
    li t0, 0b00001111     # Mask

    and a1, a0, t0        # AND operation
    mv a0, a1
    call print_number

    li a0, 0b10101010
    or a1, a0, t0         # OR operation
    mv a0, a1
    call print_number

    li a0, 0b10101010
    xor a1, a0, t0        # XOR operation
    mv a0, a1
    call print_number

    ret

    .size main, .-main
    .ident "RISC-V Assembly Example"
    .section .note.GNU-stack,"",@progbits
