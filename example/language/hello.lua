#!/usr/bin/env lua

-- Lua 示例文件
-- 展示 Lua 语言的各种语法特性

-- 全局变量和常量
APP_NAME = "Lua Example"
VERSION = "1.0.0"
PI = 3.14159

-- 全局函数
function greet(name)
    return "Hello, " .. name .. "!"
end

-- 递归函数
function factorial(n)
    if n <= 1 then
        return 1
    else
        return n * factorial(n - 1)
    end
end

-- Fibonacci 数列
function fibonacci(n)
    if n <= 0 then return {} end
    if n == 1 then return {0} end

    local sequence = {0, 1}
    for i = 3, n do
        table.insert(sequence, sequence[i-1] + sequence[i-2])
    end
    return sequence
end

-- 表（Table）作为类
Person = {}
Person.__index = Person

-- 构造函数
function Person.new(name, age, email)
    local self = setmetatable({}, Person)
    self.name = name
    self.age = age
    self.email = email or nil
    self.id = Person.generate_id()
    return self
end

-- 类方法
function Person.generate_id()
    if not Person._id_counter then
        Person._id_counter = 0
    end
    Person._id_counter = Person._id_counter + 1
    return Person._id_counter
end

-- 实例方法
function Person:greet()
    return "Hello, my name is " .. self.name .. " and I'm " .. self.age .. " years old!"
end

function Person:is_adult()
    return self.age >= 18
end

function Person:celebrate_birthday()
    self.age = self.age + 1
    return "Happy birthday! You're now " .. self.age .. " years old."
end

function Person:__tostring()
    return self.name .. " (" .. self.age .. ")"
end

-- 元表设置（运算符重载）
Person.__add = function(a, b)
    return Person.new(a.name .. " + " .. b.name, a.age + b.age)
end

-- 模块系统
local MathUtils = {}

function MathUtils.square(x)
    return x * x
end

function MathUtils.cube(x)
    return x * x * x
end

function MathUtils.average(t)
    local sum = 0
    for _, v in ipairs(t) do
        sum = sum + v
    end
    return sum / #t
end

-- 协程示例
function producer()
    return coroutine.create(function()
        for i = 1, 10 do
            coroutine.yield(i * i)
        end
    end)
end

function consumer(prod)
    return function()
        local status, value = coroutine.resume(prod)
        if status then
            return value
        else
            return nil
        end
    end
end

-- 错误处理
function safe_divide(a, b)
    if b == 0 then
        error("Division by zero")
    end
    return a / b
end

function safe_execute(func, ...)
    local status, result = pcall(func, ...)
    if status then
        return result
    else
        return nil, result
    end
end

-- 文件操作
function read_file(filename)
    local file = io.open(filename, "r")
    if not file then
        return nil, "Cannot open file: " .. filename
    end

    local content = file:read("*all")
    file:close()
    return content
end

function write_file(filename, content)
    local file = io.open(filename, "w")
    if not file then
        return false, "Cannot open file for writing: " .. filename
    end

    file:write(content)
    file:close()
    return true
end

-- 迭代器
function squares_iterator(max)
    local i = 0
    return function()
        i = i + 1
        if i <= max then
            return i, i * i
        end
    end
end

-- 泛型函数
function map(table, func)
    local result = {}
    for k, v in pairs(table) do
        result[k] = func(v)
    end
    return result
end

function filter(table, predicate)
    local result = {}
    for k, v in pairs(table) do
        if predicate(v) then
            result[k] = v
        end
    end
    return result
end

-- 元表和代理
local Proxy = {}
Proxy.__index = function(table, key)
    print("Accessing key: " .. key)
    return rawget(table, "_" .. key)
end

Proxy.__newindex = function(table, key, value)
    print("Setting key: " .. key .. " = " .. tostring(value))
    rawset(table, "_" .. key, value)
end

function Proxy.new()
    return setmetatable({}, Proxy)
end

-- 模式匹配（使用字符串操作）
function extract_numbers(text)
    local numbers = {}
    for num in string.gmatch(text, "%d+") do
        table.insert(numbers, tonumber(num))
    end
    return numbers
end

function validate_email(email)
    local pattern = "^[a-zA-Z0-9._%%+-]+@[a-zA-Z0-9.-]+%.[a-zA-Z]{2,}$"
    return string.match(email, pattern) ~= nil
end

