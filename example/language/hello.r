# R 语言示例文件
# 展示 R 语言的各种语法特性

# 基本数据类型
numeric_var <- 3.14159
integer_var <- 42L
character_var <- "Hello, R World!"
logical_var <- TRUE
complex_var <- 3 + 4i

# 向量
numeric_vector <- c(1, 2, 3, 4, 5)
character_vector <- c("apple", "banana", "cherry")
logical_vector <- c(TRUE, FALSE, TRUE, FALSE)

# 矩阵
matrix_data <- matrix(1:12, nrow = 3, ncol = 4)
rownames(matrix_data) <- c("Row1", "Row2", "Row3")
colnames(matrix_data) <- c("Col1", "Col2", "Col3", "Col4")

# 数据框
student_data <- data.frame(
    name = c("Alice", "Bob", "Charlie", "Diana"),
    age = c(20, 21, 19, 22),
    grade = c(85.5, 92.0, 78.3, 88.7),
    passed = c(TRUE, TRUE, FALSE, TRUE)
)

# 列表
mixed_list <- list(
    numbers = c(1, 2, 3, 4, 5),
    names = c("Alice", "Bob", "Charlie"),
    matrix = matrix_data,
    dataframe = student_data,
    function_example = function(x) x^2
)

# 函数定义
greet <- function(name) {
    paste("Hello,", name, "!")
}

# 函数带默认参数
calculate_area <- function(shape, radius = 1, width = 1, height = 1) {
    if (shape == "circle") {
        return(pi * radius^2)
    } else if (shape == "rectangle") {
        return(width * height)
    } else if (shape == "triangle") {
        return(width * height / 2)
    } else {
        stop("Unknown shape")
    }
}

# 递归函数
factorial <- function(n) {
    if (n <= 1) {
        return(1)
    } else {
        return(n * factorial(n - 1))
    }
}

# Fibonacci 数列
fibonacci <- function(n) {
    if (n <= 0) return(c())
    if (n == 1) return(c(0))
    if (n == 2) return(c(0, 1))

    sequence <- c(0, 1)
    for (i in 3:n) {
        sequence <- c(sequence, sequence[i-1] + sequence[i-2])
    }
    return(sequence)
}

# 向量化操作
square_numbers <- function(nums) {
    return(nums^2)
}

# 数据处理函数
analyze_data <- function(data) {
    result <- list(
        mean = mean(data),
        median = median(data),
        sd = sd(data),
        min = min(data),
        max = max(data),
        range = range(data)
    )
    return(result)
}

# 条件语句
classify_number <- function(x) {
    if (x < 0) {
        return("negative")
    } else if (x == 0) {
        return("zero")
    } else if (x < 10) {
        return("small")
    } else if (x < 100) {
        return("medium")
    } else {
        return("large")
    }
}

# 循环
print_numbers <- function(n) {
    cat("For loop:\n")
    for (i in 1:n) {
        cat("  ", i, "\n")
    }

    cat("While loop:\n")
    i <- 1
    while (i <= n) {
        cat("  ", i, "\n")
        i <- i + 1
    }

    cat("Repeat loop:\n")
    i <- 1
    repeat {
        cat("  ", i, "\n")
        i <- i + 1
        if (i > n) break
    }
}

# 向量化和函数式编程
process_vector <- function(vec) {
    # 使用 sapply 进行函数式编程
    squared <- sapply(vec, function(x) x^2)
    even <- vec[vec %% 2 == 0]
    greater_than_5 <- vec[vec > 5]

    result <- list(
        original = vec,
        squared = squared,
        even_numbers = even,
        greater_than_5 = greater_than_5,
        sum = sum(vec),
        mean = mean(vec),
        length = length(vec)
    )

    return(result)
}

# 数据框操作
analyze_students <- function(students) {
    cat("Student Analysis:\n")
    cat("Total students:", nrow(students), "\n")
    cat("Average age:", mean(students$age), "\n")
    cat("Average grade:", mean(students$grade), "\n")
    cat("Passed students:", sum(students$passed), "\n")

    # 按成绩排序
    sorted_by_grade <- students[order(students$grade, decreasing = TRUE), ]
    cat("\nTop 3 students by grade:\n")
    print(head(sorted_by_grade, 3))

    # 成绩统计
    grade_stats <- summary(students$grade)
    cat("\nGrade statistics:\n")
    print(grade_stats)

    return(sorted_by_grade)
}

