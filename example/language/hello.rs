// Rust 示例文件
// 展示 Rust 语言的各种语法特性

// 全局常量
const APP_NAME: &str = "Rust Example";
const VERSION: &str = "1.0.0";
const PI: f64 = 3.14159;

// 静态变量
static mut GLOBAL_COUNTER: i32 = 0;

// 结构体定义
#[derive(Debug, Clone)]
struct Person {
    name: String,
    age: u32,
    email: Option<String>,
}

impl Person {
    // 关联函数（构造函数）
    fn new(name: String, age: u32, email: Option<String>) -> Self {
        unsafe {
            GLOBAL_COUNTER += 1;
        }
        Person { name, age, email }
    }

    // 方法
    fn greet(&self) -> String {
        format!("Hello, my name is {} and I'm {} years old!", self.name, self.age)
    }

    fn is_adult(&self) -> bool {
        self.age >= 18
    }

    fn celebrate_birthday(&mut self) {
        self.age += 1;
        println!("Happy birthday! You're now {} years old.", self.age);
    }
}

// 实现 Display trait
impl std::fmt::Display for Person {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        let email = self.email.as_ref().map_or("N/A", |e| e);
        write!(f, "{} ({}) - {}", self.name, self.age, email)
    }
}

// 枚举
#[derive(Debug)]
enum Shape {
    Circle(f64),
    Rectangle(f64, f64),
    Triangle(f64, f64, f64),
}

// 实现 Shape 的方法
impl Shape {
    fn area(&self) -> f64 {
        match self {
            Shape::Circle(radius) => PI * radius * radius,
            Shape::Rectangle(width, height) => width * height,
            Shape::Triangle(a, b, c) => {
                let s = (a + b + c) / 2.0;
                (s * (s - a) * (s - b) * (s - c)).sqrt()
            }
        }
    }

    fn perimeter(&self) -> f64 {
        match self {
            Shape::Circle(radius) => 2.0 * PI * radius,
            Shape::Rectangle(width, height) => 2.0 * (width + height),
            Shape::Triangle(a, b, c) => a + b + c,
        }
    }

    fn description(&self) -> String {
        match self {
            Shape::Circle(r) => format!("Circle with radius {:.2}", r),
            Shape::Rectangle(w, h) => format!("Rectangle {:.2} x {:.2}", w, h),
            Shape::Triangle(a, b, c) => format!("Triangle with sides {:.2}, {:.2}, {:.2}", a, b, c),
        }
    }
}

// 泛型结构体
#[derive(Debug)]
struct Container<T> {
    items: Vec<T>,
}

impl<T> Container<T> {
    fn new() -> Self {
        Container { items: Vec::new() }
    }

    fn add(&mut self, item: T) {
        self.items.push(item);
    }

    fn remove(&mut self, index: usize) -> Option<T> {
        if index < self.items.len() {
            Some(self.items.remove(index))
        } else {
            None
        }
    }

    fn get(&self, index: usize) -> Option<&T> {
        self.items.get(index)
    }

    fn len(&self) -> usize {
        self.items.len()
    }

    fn is_empty(&self) -> bool {
        self.items.is_empty()
    }

    fn iter(&self) -> std::slice::Iter<T> {
        self.items.iter()
    }

    fn filter<F>(&self, predicate: F) -> Vec<&T>
    where
        F: Fn(&T) -> bool,
    {
        self.items.iter().filter(|item| predicate(item)).collect()
    }

    fn map<R, F>(&self, transform: F) -> Vec<R>
    where
        F: Fn(&T) -> R,
    {
        self.items.iter().map(transform).collect()
    }
}

// 计算器结构体
#[derive(Debug)]
struct Calculator {
    result: f64,
}

impl Calculator {
    fn new() -> Self {
        Calculator { result: 0.0 }
    }

    fn add(mut self, value: f64) -> Self {
        self.result += value;
        self
    }

    fn multiply(mut self, value: f64) -> Self {
        self.result *= value;
        self
    }

    fn divide(mut self, value: f64) -> Self {
        if value != 0.0 {
            self.result /= value;
        }
        self
    }

