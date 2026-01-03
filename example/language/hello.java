// Java 示例文件
// 展示 Java 语言的各种语法特性

import java.util.*;
import java.util.regex.Pattern;
import java.util.regex.Matcher;
import java.io.*;
import java.nio.file.*;
import java.time.LocalDate;
import java.time.format.DateTimeFormatter;
import java.util.stream.Collectors;
import java.util.function.Function;
import java.util.function.Predicate;

// 全局常量
public class HelloJava {
    private static final String APP_NAME = "Java Example";
    private static final String VERSION = "1.0.0";
    private static final double PI = 3.14159;

    // 静态变量
    private static int globalCounter = 0;

    // 静态方法
    public static void main(String[] args) {
        System.out.println("=== Java Example ===\n");

        demonstrateBasicTypes();
        demonstrateCollections();
        demonstrateOOP();
        demonstrateGenerics();
        demonstrateStreams();
        demonstrateExceptionHandling();
        demonstrateFileOperations();
        demonstrateConcurrency();
        demonstratePatterns();

        System.out.println("\n=== Java Example Completed ===");
    }

    // 基本数据类型
    private static void demonstrateBasicTypes() {
        System.out.println("--- Basic Data Types ---");

        String str = "Hello, Java World!";
        int num = 42;
        double floatNum = 3.14159;
        boolean bool = true;
        char ch = 'J';

        System.out.println("String: " + str);
        System.out.println("Integer: " + num);
        System.out.println("Double: " + floatNum);
        System.out.println("Boolean: " + bool);
        System.out.println("Character: " + ch);
        System.out.println();
    }

    // 集合操作
    private static void demonstrateCollections() {
        System.out.println("--- Collections ---");

        List<Integer> numbers = Arrays.asList(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);

        List<Integer> evenNumbers = numbers.stream()
                .filter(n -> n % 2 == 0)
                .collect(Collectors.toList());

        List<Integer> squaredNumbers = numbers.stream()
                .map(n -> n * n)
                .collect(Collectors.toList());

        int sum = numbers.stream().mapToInt(Integer::intValue).sum();
        double average = numbers.stream().mapToInt(Integer::intValue).average().orElse(0.0);

        System.out.println("Original: " + numbers);
        System.out.println("Even numbers: " + evenNumbers);
        System.out.println("Squared: " + squaredNumbers);
        System.out.printf("Sum: %d, Average: %.2f%n", sum, average);
        System.out.println();

        // Map 示例
        Map<String, Integer> grades = new HashMap<>();
        grades.put("Alice", 95);
        grades.put("Bob", 87);
        grades.put("Charlie", 92);
        grades.put("Diana", 88);

        System.out.println("Student grades:");
        grades.forEach((name, grade) -> System.out.println("  " + name + ": " + grade));

        // 排序
        grades.entrySet().stream()
                .sorted(Map.Entry.<String, Integer>comparingByValue().reversed())
                .forEach(entry -> System.out.println("  " + entry.getKey() + ": " + entry.getValue()));

        System.out.println();
    }

    // 面向对象编程
    private static void demonstrateOOP() {
        System.out.println("--- Object-Oriented Programming ---");

        Person person1 = new Person("Alice", 30, "alice@example.com");
        Person person2 = new Person("Bob", 25, null);

        System.out.println("Person 1: " + person1);
        System.out.println("Person 2: " + person2);
        System.out.println("Person 1 greeting: " + person1.greet());
        System.out.println("Is person 1 adult? " + person1.isAdult());

        person1.celebrateBirthday();
        System.out.println("After birthday: " + person1);
        System.out.println();

        // 形状示例
        List<Shape> shapes = Arrays.asList(
                new Circle(5.0),
                new Rectangle(4.0, 6.0),
                new Triangle(3.0, 4.0, 5.0)
        );

        for (Shape shape : shapes) {
            System.out.printf("%s -> Area: %.2f, Perimeter: %.2f%n",
                    shape.getDescription(), shape.area(), shape.perimeter());
        }
        System.out.println();
    }

    // 泛型
    private static void demonstrateGenerics() {
        System.out.println("--- Generics ---");

        Container<String> stringContainer = new Container<>();
        stringContainer.add("Hello");
        stringContainer.add("World");
        stringContainer.add("Java");

        System.out.println("Container size: " + stringContainer.size());
        System.out.print("Container contents: ");
        stringContainer.forEach(s -> System.out.print(s + " "));
        System.out.println();

        List<String> longWords = stringContainer.filter(s -> s.length() > 4);
        System.out.println("Long words: " + longWords);

        List<Integer> lengths = stringContainer.map(s -> s.length());
        System.out.println("Word lengths: " + lengths);
        System.out.println();
    }

