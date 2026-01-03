// Swift 示例文件
// 展示 Swift 的各种语法特性

import Foundation

// 全局常量和变量
let appName = "Swift Example"
let version = "1.0.0"
var globalCounter = 0

// 枚举
enum Status {
    case pending
    case approved
    case rejected

    var description: String {
        switch self {
        case .pending:
            return "Pending Review"
        case .approved:
            return "Approved"
        case .rejected:
            return "Rejected"
        }
    }
}

enum Result<T, Error> {
    case success(T)
    case failure(Error)
}

// 协议（Protocol）
protocol Describable {
    var description: String { get }
}

protocol Logger {
    func log(_ message: String)
    func error(_ message: String)
}

// 结构体
struct Point: Describable {
    var x: Double
    var y: Double

    var description: String {
        return "Point(x: \(x), y: \(y))"
    }

    func distance(to other: Point) -> Double {
        let dx = x - other.x
        let dy = y - other.y
        return sqrt(dx * dx + dy * dy)
    }
}

struct Size {
    var width: Double
    var height: Double
}

// 类
class Person: Describable {
    // 属性
    let id: UUID
    var name: String
    var age: Int
    var email: String?

    // 计算属性
    var isAdult: Bool {
        return age >= 18
    }

    // 懒加载属性
    lazy var displayName: String = {
        return "\(name) (\(age))"
    }()

    // 初始化器
    init(name: String, age: Int, email: String? = nil) {
        self.id = UUID()
        self.name = name
        self.age = age
        self.email = email
    }

    // 便利初始化器
    convenience init(name: String) {
        self.init(name: name, age: 0, email: nil)
    }

    // 方法
    func greet() -> String {
        return "Hello, my name is \(name)"
    }

    func celebrateBirthday() -> String {
        age += 1
        return "Happy birthday! You're now \(age) years old"
    }

    // 协议实现
    var description: String {
        return "Person: \(name), Age: \(age)"
    }
}

// 继承
class Employee: Person {
    var position: String
    var salary: Double

    init(name: String, age: Int, position: String, salary: Double, email: String? = nil) {
        self.position = position
        self.salary = salary
        super.init(name: name, age: age, email: email)
    }

    override func greet() -> String {
        return "Hello, I'm \(name), a \(position)"
    }

    func getAnnualSalary() -> Double {
        return salary * 12
    }
}

// 扩展（Extensions）
extension Double {
    var squared: Double {
        return self * self
    }

    var cubed: Double {
        return self * self * self
    }
}

extension Array where Element: Describable {
    func describeAll() -> String {
        return self.map { $0.description }.joined(separator: "\n")
    }
}

// 泛型
struct Stack<Element> {
    private var elements: [Element] = []

    mutating func push(_ element: Element) {
        elements.append(element)
    }

    mutating func pop() -> Element? {
        return elements.popLast()
    }

    func peek() -> Element? {
        return elements.last
    }

    var isEmpty: Bool {
        return elements.isEmpty
    }

    var count: Int {
        return elements.count
    }
}

// 错误处理
enum CalculatorError: Error {
    case divisionByZero
    case invalidInput(String)
}

class Calculator {
    private var result: Double = 0

    func add(_ number: Double) {
        result += number
    }

    func subtract(_ number: Double) {
        result -= number
    }

    func multiply(_ number: Double) {
        result *= number
    }

    func divide(_ number: Double) throws {
        guard number != 0 else {
            throw CalculatorError.divisionByZero
        }
        result /= number
    }

    func getResult() -> Double {
        return result
    }

    func reset() {
        result = 0
    }
}

// 闭包和函数
func fibonacci(_ n: Int) -> [Int] {
    guard n > 0 else { return [] }
    if n == 1 { return [0] }

    var sequence = [0, 1]
    for i in 2..<n {
        sequence.append(sequence[i-1] + sequence[i-2])
    }
    return sequence
}

func processArray(_ array: [Int], using transform: (Int) -> Int) -> [Int] {
    return array.map(transform)
}

func createMultiplier(_ factor: Int) -> (Int) -> Int {
    return { number in
        return number * factor
    }
}

// 操作符重载
struct Vector2D {
    var x: Double
    var y: Double

    static func + (left: Vector2D, right: Vector2D) -> Vector2D {
        return Vector2D(x: left.x + right.x, y: left.y + right.y)
    }

    static func * (vector: Vector2D, scalar: Double) -> Vector2D {
        return Vector2D(x: vector.x * scalar, y: vector.y * scalar)
    }
}

// 模式匹配和guard语句
func describeNumber(_ number: Int) -> String {
    switch number {
    case 0:
        return "zero"
    case 1...9:
        return "single digit"
    case 10...99:
        return "double digits"
    case let x where x % 2 == 0:
        return "even number"
    default:
        return "other number"
    }
}

