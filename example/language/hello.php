<?php

/**
 * PHP 示例文件
 * 展示 PHP 的各种语法特性
 */

// 命名空间
namespace App\Example;

use DateTime;
use Exception;
use PDO;

// 常量定义
const APP_NAME = "PHP Example";
const VERSION = "1.0.0";
const PI = 3.14159;

// 全局变量（不推荐，但在某些情况下有用）
$global_counter = 0;

// 接口定义
interface LoggerInterface {
    public function log(string $message): void;
    public function error(string $message): void;
}

// 抽象类
abstract class Shape {
    protected string $name;

    public function __construct(string $name) {
        $this->name = $name;
    }

    abstract public function area(): float;

    public function getName(): string {
        return $this->name;
    }

    public function describe(): string {
        return "This is a {$this->name} with area " . $this->area();
    }
}

// 具体类实现接口
class FileLogger implements LoggerInterface {
    private string $logFile;

    public function __construct(string $logFile = 'app.log') {
        $this->logFile = $logFile;
    }

    public function log(string $message): void {
        $timestamp = date('Y-m-d H:i:s');
        $logEntry = "[{$timestamp}] INFO: {$message}\n";
        file_put_contents($this->logFile, $logEntry, FILE_APPEND);
    }

    public function error(string $message): void {
        $timestamp = date('Y-m-d H:i:s');
        $logEntry = "[{$timestamp}] ERROR: {$message}\n";
        file_put_contents($this->logFile, $logEntry, FILE_APPEND);
    }
}

// 继承抽象类
class Circle extends Shape {
    private float $radius;

    public function __construct(float $radius) {
        parent::__construct("Circle");
        $this->radius = $radius;
    }

    public function area(): float {
        return PI * $this->radius * $this->radius;
    }

    public function getRadius(): float {
        return $this->radius;
    }
}

class Rectangle extends Shape {
    private float $width;
    private float $height;

    public function __construct(float $width, float $height) {
        parent::__construct("Rectangle");
        $this->width = $width;
        $this->height = $height;
    }

    public function area(): float {
        return $this->width * $this->height;
    }

    public function getDimensions(): array {
        return ['width' => $this->width, 'height' => $this->height];
    }
}

// 特征（Traits）
trait Timestampable {
    private DateTime $createdAt;
    private DateTime $updatedAt;

    public function __construct() {
        $this->createdAt = new DateTime();
        $this->updatedAt = new DateTime();
    }

    public function getCreatedAt(): DateTime {
        return $this->createdAt;
    }

    public function getUpdatedAt(): DateTime {
        return $this->updatedAt;
    }

    public function update(): void {
        $this->updatedAt = new DateTime();
    }
}

// 使用特征的类
class Article {
    use Timestampable;

    private string $title;
    private string $content;

    public function __construct(string $title, string $content) {
        $this->title = $title;
        $this->content = $content;
    }

    public function getTitle(): string {
        return $this->title;
    }

    public function getContent(): string {
        return $this->content;
    }
}

// 枚举（PHP 8.1+）
enum Status {
    case PENDING;
    case APPROVED;
    case REJECTED;

    public function getLabel(): string {
        return match($this) {
            Status::PENDING => 'Pending Review',
            Status::APPROVED => 'Approved',
            Status::REJECTED => 'Rejected',
        };
    }
}

// 工具函数
function calculate_factorial(int $n): int {
    if ($n <= 1) {
        return 1;
    }
    return $n * calculate_factorial($n - 1);
}

function fibonacci(int $n): array {
    if ($n <= 0) return [];
    if ($n === 1) return [0];

    $sequence = [0, 1];
    for ($i = 2; $i < $n; $i++) {
        $sequence[] = $sequence[$i-1] + $sequence[$i-2];
    }
    return $sequence;
}

function validate_email(string $email): bool {
    return filter_var($email, FILTER_VALIDATE_EMAIL) !== false;
}