    // Stream API
    private static void demonstrateStreams() {
        System.out.println("--- Stream API ---");

        List<Integer> numbers = Arrays.asList(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);

        // 过滤、映射和收集
        List<Integer> result = numbers.stream()
                .filter(n -> n % 2 == 0)
                .map(n -> n * n)
                .collect(Collectors.toList());

        System.out.println("Even squares: " + result);

        // 分组
        Map<Boolean, List<Integer>> grouped = numbers.stream()
                .collect(Collectors.partitioningBy(n -> n % 2 == 0));

        System.out.println("Even numbers: " + grouped.get(true));
        System.out.println("Odd numbers: " + grouped.get(false));

        // 统计
        IntSummaryStatistics stats = numbers.stream()
                .mapToInt(Integer::intValue)
                .summaryStatistics();

        System.out.printf("Statistics - Count: %d, Sum: %d, Avg: %.2f, Min: %d, Max: %d%n",
                stats.getCount(), stats.getSum(), stats.getAverage(), stats.getMin(), stats.getMax());

        System.out.println();
    }

    // 异常处理
    private static void demonstrateExceptionHandling() {
        System.out.println("--- Exception Handling ---");

        String[] emails = {"valid@example.com", "invalid-email", "another@valid.com"};

        for (String email : emails) {
            try {
                validateEmail(email);
                System.out.println(email + ": Valid");
            } catch (ValidationException e) {
                System.out.println(email + ": Error - " + e.getMessage());
            }
        }

        try {
            validateAge(-5);
        } catch (ValidationException e) {
            System.out.println("Age validation error: " + e.getMessage());
        }

        try {
            double result = safeDivide(10.0, 2.0);
            System.out.printf("10.0 / 2.0 = %.2f%n", result);
        } catch (ArithmeticException e) {
            System.out.println("Division error: " + e.getMessage());
        }

        try {
            double result = safeDivide(10.0, 0.0);
            System.out.printf("10.0 / 0.0 = %.2f%n", result);
        } catch (ArithmeticException e) {
            System.out.println("Division error: " + e.getMessage());
        }

        System.out.println();
    }

    // 文件操作
    private static void demonstrateFileOperations() {
        System.out.println("--- File Operations (Demo) ---");

        List<String> tempContent = Arrays.asList(
                "This is a test file created by Java example.",
                "Line 2",
                "Line 3"
        );

        try {
            Files.write(Paths.get("temp_java_example.txt"), tempContent);
            System.out.println("Created temporary file: temp_java_example.txt");

            List<String> lines = Files.readAllLines(Paths.get("temp_java_example.txt"));
            System.out.println("File content:");
            for (int i = 0; i < lines.size(); i++) {
                System.out.printf("  %d: %s%n", i + 1, lines.get(i));
            }

            // 清理
            Files.delete(Paths.get("temp_java_example.txt"));
            System.out.println("Cleaned up temporary file");

        } catch (IOException e) {
            System.err.println("File operation error: " + e.getMessage());
        }

        System.out.println();
    }

    // 并发
    private static void demonstrateConcurrency() {
        System.out.println("--- Concurrency ---");

        List<Integer> numbers = Arrays.asList(1, 2, 3, 4, 5);

        // 并行流
        List<Integer> squares = numbers.parallelStream()
                .map(n -> {
                    try {
                        Thread.sleep(100); // 模拟耗时操作
                    } catch (InterruptedException e) {
                        Thread.currentThread().interrupt();
                    }
                    return n * n;
                })
                .collect(Collectors.toList());

        System.out.println("Squares from parallel stream: " + squares);

        // 线程示例
        Thread thread = new Thread(() -> {
            System.out.println("Hello from thread!");
            try {
                Thread.sleep(500);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
            System.out.println("Thread finished!");
        });

        thread.start();
        try {
            thread.join();
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }

        System.out.println();
    }

    // 模式匹配和正则表达式
    private static void demonstratePatterns() {
        System.out.println("--- Patterns and Regular Expressions ---");

        String text = "The numbers are 123, 456 and 789 in this text.";
        System.out.println("Original text: " + text);

        // 正则表达式提取数字
        Pattern pattern = Pattern.compile("\\d+");
        Matcher matcher = pattern.matcher(text);

        List<Integer> extractedNumbers = new ArrayList<>();
        while (matcher.find()) {
            extractedNumbers.add(Integer.parseInt(matcher.group()));
        }

        System.out.println("Extracted numbers: " + extractedNumbers);

        // 字符串操作
        System.out.println("Contains 'numbers': " + text.contains("numbers"));
        System.out.println("To uppercase: " + text.toUpperCase());
        System.out.println("Replace 'numbers' with 'digits': " + text.replace("numbers", "digits"));

        System.out.println();
    }

    // 工具方法
    private static void validateEmail(String email) throws ValidationException {
        String emailRegex = "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$";
        if (!Pattern.matches(emailRegex, email)) {
            throw new ValidationException("Invalid email format: " + email);
        }
    }

    private static void validateAge(int age) throws ValidationException {
        if (age < 0) {
            throw new ValidationException("Age cannot be negative");
        }
        if (age > 150) {
            throw new ValidationException("Age cannot be greater than 150");
        }
    }

    private static double safeDivide(double a, double b) throws ArithmeticException {
        if (b == 0.0) {
            throw new ArithmeticException("Division by zero");
        }
        return a / b;
    }
}

// Person 类
class Person {
    private String name;
    private int age;
    private String email;