# 绘图函数（如果可用）
create_plot <- function() {
    if (capabilities("png")) {
        png("r_example_plot.png")
        plot(student_data$age, student_data$grade,
             xlab = "Age", ylab = "Grade",
             main = "Student Age vs Grade",
             pch = 19, col = "blue")
        abline(lm(grade ~ age, data = student_data), col = "red")
        dev.off()
        cat("Plot saved as r_example_plot.png\n")
    } else {
        cat("PNG graphics not available\n")
    }
}

# 错误处理
safe_divide <- function(a, b) {
    tryCatch({
        if (b == 0) {
            stop("Division by zero")
        }
        return(a / b)
    }, error = function(e) {
        cat("Error:", conditionMessage(e), "\n")
        return(NA)
    })
}

# 主程序
main <- function() {
    cat("=== R Language Example ===\n\n")

    # 基本输出
    cat("Basic Data Types:\n")
    cat("Numeric:", numeric_var, "\n")
    cat("Integer:", integer_var, "\n")
    cat("Character:", character_var, "\n")
    cat("Logical:", logical_var, "\n")
    cat("Complex:", complex_var, "\n\n")

    # 向量操作
    cat("Vector Operations:\n")
    cat("Numeric vector:", numeric_vector, "\n")
    cat("Sum:", sum(numeric_vector), "\n")
    cat("Mean:", mean(numeric_vector), "\n")
    cat("Standard deviation:", sd(numeric_vector), "\n\n")

    # 函数调用
    cat("Function Calls:\n")
    cat(greet("Alice"), "\n")
    cat("Circle area (r=5):", calculate_area("circle", radius = 5), "\n")
    cat("Rectangle area (4x6):", calculate_area("rectangle", width = 4, height = 6), "\n")
    cat("Factorial of 5:", factorial(5), "\n")
    cat("Fibonacci sequence:", fibonacci(10), "\n\n")

    # 向量化和函数式编程
    cat("Vector Processing:\n")
    vec_result <- process_vector(c(1, 2, 3, 4, 5, 6, 7, 8, 9, 10))
    cat("Squared:", vec_result$squared, "\n")
    cat("Even numbers:", vec_result$even_numbers, "\n")
    cat("Sum:", vec_result$sum, "\n\n")

    # 数据框操作
    sorted_students <- analyze_students(student_data)

    # 矩阵操作
    cat("Matrix Operations:\n")
    print(matrix_data)
    cat("Matrix sum:", sum(matrix_data), "\n")
    cat("Matrix mean:", mean(matrix_data), "\n")
    cat("Row sums:", rowSums(matrix_data), "\n")
    cat("Column sums:", colSums(matrix_data), "\n\n")

    # 列表操作
    cat("List Access:\n")
    cat("Numbers from list:", mixed_list$numbers, "\n")
    cat("Names from list:", mixed_list$names, "\n")
    cat("Function call on list item:", mixed_list$function_example(5), "\n\n")

    # 条件语句和循环演示
    cat("Conditional and Loop Examples:\n")
    print_numbers(3)

    cat("\nNumber Classifications:\n")
    test_numbers <- c(-5, 0, 3, 25, 150)
    for (num in test_numbers) {
        cat(num, "is", classify_number(num), "\n")
    }

    # 错误处理
    cat("\nError Handling:\n")
    cat("10 / 2 =", safe_divide(10, 2), "\n")
    cat("10 / 0 =", safe_divide(10, 0), "\n")

    # 统计分析
    cat("\nStatistical Analysis:\n")
    stats <- analyze_data(student_data$grade)
    cat("Grade statistics:\n")
    cat("  Mean:", stats$mean, "\n")
    cat("  Median:", stats$median, "\n")
    cat("  Standard deviation:", stats$sd, "\n")
    cat("  Range:", paste(stats$range, collapse = " to "), "\n")

    # 尝试绘图
    cat("\nPlotting:\n")
    create_plot()

    cat("\n=== R Language Example Completed ===\n")
}

# 如果是直接运行此脚本
if (!exists("RStudio") && !exists("rstudioapi")) {
    main()
}
