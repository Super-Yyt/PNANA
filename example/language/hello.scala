// Scala 示例文件
// 展示 Scala 语言的各种语法特性

package com.example.scala

import scala.math.{Pi, sqrt}
import scala.util.{Try, Success, Failure}
import scala.collection.mutable.ListBuffer
import java.time.LocalDate
import java.time.format.DateTimeFormatter

// 全局常量
object Constants {
  val AppName = "Scala Example"
  val Version = "1.0.0"
  val Pi = scala.math.Pi
}

// 样例类（自动生成 equals, hashCode, toString, copy 方法）
case class Person(name: String, age: Int, email: Option[String] = None) {
  // 方法
  def greet(): String = s"Hello, I'm $name and I'm $age years old!"

  def isAdult: Boolean = age >= 18

  def celebrateBirthday(): Person = copy(age = age + 1)
}

// 枚举（Scala 3）或使用 sealed trait（Scala 2）
sealed trait Status
case object Pending extends Status
case object Approved extends Status
case object Rejected extends Status

// 特征（接口）
trait Describable {
  def description: String
}

trait Measurable {
  def area: Double
  def perimeter: Double
}

// 抽象类
abstract class Shape extends Describable with Measurable

// 具体实现
case class Circle(radius: Double) extends Shape {
  def area: Double = Pi * radius * radius
  def perimeter: Double = 2 * Pi * radius
  def description: String = s"Circle with radius $radius"
}

case class Rectangle(width: Double, height: Double) extends Shape {
  def area: Double = width * height
  def perimeter: Double = 2 * (width + height)
  def description: String = s"Rectangle ${width}x$height"
}

case class Triangle(a: Double, b: Double, c: Double) extends Shape {
  def area: Double = {
    val s = (a + b + c) / 2
    sqrt(s * (s - a) * (s - b) * (s - c))
  }
  def perimeter: Double = a + b + c
  def description: String = s"Triangle with sides $a, $b, $c"
}

// 泛型类
class Container[T] private (private val items: ListBuffer[T]) {

  def this() = this(ListBuffer[T]())

  def add(item: T): Unit = items += item

  def remove(item: T): Boolean = items.remove(items.indexOf(item))

  def get(index: Int): Option[T] = Try(items(index)).toOption

  def size: Int = items.size

  def isEmpty: Boolean = items.isEmpty

  def foreach(f: T => Unit): Unit = items.foreach(f)

  def filter(predicate: T => Boolean): List[T] = items.filter(predicate).toList

  def map[R](f: T => R): List[R] = items.map(f).toList

  def toList: List[T] = items.toList
}

object Container {
  def apply[T](): Container[T] = new Container[T]()
}

// 伴生对象和工厂方法
class Calculator private (private var result: Double) {

  def this() = this(0.0)

  def add(value: Double): Calculator = {
    result += value
    this
  }

  def multiply(value: Double): Calculator = {
    result *= value
    this
  }

  def divide(value: Double): Calculator = {
    if (value != 0) result /= value
    this
  }

  def getResult: Double = result

  def reset(): Calculator = {
    result = 0.0
    this
  }

  override def toString: String = s"Calculator(result = $result)"
}

object Calculator {
  def apply(): Calculator = new Calculator()

  // 递归函数
  def factorial(n: Int): Long = {
    if (n <= 1) 1
    else n * factorial(n - 1)
  }

  // Fibonacci 数列
  def fibonacci(n: Int): List[Int] = {
    def fibHelper(a: Int, b: Int, count: Int, acc: List[Int]): List[Int] = {
      if (count >= n) acc.reverse
      else fibHelper(b, a + b, count + 1, (a + b) :: acc)
    }

    if (n <= 0) Nil
    else if (n == 1) List(0)
    else fibHelper(0, 1, 1, List(0))
  }
}

// 隐式转换和扩展方法
object Extensions {
  implicit class StringExtensions(s: String) {
    def capitalizeWords: String =
      s.split(" ").map(_.capitalize).mkString(" ")

    def isBlank: Boolean = s.trim.isEmpty
  }

  implicit class IntExtensions(n: Int) {
    def isEven: Boolean = n % 2 == 0
    def squared: Int = n * n
    def cubed: Int = n * n * n
  }
}

// 高阶函数和函数式编程
object FunctionalUtils {
  def measureTime[T](block: => T): (T, Long) = {
    val start = System.nanoTime()
    val result = block
    val end = System.nanoTime()
    (result, end - start)
  }

  def processList[T, R](list: List[T])(f: T => R): List[R] = list.map(f)

  def compose[A, B, C](f: B => C, g: A => B): A => C = (a: A) => f(g(a))
}

// 模式匹配
object PatternMatching {
  def describeNumber(n: Int): String = n match {
    case 0 => "zero"
    case 1 | 2 | 3 => "small positive"
    case x if x < 0 => "negative"
    case x if x % 2 == 0 => "even"
    case _ => "other"
  }

  def describeShape(shape: Shape): String = shape match {
    case Circle(r) => s"Circle with radius $r"
    case Rectangle(w, h) => s"Rectangle ${w}x$h"
    case Triangle(a, b, c) => s"Triangle with sides $a, $b, $c"
  }
}

// 异常处理
case class ValidationException(message: String) extends Exception(message)

object Validator {
  def validateEmail(email: String): Boolean = {
    val emailRegex = """^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$""".r
    emailRegex.findFirstIn(email).isDefined
  }

  def validateAge(age: Int): Unit = {
    if (age < 0) throw ValidationException("Age cannot be negative")
    if (age > 150) throw ValidationException("Age cannot be greater than 150")
  }
}