-- 主程序
function main()
    print("=== Lua Example ===\n")

    -- 基本数据类型
    print("--- Basic Data Types ---")
    local str = "Hello, Lua World!"
    local num = 42
    local float = 3.14159
    local boolean = true
    local nil_value = nil

    print("String:", str)
    print("Number:", num)
    print("Float:", float)
    print("Boolean:", boolean)
    print("Nil:", nil_value)
    print()

    -- 表操作
    print("--- Tables ---")
    local array = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}
    local dict = {
        name = "Lua",
        version = "5.4",
        awesome = true,
        features = {"coroutines", "metatables", "closures"}
    }

    print("Array:", table.concat(array, ", "))
    print("Dictionary name:", dict.name)
    print("Dictionary features:", table.concat(dict.features, ", "))
    print()

    -- 函数调用
    print("--- Function Calls ---")
    print(greet("Lua"))
    print("Factorial of 5:", factorial(5))

    local fib_seq = fibonacci(10)
    print("Fibonacci sequence:", table.concat(fib_seq, ", "))
    print()

    -- 面向对象
    print("--- Object-Oriented Programming ---")
    local person1 = Person.new("Alice", 30, "alice@example.com")
    local person2 = Person.new("Bob", 25)

    print("Person 1:", person1)
    print("Person 2:", person2)
    print("Person 1 greeting:", person1:greet())
    print("Is person 1 adult?", person1:is_adult())

    print("Birthday celebration:", person1:celebrate_birthday())
    print("After birthday:", person1)
    print()

    -- 模块使用
    print("--- Modules ---")
    print("Square of 5:", MathUtils.square(5))
    print("Cube of 3:", MathUtils.cube(3))
    print("Average of array:", MathUtils.average(array))
    print()

    -- 泛型函数
    print("--- Generic Functions ---")
    local doubled = map(array, function(x) return x * 2 end)
    local even = filter(array, function(x) return x % 2 == 0 end)

    print("Original:", table.concat(array, ", "))
    print("Doubled:", table.concat(doubled, ", "))
    print("Even numbers:", table.concat(even, ", "))
    print()

    -- 协程
    print("--- Coroutines ---")
    local prod = producer()
    local cons = consumer(prod)

    print("Squares from coroutine:")
    for i = 1, 5 do
        local value = cons()
        if value then
            print("  " .. i .. "^2 = " .. value)
        end
    end
    print()

    -- 迭代器
    print("--- Iterators ---")
    print("Squares using iterator:")
    for i, square in squares_iterator(5) do
        print("  " .. i .. "^2 = " .. square)
    end
    print()

    -- 错误处理
    print("--- Error Handling ---")
    local result, error_msg = safe_execute(safe_divide, 10, 2)
    if result then
        print("10 / 2 =", result)
    else
        print("Error:", error_msg)
    end

    result, error_msg = safe_execute(safe_divide, 10, 0)
    if result then
        print("10 / 0 =", result)
    else
        print("Error:", error_msg)
    end
    print()

    -- 字符串操作和模式匹配
    print("--- String Operations ---")
    local text = "The numbers are 123, 456 and 789 in this text."
    local numbers = extract_numbers(text)
    print("Text:", text)
    print("Extracted numbers:", table.concat(numbers, ", "))

    local emails = {"valid@example.com", "invalid-email", "another@valid.com"}
    print("Email validation:")
    for _, email in ipairs(emails) do
        local is_valid = validate_email(email) and "Valid" or "Invalid"
        print("  " .. email .. ": " .. is_valid)
    end
    print()

    -- 代理对象
    print("--- Proxy Objects ---")
    local proxy = Proxy.new()
    proxy.name = "Lua Proxy"
    proxy.value = 42
    print("Proxy name:", proxy.name)
    print("Proxy value:", proxy.value)
    print()

    -- 控制结构
    print("--- Control Structures ---")

    -- if-elseif-else
    local score = 85
    if score >= 90 then
        print("Grade: A")
    elseif score >= 80 then
        print("Grade: B")
    elseif score >= 70 then
        print("Grade: C")
    else
        print("Grade: F")
    end

    -- for 循环
    print("For loop (1 to 5):")
    for i = 1, 5 do
        io.write(i .. " ")
    end
    io.write("\n")

    -- while 循环
    print("While loop:")
    local counter = 1
    while counter <= 3 do
        io.write(counter .. " ")
        counter = counter + 1
    end
    io.write("\n")

    -- repeat-until 循环
    print("Repeat-until loop:")
    counter = 1
    repeat
        io.write(counter .. " ")
        counter = counter + 1
    until counter > 3
    io.write("\n")

    -- foreach 循环
    print("Foreach loop on array:")
    for index, value in ipairs(array) do
        if index > 5 then break end
        io.write(value .. " ")
    end
    io.write("\n")

    -- foreach 循环 on dictionary
    print("Foreach loop on dictionary:")
    for key, value in pairs(dict) do
        if type(value) == "table" then
            print("  " .. key .. ": [" .. table.concat(value, ", ") .. "]")
        else
            print("  " .. key .. ": " .. tostring(value))
        end
    end
    print()

    -- 文件操作演示
    print("--- File Operations (Demo) ---")
    local temp_content = "This is a test file created by Lua example.\nLine 2\nLine 3"
    local success, err = write_file("temp_lua_example.txt", temp_content)

    if success then
        print("Created temporary file: temp_lua_example.txt")
        local content, read_err = read_file("temp_lua_example.txt")
        if content then
            print("File content: " .. content:gsub("\n", "\\n"))
        else
            print("Read error:", read_err)
        end

        -- 清理
        os.remove("temp_lua_example.txt")
        print("Cleaned up temporary file")
    else
        print("Write error:", err)
    end
    print()

    -- 数学和时间
    print("--- Math and Time ---")
    print("Random number:", math.random())
    print("Square root of 16:", math.sqrt(16))
    print("Sin of PI/2:", math.sin(PI/2))
    print("Current time:", os.time())
    print("Date string:", os.date("%Y-%m-%d %H:%M:%S"))
    print()

    print("=== Lua Example Completed ===")
end

-- 如果是直接运行此脚本
if arg and arg[0] and arg[0]:match("hello%.lua$") then
    main()
end

-- 返回模块（如果作为模块使用）
return {
    greet = greet,
    factorial = factorial,
    fibonacci = fibonacci,
    Person = Person,
    MathUtils = MathUtils
}