    fn get_result(&self) -> f64 {
        self.result
    }

    fn reset(mut self) -> Self {
        self.result = 0.0;
        self
    }
}

impl std::fmt::Display for Calculator {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "Calculator(result = {:.2})", self.result)
    }
}

// 工具函数
fn factorial(n: u32) -> u32 {
    match n {
        0 | 1 => 1,
        _ => n * factorial(n - 1),
    }
}

fn fibonacci(n: usize) -> Vec<u32> {
    if n == 0 {
        return Vec::new();
    }

    let mut sequence = vec![0, 1];
    while sequence.len() < n {
        let next = sequence[sequence.len() - 1] + sequence[sequence.len() - 2];
        sequence.push(next);
    }
    sequence.truncate(n);
    sequence
}

// 泛型函数
fn process_vector<T, F, R>(vec: &Vec<T>, transform: F) -> Vec<R>
where
    F: Fn(&T) -> R,
{
    vec.iter().map(transform).collect()
}

fn filter_vector<T, F>(vec: &Vec<T>, predicate: F) -> Vec<&T>
where
    F: Fn(&T) -> bool,
{
    vec.iter().filter(|item| predicate(item)).collect()
}

// 错误处理
#[derive(Debug)]
enum ValidationError {
    InvalidEmail(String),
    InvalidAge(i32),
    DivisionByZero,
}

impl std::fmt::Display for ValidationError {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self {
            ValidationError::InvalidEmail(email) => write!(f, "Invalid email format: {}", email),
            ValidationError::InvalidAge(age) => write!(f, "Invalid age: {}", age),
            ValidationError::DivisionByZero => write!(f, "Division by zero"),
        }
    }
}

impl std::error::Error for ValidationError {}

fn validate_email(email: &str) -> Result<(), ValidationError> {
    let email_regex = regex::Regex::new(r"^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$")
        .unwrap();

    if email_regex.is_match(email) {
        Ok(())
    } else {
        Err(ValidationError::InvalidEmail(email.to_string()))
    }
}

fn validate_age(age: i32) -> Result<(), ValidationError> {
    if age < 0 {
        Err(ValidationError::InvalidAge(age))
    } else if age > 150 {
        Err(ValidationError::InvalidAge(age))
    } else {
        Ok(())
    }
}

fn safe_divide(a: f64, b: f64) -> Result<f64, ValidationError> {
    if b == 0.0 {
        Err(ValidationError::DivisionByZero)
    } else {
        Ok(a / b)
    }
}

// 模式匹配示例
fn describe_number(n: i32) -> &'static str {
    match n {
        0 => "zero",
        1..=3 => "small positive",
        n if n < 0 => "negative",
        n if n % 2 == 0 => "even",
        _ => "other",
    }
}

// 闭包和迭代器
fn demonstrate_closures() {
    let numbers = vec![1, 2, 3, 4, 5, 6, 7, 8, 9, 10];

    // 使用闭包进行过滤
    let even_numbers: Vec<_> = numbers.iter().filter(|&&x| x % 2 == 0).collect();
    println!("Even numbers: {:?}", even_numbers);

    // 使用闭包进行映射
    let squared: Vec<_> = numbers.iter().map(|&x| x * x).collect();
    println!("Squared numbers: {:?}", squared);

    // 使用闭包进行折叠
    let sum: i32 = numbers.iter().fold(0, |acc, &x| acc + x);
    println!("Sum: {}", sum);

    let product: i32 = numbers.iter().fold(1, |acc, &x| acc * x);
    println!("Product: {}", product);
}

// 并发示例
use std::thread;
use std::sync::mpsc;
use std::time::Duration;

fn demonstrate_concurrency() {
    let (tx, rx) = mpsc::channel();

    thread::spawn(move || {
        let numbers = vec![1, 2, 3, 4, 5];
        for num in numbers {
            tx.send(num * num).unwrap();
            thread::sleep(Duration::from_millis(100));
        }
    });

    println!("Squares from thread:");
    for received in rx {
        println!("  {}", received);
    }
}