// For-comprehension（增强的 for 循环）
object ForComprehensions {
  def cartesianProduct(list1: List[Int], list2: List[Int]): List[(Int, Int)] = {
    for {
      x <- list1
      y <- list2
      if x + y > 5
    } yield (x, y)
  }

  def processData(data: List[Person]): List[String] = {
    for {
      person <- data
      if person.isAdult
      description = s"${person.name} (${person.age})"
    } yield description
  }
}

// 主函数
object Main {
  def main(args: Array[String]): Unit = {
    println("=== Scala Example ===\n")

    // 导入扩展方法
    import Extensions._

    // 基本数据类型
    println("--- Basic Data Types ---")
    val string: String = "Hello, Scala World!"
    val number: Int = 42
    val float: Double = 3.14159
    val boolean: Boolean = true

    println(s"String: $string")
    println(s"Number: $number")
    println(s"Float: $float")
    println(s"Boolean: $boolean")
    println()

    // 集合操作
    println("--- Collections ---")
    val numbers = List(1, 2, 3, 4, 5, 6, 7, 8, 9, 10)
    val evenNumbers = numbers.filter(_ % 2 == 0)
    val squaredNumbers = numbers.map(_ * _)
    val sum = numbers.sum
    val average = numbers.sum.toDouble / numbers.length

    println(s"Original: $numbers")
    println(s"Even numbers: $evenNumbers")
    println(s"Squared: $squaredNumbers")
    println(f"Sum: $sum, Average: $average%.2f")
    println()

    // 样例类使用
    println("--- Case Classes ---")
    val person1 = Person("Alice", 30, Some("alice@example.com"))
    val person2 = Person("Bob", 25, None)

    println(s"Person 1: $person1")
    println(s"Person 2: $person2")
    println(s"Person 1 greeting: ${person1.greet()}")
    println(s"Is person 1 adult? ${person1.isAdult}")

    val olderPerson1 = person1.celebrateBirthday()
    println(s"After birthday: $olderPerson1")
    println()

    // 模式匹配
    println("--- Pattern Matching ---")
    val shapes = List(Circle(5.0), Rectangle(4.0, 6.0), Triangle(3.0, 4.0, 5.0))

    shapes.foreach { shape =>
      println(s"${PatternMatching.describeShape(shape)} -> Area: ${shape.area}")
    }
    println()

    // 泛型容器
    println("--- Generic Container ---")
    val stringContainer = Container[String]()
    stringContainer.add("Hello")
    stringContainer.add("World")
    stringContainer.add("Scala")

    println(s"Container size: ${stringContainer.size}")
    println("Container contents:")
    stringContainer.foreach(s => println(s"  $s"))

    val longWords = stringContainer.filter(_.length > 5)
    println(s"Long words: $longWords")

    val lengths = stringContainer.map(_.length)
    println(s"Word lengths: $lengths")
    println()

    // 扩展方法
    println("--- Extension Methods ---")
    val num = 42
    println(s"Is $num even? ${num.isEven}")
    println(s"$num squared: ${num.squared}")
    println(s"$num cubed: ${num.cubed}")

    val text = "hello world scala"
    println(s"Original: $text")
    println(s"Capitalized: ${text.capitalizeWords}")
    println(s"Is blank? ${text.isBlank}")
    println()

    // 计算器使用
    println("--- Calculator ---")
    val calc = Calculator()
      .add(10.0)
      .multiply(2.0)
      .add(5.0)

    println(s"Calculator result: ${calc.getResult}")

    println(s"Factorial of 5: ${Calculator.factorial(5)}")
    println(s"Fibonacci sequence: ${Calculator.fibonacci(10)}")
    println()

    // 高阶函数
    println("--- Higher-Order Functions ---")
    import FunctionalUtils._

    val (result, time) = measureTime {
      Thread.sleep(100) // 模拟耗时操作
      Calculator.fibonacci(20)
    }
    println(s"Fibonacci calculation took ${time / 1_000_000}ms")
    println(s"Result size: ${result.length}")

    val doubled = processList(List(1, 2, 3, 4, 5))(_ * 2)
    println(s"Doubled list: $doubled")

    val add5 = (x: Int) => x + 5
    val multiplyBy2 = (x: Int) => x * 2
    val composed = compose(multiplyBy2, add5)
    println(s"Composed function (multiplyBy2(add5(10))): ${composed(10)}")
    println()

    // For-comprehension
    println("--- For-Comprehensions ---")
    import ForComprehensions._

    val list1 = List(1, 2, 3, 4)
    val list2 = List(2, 3, 4, 5)
    val product = cartesianProduct(list1, list2)
    println(s"Cartesian product (sum > 5): $product")

    val people = List(person1, person2, Person("Charlie", 20))
    val adultDescriptions = processData(people)
    println(s"Adult descriptions: $adultDescriptions")
    println()

    // 异常处理
    println("--- Exception Handling ---")
    val emails = List("valid@example.com", "invalid-email", "another@valid.com")

    emails.foreach { email =>
      try {
        if (Validator.validateEmail(email)) {
          println(s"$email: Valid")
        } else {
          println(s"$email: Invalid")
        }
      } catch {
        case e: Exception => println(s"$email: Error - ${e.getMessage}")
      }
    }

    Try {
      Validator.validateAge(-5)
    } match {
      case Success(_) => println("Age validation passed")
      case Failure(e) => println(s"Age validation failed: ${e.getMessage}")
    }
    println()

    // 数字描述
    println("--- Number Descriptions ---")
    val testNumbers = List(-5, 0, 1, 2, 4, 15, 100)
    testNumbers.foreach { n =>
      println(s"$n is ${PatternMatching.describeNumber(n)}")
    }

    println("\n=== Scala Example Completed ===")
  }
}
