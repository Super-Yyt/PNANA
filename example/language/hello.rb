#!/usr/bin/env ruby

# Ruby示例文件
# 展示Ruby的各种语法特性

require 'date'
require 'time'

# 全局常量
PI = 3.14159
APP_NAME = "Ruby Example"

# 模块定义
module MathUtils
  def self.square(x)
    x * x
  end

  def self.cube(x)
    x * x * x
  end
end

# 异常类
class ValidationError < StandardError
  def initialize(message = "Validation failed")
    super(message)
  end
end

# 类定义
class Person
  # 类变量
  @@person_count = 0

  # 属性访问器
  attr_accessor :name, :age, :email
  attr_reader :id

  # 初始化方法
  def initialize(name, age, email = nil)
    @id = generate_id
    @name = name
    @age = age
    @email = email
    @@person_count += 1
  end

  # 类方法
  def self.total_count
    @@person_count
  end

  def self.create_from_hash(hash)
    new(hash[:name], hash[:age], hash[:email])
  end

  # 实例方法
  def greet
    "Hello, my name is #{@name} and I'm #{@age} years old"
  end

  def adult?
    @age >= 18
  end

  def birthday
    @age += 1
    "Happy birthday! You're now #{@age} years old"
  end

  def to_s
    "#{@name} (#{@age})"
  end

  private

  def generate_id
    rand(10000..99999)
  end
end

# 继承
class Employee < Person
  attr_accessor :position, :salary

  def initialize(name, age, position, salary, email = nil)
    super(name, age, email)
    @position = position
    @salary = salary
  end

  def promote(new_position, new_salary)
    @position = new_position
    @salary = new_salary
    "#{@name} has been promoted to #{@position}"
  end

  def to_s
    super + " - #{@position} ($#{@salary})"
  end
end

# 模块包含（Mixins）
module Debuggable
  def debug_info
    "Object ID: #{object_id}, Class: #{self.class}"
  end
end

class Calculator
  include Debuggable

  def initialize
    @result = 0
  end

  def add(number)
    @result += number
  end

  def subtract(number)
    @result -= number
  end

  def multiply(number)
    @result *= number
  end

  def divide(number)
    raise ZeroDivisionError, "Cannot divide by zero" if number.zero?
    @result /= number.to_f
  end

  def result
    @result
  end

  def reset
    @result = 0
  end
end

# 代码块和迭代器
def fibonacci(n)
  return [] if n <= 0
  return [0] if n == 1

  sequence = [0, 1]
  (2...n).each do |i|
    sequence << sequence[i-1] + sequence[i-2]
  end
  sequence
end

def process_array(array, &block)
  array.map(&block)
end

# DSL 示例
class Config
  def initialize(&block)
    @settings = {}
    instance_eval(&block) if block_given?
  end

  def set(key, value)
    @settings[key] = value
  end

  def get(key)
    @settings[key]
  end

  def to_hash
    @settings.dup
  end
end

# 正则表达式
class StringValidator
  EMAIL_REGEX = /\A[\w+\-.]+@[a-z\d\-]+(\.[a-z\d\-]+)*\.[a-z]+\z/i
  PHONE_REGEX = /\A\d{3}-\d{3}-\d{4}\z/

  def self.valid_email?(email)
    !!(email =~ EMAIL_REGEX)
  end

  def self.valid_phone?(phone)
    !!(phone =~ PHONE_REGEX)
  end
end

