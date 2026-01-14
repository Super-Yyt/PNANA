# MIPS Assembly Hello World Example
# Demonstrates MIPS instruction set (32-bit)

    .data
# String constants
hello_msg:
    .asciiz "Hello, World from MIPS!\n"
name_msg:
    .asciiz "MIPS Programmer\n"
result_msg:
    .asciiz "Result: "
newline:
    .asciiz "\n"
fib_msg:
    .asciiz "Fibonacci("

# Global variables
    .globl global_counter
    .align 2
global_counter:
    .word 0

    .text
    .globl main
    .ent main

main:
    # Function prologue
    addiu $sp, $sp, -32    # Allocate stack space
    sw $ra, 28($sp)        # Save return address
    sw $fp, 24($sp)        # Save frame pointer
    move $fp, $sp          # Set up frame pointer

    # Print hello message
    li $v0, 4              # syscall code for print_string
    la $a0, hello_msg      # Load address of hello message
    syscall                # Make syscall

    # Load and increment global counter
    la $t0, global_counter # Load address of counter
    lw $t1, 0($t0)         # Load counter value
    addiu $t1, $t1, 1      # Increment counter
    sw $t1, 0($t0)         # Store back to memory

    # Print counter value
    li $v0, 4              # Print string
    la $a0, result_msg
    syscall

    li $v0, 1              # Print integer
    move $a0, $t1
    syscall

    li $v0, 4              # Print newline
    la $a0, newline
    syscall

    # Call custom functions
    jal print_name         # Print name

    li $a0, 5              # Argument for factorial
    jal factorial          # Calculate 5!
    move $s1, $v0          # Save result

    move $a0, $s1          # Move result to print
    jal print_number       # Print result

    # Calculate and print Fibonacci
    li $a0, 10             # Calculate fib(10)
    jal fibonacci
    move $s2, $v0          # Save fib result

    li $v0, 4              # Print "Fibonacci("
    la $a0, fib_msg
    syscall

    li $v0, 1              # Print 10
    li $a0, 10
    syscall

    li $v0, 11             # Print ")"
    li $a0, 41
    syscall

    li $v0, 11             # Print " = "
    li $a0, 61
    syscall

    li $v0, 11             # Print " "
    li $a0, 32
    syscall

    li $v0, 1              # Print fib result
    move $a0, $s2
    syscall

    li $v0, 4              # Print newline
    la $a0, newline
    syscall

    # Demonstrate array operations
    jal array_demo

    # Demonstrate conditional logic
    li $a0, 42
    li $a1, 24
    jal max_function
    move $a0, $v0
    jal print_number

    # Function epilogue
    move $sp, $fp          # Restore stack pointer
    lw $ra, 28($sp)        # Restore return address
    lw $fp, 24($sp)        # Restore frame pointer
    addiu $sp, $sp, 32     # Deallocate stack space
    li $v0, 0              # Return 0
    jr $ra                 # Return

# Function to print name
print_name:
    addiu $sp, $sp, -8     # Allocate stack space
    sw $ra, 4($sp)         # Save return address

    li $v0, 4              # Print string
    la $a0, name_msg
    syscall

    lw $ra, 4($sp)         # Restore return address
    addiu $sp, $sp, 8      # Deallocate stack space
    jr $ra

# Recursive factorial function
factorial:
    addiu $sp, $sp, -16    # Allocate stack space
    sw $ra, 12($sp)        # Save return address
    sw $s1, 8($sp)         # Save s1
    sw $a0, 4($sp)         # Save argument

    li $t0, 1              # Load 1 for comparison
    ble $a0, $t0, factorial_base  # If n <= 1, return 1

    move $s1, $a0          # Save n
    addiu $a0, $a0, -1     # n - 1
    jal factorial          # Recursive call
    mul $v0, $s1, $v0      # n * factorial(n-1)
    j factorial_end

factorial_base:
    li $v0, 1              # Return 1

factorial_end:
    lw $ra, 12($sp)        # Restore return address
    lw $s1, 8($sp)         # Restore s1
    lw $a0, 4($sp)         # Restore argument
    addiu $sp, $sp, 16     # Deallocate stack space
    jr $ra

