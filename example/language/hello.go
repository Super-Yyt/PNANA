// Go 示例文件
// 展示 Go 语言的各种语法特性

package main

import (
	"bufio"
	"encoding/json"
	"errors"
	"fmt"
	"io"
	"log"
	"math"
	"math/rand"
	"os"
	"regexp"
	"sort"
	"strconv"
	"strings"
	"time"
)

// 全局常量
const (
	AppName = "Go Example"
	Version = "1.0.0"
	Pi      = 3.14159
)

// 全局变量
var (
	globalCounter = 0
	globalSlice   = []string{"apple", "banana", "cherry"}
)

// 结构体定义
type Person struct {
	Name  string `json:"name"`
	Age   int    `json:"age"`
	Email *string `json:"email,omitempty"`
	ID    int    `json:"-"`
}

// 构造函数
func NewPerson(name string, age int, email *string) *Person {
	globalCounter++
	return &Person{
		Name:  name,
		Age:   age,
		Email: email,
		ID:    globalCounter,
	}
}

// 方法
func (p *Person) Greet() string {
	return fmt.Sprintf("Hello, my name is %s and I'm %d years old!", p.Name, p.Age)
}

func (p *Person) IsAdult() bool {
	return p.Age >= 18
}

func (p *Person) CelebrateBirthday() {
	p.Age++
	fmt.Printf("Happy birthday! You're now %d years old.\n", p.Age)
}

func (p Person) String() string {
	email := "N/A"
	if p.Email != nil {
		email = *p.Email
	}
	return fmt.Sprintf("%s (%d) - %s", p.Name, p.Age, email)
}

// 接口定义
type Shape interface {
	Area() float64
	Perimeter() float64
	Description() string
}

type Measurable interface {
	Measure() float64
}

// 具体实现
type Circle struct {
	Radius float64
}

func (c Circle) Area() float64 {
	return math.Pi * c.Radius * c.Radius
}

func (c Circle) Perimeter() float64 {
	return 2 * math.Pi * c.Radius
}

func (c Circle) Description() string {
	return fmt.Sprintf("Circle with radius %.2f", c.Radius)
}

func (c Circle) Measure() float64 {
	return c.Area()
}

type Rectangle struct {
	Width, Height float64
}

func (r Rectangle) Area() float64 {
	return r.Width * r.Height
}

func (r Rectangle) Perimeter() float64 {
	return 2 * (r.Width + r.Height)
}

func (r Rectangle) Description() string {
	return fmt.Sprintf("Rectangle %.2f x %.2f", r.Width, r.Height)
}

func (r Rectangle) Measure() float64 {
	return r.Area()
}

// 泛型容器（Go 1.18+）
type Container[T any] struct {
	items []T
}

func NewContainer[T any]() *Container[T] {
	return &Container[T]{items: make([]T, 0)}
}

func (c *Container[T]) Add(item T) {
	c.items = append(c.items, item)
}

func (c *Container[T]) Remove(index int) (T, bool) {
	if index < 0 || index >= len(c.items) {
		var zero T
		return zero, false
	}
	item := c.items[index]
	c.items = append(c.items[:index], c.items[index+1:]...)
	return item, true
}

func (c *Container[T]) Get(index int) (T, bool) {
	if index < 0 || index >= len(c.items) {
		var zero T
		return zero, false
	}
	return c.items[index], true
}

func (c *Container[T]) Size() int {
	return len(c.items)
}

func (c *Container[T]) ForEach(fn func(T)) {
	for _, item := range c.items {
		fn(item)
	}
}

func (c *Container[T]) Filter(predicate func(T) bool) []T {
	result := make([]T, 0)
	for _, item := range c.items {
		if predicate(item) {
			result = append(result, item)
		}
	}
	return result
}

func (c *Container[T]) Map[R any](transform func(T) R) []R {
	result := make([]R, len(c.items))
	for i, item := range c.items {
		result[i] = transform(item)
	}
	return result
}

// 计算器
type Calculator struct {
	result float64
}

func NewCalculator() *Calculator {
	return &Calculator{result: 0}
}