# 主程序
def main
  puts "=== Ruby Example ==="

  # 基本数据类型
  puts "\n--- Basic Data Types ---"
  string = "Hello World"
  number = 42
  float = 3.14159
  boolean = true
  array = [1, 2, 3, 4, 5]
  hash = {name: "Ruby", version: "3.0", awesome: true}

  puts "String: #{string}"
  puts "Number: #{number}"
  puts "Float: #{float}"
  puts "Boolean: #{boolean}"
  puts "Array: #{array.inspect}"
  puts "Hash: #{hash.inspect}"

  # 字符串插值和操作
  puts "\n--- String Operations ---"
  name = "Alice"
  age = 30
  puts "My name is #{name} and I am #{age} years old"
  puts "String length: #{string.length}"
  puts "Uppercase: #{string.upcase}"
  puts "Reversed: #{string.reverse}"

  # 数组操作
  puts "\n--- Array Operations ---"
  numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
  puts "Original: #{numbers.inspect}"
  puts "Even numbers: #{numbers.select(&:even?).inspect}"
  puts "Sum: #{numbers.sum}"
  puts "Average: #{numbers.sum / numbers.length.to_f}"

  # 哈希操作
  puts "\n--- Hash Operations ---"
  person_data = {name: "Bob", age: 25, city: "New York"}
  puts "Person: #{person_data.inspect}"
  puts "Keys: #{person_data.keys.inspect}"
  puts "Values: #{person_data.values.inspect}"

  # 条件语句
  puts "\n--- Conditional Statements ---"
  score = 85

  if score >= 90
    grade = "A"
  elsif score >= 80
    grade = "B"
  elsif score >= 70
    grade = "C"
  else
    grade = "F"
  end

  puts "Score: #{score}, Grade: #{grade}"

  # 循环
  puts "\n--- Loops ---"
  puts "For loop:"
  for i in 1..5
    puts "  Iteration #{i}"
  end

  puts "Each loop:"
  ["apple", "banana", "cherry"].each do |fruit|
    puts "  I like #{fruit}"
  end

  puts "Times loop:"
  3.times do |i|
    puts "  Count: #{i}"
  end

  # 类和对象
  puts "\n--- Classes and Objects ---"
  person1 = Person.new("Charlie", 28, "charlie@example.com")
  person2 = Person.new("Diana", 32)

  puts person1.greet
  puts person2.greet
  puts "Total persons created: #{Person.total_count}"

  employee = Employee.new("Eve", 35, "Developer", 75000)
  puts employee.to_s
  puts employee.promote("Senior Developer", 95000)
  puts employee.to_s

  # 异常处理
  puts "\n--- Exception Handling ---"
  begin
    result = 10 / 0
  rescue ZeroDivisionError => e
    puts "Caught division by zero: #{e.message}"
  rescue => e
    puts "Caught general error: #{e.message}"
  ensure
    puts "This always executes"
  end

  # 计算器使用
  puts "\n--- Calculator Usage ---"
  calc = Calculator.new
  calc.add(10)
  calc.multiply(2)
  calc.subtract(3)
  puts "Result: #{calc.result}"
  puts calc.debug_info

  # 斐波那契数列
  puts "\n--- Fibonacci Sequence ---"
  fib_sequence = fibonacci(10)
  puts "First 10 Fibonacci numbers: #{fib_sequence.inspect}"

  # 代码块处理
  puts "\n--- Block Processing ---"
  numbers = [1, 2, 3, 4, 5]
  doubled = process_array(numbers) { |x| x * 2 }
  puts "Original: #{numbers.inspect}"
  puts "Doubled: #{doubled.inspect}"

  # DSL使用
  puts "\n--- DSL Usage ---"
  config = Config.new do
    set :database_url, "sqlite:///app.db"
    set :debug_mode, true
    set :max_connections, 100
  end

  puts "Config: #{config.to_hash.inspect}"

  # 正则表达式验证
  puts "\n--- Regular Expression Validation ---"
  emails = ["valid@example.com", "invalid-email", "another@valid.com"]
  phones = ["123-456-7890", "1234567890", "123-45-6789"]

  puts "Valid emails:"
  emails.each { |email| puts "  #{email}: #{StringValidator.valid_email?(email)}" }

  puts "Valid phones:"
  phones.each { |phone| puts "  #{phone}: #{StringValidator.valid_phone?(phone)}" }

  # 日期和时间
  puts "\n--- Date and Time ---"
  now = Time.now
  today = Date.today

  puts "Current time: #{now}"
  puts "Current date: #{today}"
  puts "Day of week: #{today.strftime('%A')}"

  # 范围和区间
  puts "\n--- Ranges ---"
  range = (1..10)
  puts "Range 1..10 includes 5: #{range.include?(5)}"
  puts "Range 1..10 includes 15: #{range.include?(15)}"

  char_range = ('a'..'z')
  puts "First 5 letters: #{char_range.first(5).inspect}"

  # 符号（Symbols）
  puts "\n--- Symbols ---"
  symbol1 = :hello
  symbol2 = :hello
  string1 = "hello"
  string2 = "hello"

  puts "symbol1 == symbol2: #{symbol1 == symbol2}"
  puts "string1 == string2: #{string1 == string2}"
  puts "symbol1 == string1: #{symbol1 == string1}"
  puts "symbol1.object_id == symbol2.object_id: #{symbol1.object_id == symbol2.object_id}"
  puts "string1.object_id == string2.object_id: #{string1.object_id == string2.object_id}"

  puts "\n=== Ruby Example Completed ==="
end

# 脚本入口
if __FILE__ == $0
  main
end