// 主程序
function main(): void {
    echo "=== PHP Example ===\n\n";

    // 基本数据类型
    echo "--- Basic Data Types ---\n";
    $string = "Hello World";
    $integer = 42;
    $float = 3.14159;
    $boolean = true;
    $array = [1, 2, 3, 4, 5];
    $associative_array = [
        'name' => 'PHP',
        'version' => '8.2',
        'awesome' => true
    ];

    echo "String: $string\n";
    echo "Integer: $integer\n";
    echo "Float: $float\n";
    echo "Boolean: " . ($boolean ? 'true' : 'false') . "\n";
    echo "Array: " . implode(', ', $array) . "\n";
    echo "Associative Array: " . json_encode($associative_array) . "\n\n";

    // 字符串操作
    echo "--- String Operations ---\n";
    $name = "Alice";
    $age = 30;
    $message = "My name is $name and I am $age years old";
    echo $message . "\n";
    echo "String length: " . strlen($string) . "\n";
    echo "Uppercase: " . strtoupper($string) . "\n";
    echo "Reversed: " . strrev($string) . "\n\n";

    // 数组操作
    echo "--- Array Operations ---\n";
    $numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
    $even_numbers = array_filter($numbers, fn($n) => $n % 2 === 0);
    $squared_numbers = array_map(fn($n) => $n * $n, $numbers);
    $sum = array_sum($numbers);

    echo "Original: [" . implode(', ', $numbers) . "]\n";
    echo "Even numbers: [" . implode(', ', $even_numbers) . "]\n";
    echo "Squared: [" . implode(', ', $squared_numbers) . "]\n";
    echo "Sum: $sum\n\n";

    // 条件语句
    echo "--- Conditional Statements ---\n";
    $score = 85;

    if ($score >= 90) {
        $grade = 'A';
    } elseif ($score >= 80) {
        $grade = 'B';
    } elseif ($score >= 70) {
        $grade = 'C';
    } else {
        $grade = 'F';
    }

    echo "Score: $score, Grade: $grade\n\n";

    // 循环
    echo "--- Loops ---\n";
    echo "For loop:\n";
    for ($i = 1; $i <= 5; $i++) {
        echo "  Iteration $i\n";
    }

    echo "Foreach loop:\n";
    foreach (['apple', 'banana', 'cherry'] as $fruit) {
        echo "  I like $fruit\n";
    }

    echo "While loop:\n";
    $counter = 1;
    while ($counter <= 3) {
        echo "  Count: $counter\n";
        $counter++;
    }
    echo "\n";

    // 类和对象
    echo "--- Classes and Objects ---\n";
    $circle = new Circle(5.0);
    $rectangle = new Rectangle(4.0, 6.0);

    echo $circle->describe() . "\n";
    echo $rectangle->describe() . "\n";

    $logger = new FileLogger();
    $logger->log("Application started");
    $logger->log("Circle area calculated: " . $circle->area());
    $logger->error("This is a test error message");

    // 使用特征的类
    $article = new Article("PHP 8.2 Features", "PHP 8.2 introduces many new features...");
    echo "Article created at: " . $article->getCreatedAt()->format('Y-m-d H:i:s') . "\n";

    // 枚举
    echo "\n--- Enums ---\n";
    $status = Status::APPROVED;
    echo "Status: " . $status->getLabel() . "\n";

    // 函数调用
    echo "\n--- Function Calls ---\n";
    echo "Factorial of 5: " . calculate_factorial(5) . "\n";
    echo "Fibonacci sequence: [" . implode(', ', fibonacci(10)) . "]\n";

    // 电子邮件验证
    echo "\n--- Email Validation ---\n";
    $emails = ['valid@example.com', 'invalid-email', 'another@valid.com'];
    foreach ($emails as $email) {
        $isValid = validate_email($email) ? 'Valid' : 'Invalid';
        echo "$email: $isValid\n";
    }

    // 异常处理
    echo "\n--- Exception Handling ---\n";
    try {
        if (rand(0, 1)) {
            throw new Exception("Random exception occurred");
        }
        echo "No exception thrown\n";
    } catch (Exception $e) {
        echo "Caught exception: " . $e->getMessage() . "\n";
    } finally {
        echo "This always executes\n";
    }

    // 数据库连接示例（模拟）
    echo "\n--- Database Operations (Simulated) ---\n";
    try {
        // 注意：这只是模拟，实际使用需要真实的数据库
        $pdo = new PDO('sqlite::memory:');
        $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

        // 创建表
        $pdo->exec("
            CREATE TABLE users (
                id INTEGER PRIMARY KEY,
                name TEXT NOT NULL,
                email TEXT UNIQUE
            )
        ");

        // 插入数据
        $stmt = $pdo->prepare("INSERT INTO users (name, email) VALUES (?, ?)");
        $users = [
            ['Alice Johnson', 'alice@example.com'],
            ['Bob Smith', 'bob@example.com'],
            ['Charlie Brown', 'charlie@example.com']
        ];

        foreach ($users as $user) {
            $stmt->execute($user);
        }

        // 查询数据
        $result = $pdo->query("SELECT * FROM users");
        echo "Users in database:\n";
        while ($row = $result->fetch(PDO::FETCH_ASSOC)) {
            echo "  ID: {$row['id']}, Name: {$row['name']}, Email: {$row['email']}\n";
        }

    } catch (Exception $e) {
        echo "Database error (expected in example): " . $e->getMessage() . "\n";
    }

    // 文件操作
    echo "\n--- File Operations ---\n";
    $tempFile = tempnam(sys_get_temp_dir(), 'php_example_');
    $content = "This is a test file created by PHP example.\n";

    file_put_contents($tempFile, $content);
    $readContent = file_get_contents($tempFile);

    echo "File content: " . trim($readContent) . "\n";
    echo "File exists: " . (file_exists($tempFile) ? 'Yes' : 'No') . "\n";

    // 清理
    unlink($tempFile);

    // JSON 操作
    echo "\n--- JSON Operations ---\n";
    $data = [
        'application' => APP_NAME,
        'version' => VERSION,
        'features' => ['classes', 'traits', 'enums', 'arrow_functions'],
        'timestamp' => time()
    ];

    $jsonString = json_encode($data, JSON_PRETTY_PRINT);
    echo "JSON data:\n$jsonString\n";

    $decodedData = json_decode($jsonString, true);
    echo "Decoded application name: " . $decodedData['application'] . "\n";

    echo "\n=== PHP Example Completed ===\n";
}

// 脚本入口
if ($argv[0] === __FILE__) {
    main();
}