# Fibonacci function (iterative)
fibonacci:
    li $t0, 0              # a = 0
    li $t1, 1              # b = 1
    li $t2, 1              # counter = 1

fib_loop:
    bge $t2, $a0, fib_done # If counter >= n, done
    addu $t3, $t0, $t1     # next = a + b
    move $t0, $t1          # a = b
    move $t1, $t3          # b = next
    addiu $t2, $t2, 1      # counter++
    j fib_loop

fib_done:
    move $v0, $t0          # Return result
    jr $ra

# Function to print a number
print_number:
    addiu $sp, $sp, -8     # Allocate stack space
    sw $ra, 4($sp)         # Save return address

    move $t0, $a0          # Save number to print
    li $v0, 4              # Print "Result: "
    la $a0, result_msg
    syscall

    li $v0, 1              # Print integer
    move $a0, $t0
    syscall

    li $v0, 4              # Print newline
    la $a0, newline
    syscall

    lw $ra, 4($sp)         # Restore return address
    addiu $sp, $sp, 8      # Deallocate stack space
    jr $ra

# Max function
max_function:
    bge $a0, $a1, max_done # If a >= b, return a
    move $a0, $a1          # Else return b
max_done:
    move $v0, $a0          # Return result
    jr $ra

# Array demonstration
array_demo:
    addiu $sp, $sp, -12    # Allocate stack space
    sw $ra, 8($sp)         # Save return address
    sw $s1, 4($sp)         # Save s1

    # Create array on stack (5 elements)
    addiu $sp, $sp, -20    # Allocate array space (5 words)
    move $s1, $sp          # Save array base address

    # Initialize array [1, 2, 3, 4, 5]
    li $t0, 1
    sw $t0, 0($s1)
    li $t0, 2
    sw $t0, 4($s1)
    li $t0, 3
    sw $t0, 8($s1)
    li $t0, 4
    sw $t0, 12($s1)
    li $t0, 5
    sw $t0, 16($s1)

    # Print array elements
    lw $a0, 0($s1)
    jal print_number
    lw $a0, 4($s1)
    jal print_number
    lw $a0, 8($s1)
    jal print_number
    lw $a0, 12($s1)
    jal print_number
    lw $a0, 16($s1)
    jal print_number

    # Calculate sum
    lw $t0, 0($s1)
    lw $t1, 4($s1)
    addu $t0, $t0, $t1
    lw $t1, 8($s1)
    addu $t0, $t0, $t1
    lw $t1, 12($s1)
    addu $t0, $t0, $t1
    lw $t1, 16($s1)
    addu $t0, $t0, $t1

    move $a0, $t0
    jal print_number       # Print sum

    addiu $sp, $sp, 20     # Deallocate array space
    lw $ra, 8($sp)         # Restore return address
    lw $s1, 4($sp)         # Restore s1
    addiu $sp, $sp, 12     # Deallocate function space
    jr $ra

# Loop demonstration
loop_demo:
    addiu $sp, $sp, -8     # Allocate stack space
    sw $ra, 4($sp)         # Save return address

    li $t0, 0              # Loop counter
    li $t1, 5              # Loop limit

loop_start:
    bge $t0, $t1, loop_end # If counter >= limit, exit
    move $a0, $t0          # Print current counter
    jal print_number
    addiu $t0, $t0, 1      # Increment counter
    j loop_start           # Loop back

loop_end:
    lw $ra, 4($sp)         # Restore return address
    addiu $sp, $sp, 8      # Deallocate stack space
    jr $ra

# Bit manipulation demo
bit_demo:
    li $a0, 0xAA           # Hex literal (10101010)
    li $t0, 0x0F           # Mask (00001111)

    and $a1, $a0, $t0      # AND operation
    move $a0, $a1
    jal print_number

    li $a0, 0xAA
    or $a1, $a0, $t0       # OR operation
    move $a0, $a1
    jal print_number

    li $a0, 0xAA
    xor $a1, $a0, $t0      # XOR operation
    move $a0, $a1
    jal print_number

    jr $ra

    .end main
    .size main, .-main