// 所有权和借用示例
fn demonstrate_ownership() {
    // 字符串字面量
    let s1 = "Hello";  // &str, 静态生命周期
    println!("String literal: {}", s1);

    // String 类型
    let s2 = String::from("World");  // String, 堆分配
    println!("String object: {}", s2);

    // 移动所有权
    let s3 = s2;  // s2 的所有权移动到 s3
    println!("Moved string: {}", s3);
    // println!("{}", s2);  // 这会编译错误，因为 s2 不再拥有数据

    // 克隆
    let s4 = s3.clone();
    println!("Cloned string: {}", s4);

    // 借用
    let s5 = &s3;
    println!("Borrowed string: {}", s5);

    // 可变借用
    let mut s6 = String::from("Mutable");
    modify_string(&mut s6);
    println!("Modified string: {}", s6);
}

fn modify_string(s: &mut String) {
    s.push_str(" string");
}

// 宏示例
macro_rules! create_vector {
    ($($x:expr),*) => {
        {
            let mut temp_vec = Vec::new();
            $(
                temp_vec.push($x);
            )*
            temp_vec
        }
    };
}

macro_rules! calculate {
    (eval $e:expr) => {{
        {
            let val: f64 = $e;
            println!("{} = {:.2}", stringify!{$e}, val);
            val
        }
    }};
}

