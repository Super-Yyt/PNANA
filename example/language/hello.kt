// Kotlin 示例文件
// 展示 Kotlin 语言的各种语法特性

// 包声明
package com.example.kotlin

import java.time.LocalDate
import java.time.format.DateTimeFormatter
import kotlin.math.PI
import kotlin.math.sqrt

// 全局常量
const val APP_NAME = "Kotlin Example"
const val VERSION = "1.0.0"

// 数据类
data class Person(
    val name: String,
    val age: Int,
    val email: String? = null
) {
    // 计算属性
    val isAdult: Boolean
        get() = age >= 18

    // 方法
    fun greet(): String = "Hello, I'm $name and I'm $age years old!"

    fun celebrateBirthday(): Person {
        return copy(age = age + 1)
    }
}

// 枚举类
enum class Status(val displayName: String) {
    PENDING("Pending Review"),
    APPROVED("Approved"),
    REJECTED("Rejected");

    fun getDescription(): String = "Status: $displayName"
}

// 密封类
sealed class Shape {
    abstract fun area(): Double
    abstract fun perimeter(): Double
}

data class Circle(val radius: Double) : Shape() {
    override fun area(): Double = PI * radius * radius
    override fun perimeter(): Double = 2 * PI * radius
}

data class Rectangle(val width: Double, val height: Double) : Shape() {
    override fun area(): Double = width * height
    override fun perimeter(): Double = 2 * (width + height)
}

data class Triangle(val a: Double, val b: Double, val c: Double) : Shape() {
    override fun area(): Double {
        val s = (a + b + c) / 2
        return sqrt(s * (s - a) * (s - b) * (s - c))
    }

    override fun perimeter(): Double = a + b + c
}

// 接口
interface Drawable {
    fun draw()
    val color: String
}

interface Measurable {
    fun measure(): Double
}

// 实现多个接口的类
class ColoredShape(
    private val shape: Shape,
    override val color: String
) : Shape(), Drawable, Measurable {

    override fun area(): Double = shape.area()
    override fun perimeter(): Double = shape.perimeter()

    override fun draw() {
        println("Drawing a $color ${shape::class.simpleName} with area ${area()}")
    }

    override fun measure(): Double = area()
}

// 泛型类
class Container<T>(
    private val items: MutableList<T> = mutableListOf()
) {
    fun add(item: T) {
        items.add(item)
    }

    fun remove(item: T): Boolean {
        return items.remove(item)
    }

    fun get(index: Int): T? {
        return items.getOrNull(index)
    }

    fun size(): Int = items.size

    fun isEmpty(): Boolean = items.isEmpty()

    fun forEach(action: (T) -> Unit) {
        items.forEach(action)
    }

    fun filter(predicate: (T) -> Boolean): List<T> {
        return items.filter(predicate)
    }

    fun <R> map(transform: (T) -> R): List<R> {
        return items.map(transform)
    }
}

// 扩展函数
fun Int.isEven(): Boolean = this % 2 == 0

fun String.capitalizeWords(): String =
    split(" ").joinToString(" ") { it.replaceFirstChar { char -> char.uppercase() } }

// 内联函数和高阶函数
inline fun <T> measureTime(block: () -> T): Pair<T, Long> {
    val start = System.nanoTime()
    val result = block()
    val end = System.nanoTime()
    return Pair(result, end - start)
}

// 运算符重载
data class Vector2D(val x: Double, val y: Double) {
    operator fun plus(other: Vector2D): Vector2D {
        return Vector2D(x + other.x, y + other.y)
    }

    operator fun times(scalar: Double): Vector2D {
        return Vector2D(x * scalar, y * scalar)
    }

    fun length(): Double = sqrt(x * x + y * y)
}

// 伴生对象（静态成员）
class Calculator {
    companion object {
        fun factorial(n: Int): Long {
            return if (n <= 1) 1 else n * factorial(n - 1)
        }

        fun fibonacci(n: Int): List<Int> {
            if (n <= 0) return emptyList()
            if (n == 1) return listOf(0)

            val sequence = mutableListOf(0, 1)
            for (i in 2 until n) {
                sequence.add(sequence[i-1] + sequence[i-2])
            }
            return sequence
        }
    }