func (c *Calculator) Add(value float64) *Calculator {
	c.result += value
	return c
}

func (c *Calculator) Multiply(value float64) *Calculator {
	c.result *= value
	return c
}

func (c *Calculator) Divide(value float64) *Calculator {
	if value != 0 {
		c.result /= value
	}
	return c
}

func (c *Calculator) GetResult() float64 {
	return c.result
}

func (c *Calculator) Reset() *Calculator {
	c.result = 0
	return c
}

func (c *Calculator) String() string {
	return fmt.Sprintf("Calculator(result = %.2f)", c.result)
}

// 工具函数
func Factorial(n int) int {
	if n <= 1 {
		return 1
	}
	return n * Factorial(n - 1)
}

func Fibonacci(n int) []int {
	if n <= 0 {
		return []int{}
	}
	if n == 1 {
		return []int{0}
	}

	sequence := []int{0, 1}
	for len(sequence) < n {
		next := sequence[len(sequence)-1] + sequence[len(sequence)-2]
		sequence = append(sequence, next)
	}
	return sequence
}

// 错误处理
type ValidationError struct {
	Field   string
	Message string
}

func (e ValidationError) Error() string {
	return fmt.Sprintf("validation error on field '%s': %s", e.Field, e.Message)
}

func ValidateEmail(email string) error {
	emailRegex := regexp.MustCompile(`^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$`)
	if !emailRegex.MatchString(email) {
		return ValidationError{Field: "email", Message: "invalid email format"}
	}
	return nil
}

func ValidateAge(age int) error {
	if age < 0 {
		return ValidationError{Field: "age", Message: "age cannot be negative"}
	}
	if age > 150 {
		return ValidationError{Field: "age", Message: "age cannot be greater than 150"}
	}
	return nil
}

// 文件操作
func ReadFileLines(filename string) ([]string, error) {
	file, err := os.Open(filename)
	if err != nil {
		return nil, err
	}
	defer file.Close()

	var lines []string
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		lines = append(lines, scanner.Text())
	}

	return lines, scanner.Err()
}

func WriteToFile(filename string, lines []string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	writer := bufio.NewWriter(file)
	for _, line := range lines {
		if _, err := writer.WriteString(line + "\n"); err != nil {
			return err
		}
	}
	return writer.Flush()
}

// 并发示例
func processNumbers(nums []int, results chan<- int) {
	for _, num := range nums {
		results <- num * num
	}
	close(results)
}

// 泛型函数
func ProcessSlice[T any, R any](slice []T, transform func(T) R) []R {
	result := make([]R, len(slice))
	for i, item := range slice {
		result[i] = transform(item)
	}
	return result
}

func FilterSlice[T any](slice []T, predicate func(T) bool) []T {
	result := make([]T, 0)
	for _, item := range slice {
		if predicate(item) {
			result = append(result, item)
		}
	}
	return result
}

// JSON 处理
func PersonToJSON(p *Person) (string, error) {
	data, err := json.MarshalIndent(p, "", "  ")
	if err != nil {
		return "", err
	}
	return string(data), nil
}

func PersonFromJSON(jsonStr string) (*Person, error) {
	var person Person
	err := json.Unmarshal([]byte(jsonStr), &person)
	if err != nil {
		return nil, err
	}
	return &person, nil
}