// 主函数
func main() {
    print("=== Swift Example ===")

    // 基本数据类型
    print("\n--- Basic Data Types ---")
    let string: String = "Hello World"
    let integer: Int = 42
    let float: Double = 3.14159
    let boolean: Bool = true
    let array: [Int] = [1, 2, 3, 4, 5]
    let dictionary: [String: Any] = ["name": "Swift", "version": "5.7", "awesome": true]

    print("String: \(string)")
    print("Integer: \(integer)")
    print("Float: \(float)")
    print("Boolean: \(boolean)")
    print("Array: \(array)")
    print("Dictionary: \(dictionary)")

    // 可选类型
    print("\n--- Optionals ---")
    let optionalString: String? = "Optional value"
    let nilString: String? = nil

    if let unwrapped = optionalString {
        print("Optional contains: \(unwrapped)")
    }

    let defaultValue = nilString ?? "Default value"
    print("Nil coalescing result: \(defaultValue)")

    // 字符串插值和操作
    print("\n--- String Operations ---")
    let name = "Alice"
    let age = 30
    let message = "My name is \(name) and I am \(age) years old"
    print(message)
    print("String length: \(string.count)")
    print("Uppercase: \(string.uppercased())")
    print("Reversed: \(String(string.reversed()))")

    // 数组操作
    print("\n--- Array Operations ---")
    let numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
    let evenNumbers = numbers.filter { $0 % 2 == 0 }
    let squaredNumbers = numbers.map { $0 * $0 }
    let sum = numbers.reduce(0, +)

    print("Original: \(numbers)")
    print("Even numbers: \(evenNumbers)")
    print("Squared: \(squaredNumbers)")
    print("Sum: \(sum)")

    // 条件语句
    print("\n--- Conditional Statements ---")
    let score = 85

    let grade: String
    if score >= 90 {
        grade = "A"
    } else if score >= 80 {
        grade = "B"
    } else if score >= 70 {
        grade = "C"
    } else {
        grade = "F"
    }

    print("Score: \(score), Grade: \(grade)")

    // 循环
    print("\n--- Loops ---")
    print("For-in loop:")
    for i in 1...5 {
        print("  Iteration \(i)")
    }

    print("For-in with array:")
    for fruit in ["apple", "banana", "cherry"] {
        print("  I like \(fruit)")
    }

    print("While loop:")
    var counter = 1
    while counter <= 3 {
        print("  Count: \(counter)")
        counter += 1
    }

    // 类和对象
    print("\n--- Classes and Objects ---")
    let person = Person(name: "Charlie", age: 28, email: "charlie@example.com")
    let employee = Employee(name: "Diana", age: 35, position: "Developer", salary: 75000)

    print(person.greet())
    print(employee.greet())
    print("Employee annual salary: \(employee.getAnnualSalary())")

    // 结构体使用
    let point1 = Point(x: 0, y: 0)
    let point2 = Point(x: 3, y: 4)
    print("Point 1: \(point1)")
    print("Point 2: \(point2)")
    print("Distance between points: \(point1.distance(to: point2))")

    // 枚举使用
    let status = Status.approved
    print("Status: \(status.description)")

    // 泛型栈
    var intStack = Stack<Int>()
    intStack.push(1)
    intStack.push(2)
    intStack.push(3)
    print("Stack count: \(intStack.count)")
    print("Popped: \(intStack.pop() ?? 0)")
    print("Stack count after pop: \(intStack.count)")

    // 计算器使用和错误处理
    print("\n--- Calculator and Error Handling ---")
    let calc = Calculator()
    calc.add(10)
    calc.multiply(2)

    do {
        try calc.divide(0)
    } catch CalculatorError.divisionByZero {
        print("Caught division by zero error")
    } catch CalculatorError.invalidInput(let message) {
        print("Invalid input: \(message)")
    } catch {
        print("Unknown error: \(error)")
    }

    print("Calculator result: \(calc.getResult())")

    // 函数调用
    print("\n--- Function Calls ---")
    let fibSequence = fibonacci(10)
    print("Fibonacci sequence: \(fibSequence)")

    let doubled = processArray([1, 2, 3, 4, 5]) { $0 * 2 }
    print("Doubled array: \(doubled)")

    let multiplier = createMultiplier(3)
    print("5 * 3 = \(multiplier(5))")

    // 扩展使用
    print("\n--- Extensions ---")
    let value: Double = 5.0
    print("5 squared: \(value.squared)")
    print("5 cubed: \(value.cubed)")

    let describableObjects: [Describable] = [point1, person]
    print("All objects:\n\(describableObjects.describeAll())")

    // 操作符重载
    print("\n--- Operator Overloading ---")
    let vector1 = Vector2D(x: 1, y: 2)
    let vector2 = Vector2D(x: 3, y: 4)
    let sumVector = vector1 + vector2
    let scaledVector = vector1 * 2.0

    print("Vector 1: (\(vector1.x), \(vector1.y))")
    print("Vector 2: (\(vector2.x), \(vector2.y))")
    print("Sum: (\(sumVector.x), \(sumVector.y))")
    print("Scaled: (\(scaledVector.x), \(scaledVector.y))")

    // 模式匹配
    print("\n--- Pattern Matching ---")
    for num in [0, 5, 15, 42, 100] {
        print("\(num) is \(describeNumber(num))")
    }

    // 日期和时间
    print("\n--- Date and Time ---")
    let now = Date()
    let formatter = DateFormatter()
    formatter.dateStyle = .medium
    formatter.timeStyle = .medium

    print("Current date and time: \(formatter.string(from: now))")

    // 文件操作（基本示例）
    print("\n--- File Operations ---")
    let tempDir = NSTemporaryDirectory()
    let tempFile = tempDir + "swift_example.txt"

    do {
        let content = "This is a test file created by Swift example.\n"
        try content.write(toFile: tempFile, atomically: true, encoding: .utf8)

        let readContent = try String(contentsOfFile: tempFile, encoding: .utf8)
        print("File content: \(readContent.trimmingCharacters(in: .whitespacesAndNewlines))")

        // 清理
        try FileManager.default.removeItem(atPath: tempFile)
        print("Temporary file cleaned up")

    } catch {
        print("File operation error: \(error)")
    }

    print("\n=== Swift Example Completed ===")
}

// 程序入口
main()
