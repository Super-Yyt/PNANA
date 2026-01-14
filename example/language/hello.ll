; LLVM IR Hello World Example
; Demonstrates LLVM Intermediate Representation syntax

; Module declaration
target triple = "x86_64-unknown-linux-gnu"

; External function declarations
declare i32 @printf(i8*, ...)
declare i32 @puts(i8*)

; Global string constants
@hello_str = private unnamed_addr constant [14 x i8] c"Hello, World!\00"
@format_str = private unnamed_addr constant [4 x i8] c"%d\0A\00"
@name_str = private unnamed_addr constant [16 x i8] c"LLVM Programmer\00"

; Global variables
@global_counter = global i32 0
@global_name = global i8* getelementptr inbounds ([16 x i8], [16 x i8]* @name_str, i32 0, i32 0)

; Function definition
define i32 @main() {
entry:
  ; Call puts to print hello world
  %hello_ptr = getelementptr inbounds [14 x i8], [14 x i8]* @hello_str, i32 0, i32 0
  call i32 @puts(i8* %hello_ptr)

  ; Load and print global counter
  %counter_val = load i32, i32* @global_counter
  %new_counter = add i32 %counter_val, 1
  store i32 %new_counter, i32* @global_counter

  ; Print the incremented counter
  call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @format_str, i32 0, i32 0), i32 %new_counter)

  ; Call our custom functions
  call void @print_name()
  %result = call i32 @factorial(i32 5)
  call void @print_number(i32 %result)

  ; Return 0
  ret i32 0
}

; Function to print name
define void @print_name() {
  %name_ptr = load i8*, i8** @global_name
  call i32 @puts(i8* %name_ptr)
  ret void
}

; Recursive factorial function
define i32 @factorial(i32 %n) {
entry:
  %cmp = icmp sle i32 %n, 1
  br i1 %cmp, label %base_case, label %recursive_case

base_case:
  ret i32 1

recursive_case:
  %n_minus_1 = sub i32 %n, 1
  %fact_n_minus_1 = call i32 @factorial(i32 %n_minus_1)
  %result = mul i32 %n, %fact_n_minus_1
  ret i32 %result
}

; Function to print a number
define void @print_number(i32 %num) {
  call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @format_str, i32 0, i32 0), i32 %num)
  ret void
}

; Function with conditional logic
define i32 @max(i32 %a, i32 %b) {
entry:
  %cmp = icmp sgt i32 %a, %b
  br i1 %cmp, label %return_a, label %return_b

return_a:
  ret i32 %a

return_b:
  ret i32 %b
}

; Function demonstrating memory operations
define void @memory_demo() {
  ; Allocate memory on stack
  %ptr = alloca i32
  store i32 42, i32* %ptr

  ; Load and modify
  %val = load i32, i32* %ptr
  %new_val = mul i32 %val, 2
  store i32 %new_val, i32* %ptr

  ; Print result
  call void @print_number(i32 %new_val)

  ret void
}

; Function with floating point operations
define double @circle_area(double %radius) {
entry:
  %pi = fadd double 3.14159, 0.0  ; Load pi value
  %radius_squared = fmul double %radius, %radius
  %area = fmul double %pi, %radius_squared
  ret double %area
}

; Structure definition (in LLVM IR, structures are defined by usage)
%Point = type { i32, i32 }
%Person = type { i8*, i32, i8* }

; Function using structures
define void @struct_demo() {
  ; Allocate a point structure
  %point_ptr = alloca %Point
  %x_ptr = getelementptr %Point, %Point* %point_ptr, i32 0, i32 0
  %y_ptr = getelementptr %Point, %Point* %point_ptr, i32 0, i32 1

  store i32 10, i32* %x_ptr
  store i32 20, i32* %y_ptr

  ; Load and print coordinates
  %x = load i32, i32* %x_ptr
  %y = load i32, i32* %y_ptr

  call void @print_number(i32 %x)
  call void @print_number(i32 %y)

  ret void
}

; Array operations
define void @array_demo() {
  ; Define and initialize array
  %arr = alloca [5 x i32]
  %arr0 = getelementptr [5 x i32], [5 x i32]* %arr, i32 0, i32 0
  %arr1 = getelementptr [5 x i32], [5 x i32]* %arr, i32 0, i32 1
  %arr2 = getelementptr [5 x i32], [5 x i32]* %arr, i32 0, i32 2
  %arr3 = getelementptr [5 x i32], [5 x i32]* %arr, i32 0, i32 3
  %arr4 = getelementptr [5 x i32], [5 x i32]* %arr, i32 0, i32 4

  store i32 1, i32* %arr0
  store i32 2, i32* %arr1
  store i32 3, i32* %arr2
  store i32 4, i32* %arr3
  store i32 5, i32* %arr4

  ; Print array elements
  %val0 = load i32, i32* %arr0
  %val1 = load i32, i32* %arr1
  %val2 = load i32, i32* %arr2
  %val3 = load i32, i32* %arr3
  %val4 = load i32, i32* %arr4

  call void @print_number(i32 %val0)
  call void @print_number(i32 %val1)
  call void @print_number(i32 %val2)
  call void @print_number(i32 %val3)
  call void @print_number(i32 %val4)

  ret void
}

; Function with switch statement
define void @switch_demo(i32 %value) {
entry:
  switch i32 %value, label %default [
    i32 1, label %case1
    i32 2, label %case2
    i32 3, label %case3
  ]

case1:
  call i32 @puts(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.case1, i32 0, i32 0))
  br label %exit

case2:
  call i32 @puts(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.case2, i32 0, i32 0))
  br label %exit

case3:
  call i32 @puts(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.case3, i32 0, i32 0))
  br label %exit

default:
  call i32 @puts(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @.str.default, i32 0, i32 0))
  br label %exit

exit:
  ret void
}

; String constants for switch demo
@.str.case1 = private unnamed_addr constant [6 x i8] c"One!\0A\00"
@.str.case2 = private unnamed_addr constant [6 x i8] c"Two!\0A\00"
@.str.case3 = private unnamed_addr constant [7 x i8] c"Three!\0A\00"
@.str.default = private unnamed_addr constant [8 x i8] c"Other!\0A\00"

; Metadata and attributes
!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"llvm-ir example"}