    public Person(String name, int age, String email) {
        this.name = name;
        this.age = age;
        this.email = email;
        HelloJava.globalCounter++;
    }

    public String greet() {
        return String.format("Hello, my name is %s and I'm %d years old!", name, age);
    }

    public boolean isAdult() {
        return age >= 18;
    }

    public void celebrateBirthday() {
        age++;
        System.out.printf("Happy birthday! You're now %d years old.%n", age);
    }

    @Override
    public String toString() {
        String emailStr = email != null ? email : "N/A";
        return String.format("%s (%d) - %s", name, age, emailStr);
    }

    // Getters and setters
    public String getName() { return name; }
    public void setName(String name) { this.name = name; }
    public int getAge() { return age; }
    public void setAge(int age) { this.age = age; }
    public String getEmail() { return email; }
    public void setEmail(String email) { this.email = email; }
}

// 形状接口
interface Shape {
    double area();
    double perimeter();
    String getDescription();
}

// 圆形
class Circle implements Shape {
    private double radius;

    public Circle(double radius) {
        this.radius = radius;
    }

    @Override
    public double area() {
        return Math.PI * radius * radius;
    }

    @Override
    public double perimeter() {
        return 2 * Math.PI * radius;
    }

    @Override
    public String getDescription() {
        return String.format("Circle with radius %.2f", radius);
    }

    public double getRadius() { return radius; }
    public void setRadius(double radius) { this.radius = radius; }
}

// 矩形
class Rectangle implements Shape {
    private double width, height;

    public Rectangle(double width, double height) {
        this.width = width;
        this.height = height;
    }

    @Override
    public double area() {
        return width * height;
    }

    @Override
    public double perimeter() {
        return 2 * (width + height);
    }

    @Override
    public String getDescription() {
        return String.format("Rectangle %.2f x %.2f", width, height);
    }

    public double getWidth() { return width; }
    public void setWidth(double width) { this.width = width; }
    public double getHeight() { return height; }
    public void setHeight(double height) { this.height = height; }
}

// 三角形
class Triangle implements Shape {
    private double a, b, c;

    public Triangle(double a, double b, double c) {
        this.a = a;
        this.b = b;
        this.c = c;
    }

    @Override
    public double area() {
        double s = (a + b + c) / 2;
        return Math.sqrt(s * (s - a) * (s - b) * (s - c));
    }

    @Override
    public double perimeter() {
        return a + b + c;
    }

    @Override
    public String getDescription() {
        return String.format("Triangle with sides %.2f, %.2f, %.2f", a, b, c);
    }

    public double getA() { return a; }
    public double getB() { return b; }
    public double getC() { return c; }
}

// 泛型容器类
class Container<T> {
    private List<T> items = new ArrayList<>();

    public void add(T item) {
        items.add(item);
    }

    public T remove(int index) {
        return items.remove(index);
    }

    public T get(int index) {
        return items.get(index);
    }

    public int size() {
        return items.size();
    }

    public boolean isEmpty() {
        return items.isEmpty();
    }

    public void forEach(java.util.function.Consumer<T> action) {
        items.forEach(action);
    }

    public <R> List<R> map(Function<T, R> transform) {
        return items.stream().map(transform).collect(Collectors.toList());
    }

    public List<T> filter(Predicate<T> predicate) {
        return items.stream().filter(predicate).collect(Collectors.toList());
    }

    @Override
    public String toString() {
        return items.toString();
    }
}

// 自定义异常
class ValidationException extends Exception {
    public ValidationException(String message) {
        super(message);
    }
}