    private var result: Double = 0.0

    fun add(value: Double): Calculator {
        result += value
        return this
    }

    fun multiply(value: Double): Calculator {
        result *= value
        return this
    }

    fun getResult(): Double = result

    fun reset(): Calculator {
        result = 0.0
        return this
    }
}

// 异常处理
class ValidationException(message: String) : Exception(message)

object Validator {
    fun validateEmail(email: String): Boolean {
        val emailRegex = Regex("^[A-Za-z0-9+_.-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$")
        return emailRegex.matches(email)
    }

    fun validateAge(age: Int) {
        if (age < 0) {
            throw ValidationException("Age cannot be negative")
        }
        if (age > 150) {
            throw ValidationException("Age cannot be greater than 150")
        }
    }
}

// DSL 示例
class HTMLBuilder {
    private val elements = mutableListOf<String>()

    fun html(init: HTMLBuilder.() -> Unit): HTMLBuilder {
        elements.add("<html>")
        init()
        elements.add("</html>")
        return this
    }

    fun body(init: HTMLBuilder.() -> Unit): HTMLBuilder {
        elements.add("<body>")
        init()
        elements.add("</body>")
        return this
    }

    fun h1(text: String): HTMLBuilder {
        elements.add("<h1>$text</h1>")
        return this
    }

    fun p(text: String): HTMLBuilder {
        elements.add("<p>$text</p>")
        return this
    }

    fun build(): String = elements.joinToString("\n")
}

fun html(init: HTMLBuilder.() -> Unit): String {
    return HTMLBuilder().apply(init).build()
}