// 主函数
func main() {
	fmt.Println("=== Go Example ===\n")

	// 基本数据类型
	fmt.Println("--- Basic Data Types ---")
	var str string = "Hello, Go World!"
	var num int = 42
	var floatNum float64 = 3.14159
	var boolean bool = true
	var nilPtr *string = nil

	fmt.Printf("String: %s\n", str)
	fmt.Printf("Number: %d\n", num)
	fmt.Printf("Float: %.5f\n", floatNum)
	fmt.Printf("Boolean: %t\n", boolean)
	fmt.Printf("Nil pointer: %v\n", nilPtr)
	fmt.Println()

	// 切片操作
	fmt.Println("--- Slices ---")
	numbers := []int{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}
	evenNumbers := FilterSlice(numbers, func(n int) bool { return n%2 == 0 })
	squaredNumbers := ProcessSlice(numbers, func(n int) int { return n * n })

	sum := 0
	for _, n := range numbers {
		sum += n
	}
	average := float64(sum) / float64(len(numbers))

	fmt.Printf("Original: %v\n", numbers)
	fmt.Printf("Even numbers: %v\n", evenNumbers)
	fmt.Printf("Squared: %v\n", squaredNumbers)
	fmt.Printf("Sum: %d, Average: %.2f\n", sum, average)
	fmt.Println()

	// 结构体使用
	fmt.Println("--- Structs ---")
	email1 := "alice@example.com"
	person1 := NewPerson("Alice", 30, &email1)
	person2 := NewPerson("Bob", 25, nil)

	fmt.Printf("Person 1: %s\n", person1)
	fmt.Printf("Person 2: %s\n", person2)
	fmt.Printf("Person 1 greeting: %s\n", person1.Greet())
	fmt.Printf("Is person 1 adult? %t\n", person1.IsAdult())

	person1.CelebrateBirthday()
	fmt.Printf("After birthday: %s\n", person1)
	fmt.Println()

	// 接口和多态
	fmt.Println("--- Interfaces and Polymorphism ---")
	shapes := []Shape{
		Circle{Radius: 5.0},
		Rectangle{Width: 4.0, Height: 6.0},
		Circle{Radius: 3.0},
	}

	for _, shape := range shapes {
		fmt.Printf("%s -> Area: %.2f, Perimeter: %.2f\n",
			shape.Description(), shape.Area(), shape.Perimeter())
	}
	fmt.Println()

	// 泛型容器
	fmt.Println("--- Generic Container ---")
	stringContainer := NewContainer[string]()
	stringContainer.Add("Hello")
	stringContainer.Add("World")
	stringContainer.Add("Go")

	fmt.Printf("Container size: %d\n", stringContainer.Size())
	fmt.Print("Container contents: ")
	stringContainer.ForEach(func(s string) {
		fmt.Printf("%s ", s)
	})
	fmt.Println()

	longWords := stringContainer.Filter(func(s string) bool { return len(s) > 4 })
	fmt.Printf("Long words: %v\n", longWords)

	lengths := stringContainer.Map(func(s string) int { return len(s) })
	fmt.Printf("Word lengths: %v\n", lengths)
	fmt.Println()

	// 计算器
	fmt.Println("--- Calculator ---")
	calc := NewCalculator().
		Add(10.0).
		Multiply(2.0).
		Add(5.0)

	fmt.Printf("Calculator result: %.2f\n", calc.GetResult())

	fmt.Printf("Factorial of 5: %d\n", Factorial(5))
	fmt.Printf("Fibonacci sequence: %v\n", Fibonacci(10))
	fmt.Println()

	// 映射（Map）
	fmt.Println("--- Maps ---")
	grades := map[string]int{
		"Alice":   95,
		"Bob":     87,
		"Charlie": 92,
		"Diana":   88,
	}

	fmt.Println("Student grades:")
	for name, grade := range grades {
		fmt.Printf("  %s: %d\n", name, grade)
	}

	// 按成绩排序
	type studentGrade struct {
		name  string
		grade int
	}
	var gradeList []studentGrade
	for name, grade := range grades {
		gradeList = append(gradeList, studentGrade{name, grade})
	}
	sort.Slice(gradeList, func(i, j int) bool {
		return gradeList[i].grade > gradeList[j].grade
	})

	fmt.Println("Sorted by grade (descending):")
	for _, sg := range gradeList {
		fmt.Printf("  %s: %d\n", sg.name, sg.grade)
	}
	fmt.Println()

	// 并发
	fmt.Println("--- Concurrency ---")
	nums := []int{1, 2, 3, 4, 5}
	results := make(chan int, len(nums))

	go processNumbers(nums, results)

	fmt.Print("Squares from goroutine: ")
	for square := range results {
		fmt.Printf("%d ", square)
	}
	fmt.Println()
	fmt.Println()

	// 错误处理
	fmt.Println("--- Error Handling ---")
	emails := []string{"valid@example.com", "invalid-email", "another@valid.com"}

	for _, email := range emails {
		if err := ValidateEmail(email); err != nil {
			fmt.Printf("%s: Error - %v\n", email, err)
		} else {
			fmt.Printf("%s: Valid\n", email)
		}
	}

	if err := ValidateAge(-5); err != nil {
		fmt.Printf("Age validation error: %v\n", err)
	}
	fmt.Println()

	// JSON 处理
	fmt.Println("--- JSON Processing ---")
	jsonStr, err := PersonToJSON(person1)
	if err != nil {
		log.Printf("JSON marshal error: %v", err)
	} else {
		fmt.Println("Person as JSON:")
		fmt.Println(jsonStr)

		// 从 JSON 解析回来
		parsedPerson, err := PersonFromJSON(jsonStr)
		if err != nil {
			log.Printf("JSON unmarshal error: %v", err)
		} else {
			fmt.Printf("Parsed person: %s\n", parsedPerson)
		}
	}
	fmt.Println()

	// 文件操作演示
	fmt.Println("--- File Operations (Demo) ---")
	tempContent := []string{
		"This is a test file created by Go example.",
		"Line 2",
		"Line 3",
	}

	err = WriteToFile("temp_go_example.txt", tempContent)
	if err != nil {
		log.Printf("Write file error: %v", err)
	} else {
		fmt.Println("Created temporary file: temp_go_example.txt")

		lines, err := ReadFileLines("temp_go_example.txt")
		if err != nil {
			log.Printf("Read file error: %v", err)
		} else {
			fmt.Println("File content:")
			for i, line := range lines {
				fmt.Printf("  %d: %s\n", i+1, line)
			}
		}

		// 清理
		os.Remove("temp_go_example.txt")
		fmt.Println("Cleaned up temporary file")
	}
	fmt.Println()

	// 时间和随机数
	fmt.Println("--- Time and Random ---")
	rand.Seed(time.Now().UnixNano())
	fmt.Printf("Random number: %d\n", rand.Intn(100))
	fmt.Printf("Current time: %s\n", time.Now().Format("2006-01-02 15:04:05"))
	fmt.Printf("Unix timestamp: %d\n", time.Now().Unix())
	fmt.Println()

	// 字符串操作
	fmt.Println("--- String Operations ---")
	text := "The numbers are 123, 456 and 789 in this text."
	fmt.Printf("Original text: %s\n", text)

	// 提取数字
	re := regexp.MustCompile(`\d+`)
	numberStrings := re.FindAllString(text, -1)
	var extractedNumbers []int
	for _, numStr := range numberStrings {
		if num, err := strconv.Atoi(numStr); err == nil {
			extractedNumbers = append(extractedNumbers, num)
		}
	}
	fmt.Printf("Extracted numbers: %v\n", extractedNumbers)

	// 字符串分割和连接
	words := strings.Fields(text)
	fmt.Printf("Words: %v\n", words)
	fmt.Printf("Uppercase: %s\n", strings.ToUpper(text))
	fmt.Printf("Contains 'numbers': %t\n", strings.Contains(text, "numbers"))
	fmt.Println()

	// 类型断言和反射
	fmt.Println("--- Type Assertions ---")
	var shape Shape = Circle{Radius: 5.0}
	fmt.Printf("Shape description: %s\n", shape.Description())

	// 类型断言
	if circle, ok := shape.(Circle); ok {
		fmt.Printf("Circle radius: %.2f\n", circle.Radius)
	}

	// 类型选择
	switch s := shape.(type) {
	case Circle:
		fmt.Printf("This is a circle with radius %.2f\n", s.Radius)
	case Rectangle:
		fmt.Printf("This is a rectangle %.2f x %.2f\n", s.Width, s.Height)
	default:
		fmt.Println("Unknown shape type")
	}
	fmt.Println()

	fmt.Println("=== Go Example Completed ===")
}