// 主函数
fn main() {
    println!("=== Rust Example ===\n");

    // 基本数据类型
    println!("--- Basic Data Types ---");
    let string_var: &str = "Hello, Rust World!";
    let number_var: i32 = 42;
    let float_var: f64 = 3.14159;
    let boolean_var: bool = true;
    let char_var: char = 'R';

    println!("String: {}", string_var);
    println!("Number: {}", number_var);
    println!("Float: {:.5}", float_var);
    println!("Boolean: {}", boolean_var);
    println!("Character: {}", char_var);
    println!();

    // 集合类型
    println!("--- Collections ---");
    let numbers: Vec<i32> = vec![1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
    let even_numbers = filter_vector(&numbers, |&&x| x % 2 == 0);
    let squared_numbers = process_vector(&numbers, |&&x| x * x);

    let sum: i32 = numbers.iter().sum();
    let average = sum as f64 / numbers.len() as f64;

    println!("Original: {:?}", numbers);
    println!("Even numbers: {:?}", even_numbers);
    println!("Squared: {:?}", squared_numbers);
    println!("Sum: {}, Average: {:.2}", sum, average);
    println!();

    // 结构体使用
    println!("--- Structs ---");
    let person1 = Person::new("Alice".to_string(), 30, Some("alice@example.com".to_string()));
    let mut person2 = Person::new("Bob".to_string(), 25, None);

    println!("Person 1: {}", person1);
    println!("Person 2: {}", person2);
    println!("Person 1 greeting: {}", person1.greet());
    println!("Is person 1 adult? {}", person1.is_adult());

    person2.celebrate_birthday();
    println!("After birthday: {}", person2);
    println!();

    // 枚举和模式匹配
    println!("--- Enums and Pattern Matching ---");
    let shapes = vec![
        Shape::Circle(5.0),
        Shape::Rectangle(4.0, 6.0),
        Shape::Triangle(3.0, 4.0, 5.0),
    ];

    for shape in &shapes {
        println!("{} -> Area: {:.2}, Perimeter: {:.2}",
                shape.description(), shape.area(), shape.perimeter());
    }
    println!();

    // 泛型容器
    println!("--- Generic Container ---");
    let mut string_container = Container::new();
    string_container.add("Hello".to_string());
    string_container.add("World".to_string());
    string_container.add("Rust".to_string());

    println!("Container size: {}", string_container.len());
    print!("Container contents: ");
    for item in string_container.iter() {
        print!("{} ", item);
    }
    println!();

    let long_words = string_container.filter(|s| s.len() > 4);
    println!("Long words: {:?}", long_words);

    let lengths = string_container.map(|s| s.len());
    println!("Word lengths: {:?}", lengths);
    println!();

    // 计算器
    println!("--- Calculator ---");
    let calc = Calculator::new()
        .add(10.0)
        .multiply(2.0)
        .add(5.0);

    println!("Calculator result: {:.2}", calc.get_result());
    println!("Calculator: {}", calc);

    println!("Factorial of 5: {}", factorial(5));
    println!("Fibonacci sequence: {:?}", fibonacci(10));
    println!();

    // 宏使用
    println!("--- Macros ---");
    let vec = create_vector![1, 2, 3, 4, 5];
    println!("Vector created with macro: {:?}", vec);

    let result = calculate!(eval 2.0 * PI * 3.0);
    println!("Calculated value: {:.2}", result);
    println!();

    // 闭包和迭代器
    println!("--- Closures and Iterators ---");
    demonstrate_closures();
    println!();

    // 所有权和借用
    println!("--- Ownership and Borrowing ---");
    demonstrate_ownership();
    println!();

    // 错误处理
    println!("--- Error Handling ---");
    let emails = vec!["valid@example.com", "invalid-email", "another@valid.com"];

    for email in &emails {
        match validate_email(email) {
            Ok(()) => println!("{}: Valid", email),
            Err(e) => println!("{}: Error - {}", email, e),
        }
    }

    match validate_age(-5) {
        Ok(()) => println!("Age validation passed"),
        Err(e) => println!("Age validation failed: {}", e),
    }

    match safe_divide(10.0, 2.0) {
        Ok(result) => println!("10.0 / 2.0 = {:.2}", result),
        Err(e) => println!("Division error: {}", e),
    }

    match safe_divide(10.0, 0.0) {
        Ok(result) => println!("10.0 / 0.0 = {:.2}", result),
        Err(e) => println!("Division error: {}", e),
    }
    println!();

    // 模式匹配
    println!("--- Pattern Matching ---");
    let test_numbers = vec![-5, 0, 1, 2, 4, 15, 100];
    for &num in &test_numbers {
        println!("{} is {}", num, describe_number(num));
    }
    println!();

    // 并发（需要启用相应 feature）
    println!("--- Concurrency ---");
    demonstrate_concurrency();
    println!();

    // 字符串操作
    println!("--- String Operations ---");
    let text = "The numbers are 123, 456 and 789 in this text.";
    println!("Original text: {}", text);

    // 使用正则表达式提取数字
    let re = regex::Regex::new(r"\d+").unwrap();
    let number_strings: Vec<&str> = re.find_iter(text).map(|m| m.as_str()).collect();

    let mut extracted_numbers = Vec::new();
    for num_str in number_strings {
        if let Ok(num) = num_str.parse::<i32>() {
            extracted_numbers.push(num);
        }
    }
    println!("Extracted numbers: {:?}", extracted_numbers);

    // 字符串方法
    println!("Contains 'numbers': {}", text.contains("numbers"));
    println!("To uppercase: {}", text.to_uppercase());
    println!("Replace 'numbers' with 'digits': {}", text.replace("numbers", "digits"));
    println!();

    // 切片和数组
    println!("--- Slices and Arrays ---");
    let array = [1, 2, 3, 4, 5];
    let slice = &array[1..4];
    println!("Array: {:?}", array);
    println!("Slice [1..4]: {:?}", slice);

    let matrix = [[1, 2, 3], [4, 5, 6], [7, 8, 9]];
    println!("Matrix:");
    for row in &matrix {
        println!("  {:?}", row);
    }
    println!();

    // 智能指针
    println!("--- Smart Pointers ---");
    let boxed_value = Box::new(42);
    println!("Boxed value: {}", boxed_value);

    let reference_counted = std::rc::Rc::new("Shared string".to_string());
    let rc_clone = std::rc::Rc::clone(&reference_counted);
    println!("Reference counted: {}", reference_counted);
    println!("Clone: {}", rc_clone);
    println!("Reference count: {}", std::rc::Rc::strong_count(&reference_counted));
    println!();

    println!("=== Rust Example Completed ===");

    // 防止未使用变量的警告
    let _ = APP_NAME;
    let _ = VERSION;
    unsafe {
        let _ = GLOBAL_COUNTER;
    }
}