// 主函数
fun main() {
    println("=== Kotlin Example ===\n")

    // 基本数据类型
    println("--- Basic Data Types ---")
    val string: String = "Hello, Kotlin World!"
    val number: Int = 42
    val float: Double = 3.14159
    val boolean: Boolean = true

    println("String: $string")
    println("Number: $number")
    println("Float: $float")
    println("Boolean: $boolean")
    println()

    // 集合操作
    println("--- Collections ---")
    val numbers = listOf(1, 2, 3, 4, 5, 6, 7, 8, 9, 10)
    val evenNumbers = numbers.filter { it % 2 == 0 }
    val squaredNumbers = numbers.map { it * it }
    val sum = numbers.sum()
    val average = numbers.average()

    println("Original: $numbers")
    println("Even numbers: $evenNumbers")
    println("Squared: $squaredNumbers")
    println("Sum: $sum, Average: $average")
    println()

    // 数据类使用
    println("--- Data Classes ---")
    val person1 = Person("Alice", 30, "alice@example.com")
    val person2 = Person("Bob", 25)

    println("Person 1: $person1")
    println("Person 2: $person2")
    println("Person 1 greeting: ${person1.greet()}")
    println("Is person 1 adult? ${person1.isAdult}")

    val olderPerson1 = person1.celebrateBirthday()
    println("After birthday: $olderPerson1")
    println()

    // 枚举使用
    println("--- Enums ---")
    val status = Status.APPROVED
    println("Status: ${status.getDescription()}")
    println()

    // 密封类和模式匹配
    println("--- Sealed Classes and Pattern Matching ---")
    val shapes = listOf(
        Circle(5.0),
        Rectangle(4.0, 6.0),
        Triangle(3.0, 4.0, 5.0)
    )

    shapes.forEach { shape ->
        when (shape) {
            is Circle -> println("Circle with radius ${shape.radius}: area = ${shape.area()}")
            is Rectangle -> println("Rectangle ${shape.width}x${shape.height}: area = ${shape.area()}")
            is Triangle -> println("Triangle with sides ${shape.a}, ${shape.b}, ${shape.c}: area = ${shape.area()}")
        }
    }
    println()

    // 泛型容器
    println("--- Generic Container ---")
    val stringContainer = Container<String>()
    stringContainer.add("Hello")
    stringContainer.add("World")
    stringContainer.add("Kotlin")

    println("Container size: ${stringContainer.size()}")
    println("Container contents:")
    stringContainer.forEach { println("  $it") }

    val longWords = stringContainer.filter { it.length > 5 }
    println("Long words: $longWords")

    val lengths = stringContainer.map { it.length }
    println("Word lengths: $lengths")
    println()

    // 扩展函数
    println("--- Extension Functions ---")
    val num = 42
    println("Is $num even? ${num.isEven()}")

    val text = "hello world kotlin"
    println("Original: $text")
    println("Capitalized: ${text.capitalizeWords()}")
    println()

    // 高阶函数和内联函数
    println("--- Higher-Order Functions ---")
    val (result, time) = measureTime {
        Thread.sleep(100) // 模拟耗时操作
        Calculator.fibonacci(20)
    }
    println("Fibonacci calculation took ${time / 1_000_000}ms")
    println("Result size: ${result.size}")
    println()

    // 运算符重载
    println("--- Operator Overloading ---")
    val v1 = Vector2D(1.0, 2.0)
    val v2 = Vector2D(3.0, 4.0)
    val sum = v1 + v2
    val scaled = v1 * 2.0

    println("Vector 1: (${v1.x}, ${v1.y})")
    println("Vector 2: (${v2.x}, ${v2.y})")
    println("Sum: (${sum.x}, ${sum.y})")
    println("Scaled: (${scaled.x}, ${scaled.y})")
    println("Sum length: ${sum.length()}")
    println()

    // 计算器使用
    println("--- Calculator ---")
    val calc = Calculator()
        .add(10.0)
        .multiply(2.0)
        .add(5.0)

    println("Calculator result: ${calc.getResult()}")

    println("Factorial of 5: ${Calculator.factorial(5)}")
    println("Fibonacci sequence: ${Calculator.fibonacci(10)}")
    println()

    // 异常处理
    println("--- Exception Handling ---")
    val emails = listOf("valid@example.com", "invalid-email", "another@valid.com")

    emails.forEach { email ->
        try {
            if (Validator.validateEmail(email)) {
                println("$email: Valid")
            } else {
                println("$email: Invalid")
            }
        } catch (e: Exception) {
            println("$email: Error - ${e.message}")
        }
    }

    try {
        Validator.validateAge(-5)
    } catch (e: ValidationException) {
        println("Age validation error: ${e.message}")
    }
    println()

    // DSL 使用
    println("--- DSL Example ---")
    val html = html {
        html {
            body {
                h1("Kotlin DSL Example")
                p("This is a paragraph created using Kotlin DSL")
                p("Kotlin makes creating DSLs easy and readable")
            }
        }
    }

    println("Generated HTML:")
    println(html)
    println()

    // 范围和循环
    println("--- Ranges and Loops ---")
    println("For loop with range:")
    for (i in 1..5) {
        print("$i ")
    }
    println()

    println("For loop with step:")
    for (i in 0..10 step 2) {
        print("$i ")
    }
    println()

    println("For loop with collection:")
    val fruits = listOf("apple", "banana", "cherry")
    for (fruit in fruits) {
        print("$fruit ")
    }
    println()

    // When 表达式（增强的 switch）
    println("\n--- When Expression ---")
    val scores = listOf(45, 67, 89, 92, 78)
    scores.forEach { score ->
        val grade = when {
            score >= 90 -> "A"
            score >= 80 -> "B"
            score >= 70 -> "C"
            score >= 60 -> "D"
            else -> "F"
        }
        println("Score $score = Grade $grade")
    }

    println("\n=== Kotlin Example Completed ===")
}

// 顶层函数示例
fun utilityFunction(): String {
    return "This is a utility function"
}

// 扩展属性
val String.isBlank: Boolean
    get() = all { it.isWhitespace() }
