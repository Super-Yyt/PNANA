# Elixir 示例文件
# 展示 Elixir 语言的各种语法特性

defmodule HelloElixir do
  # 模块属性
  @app_name "Elixir Example"
  @version "1.0.0"
  @pi 3.14159

  # 模块常量
  defmacro app_name, do: @app_name
  defmacro version, do: @version
  defmacro pi, do: @pi

  # 全局状态（使用Agent）
  def start_link do
    Agent.start_link(fn -> %{counter: 0} end, name: __MODULE__)
  end

  def get_counter do
    Agent.get(__MODULE__, & &1.counter)
  end

  def increment_counter do
    Agent.update(__MODULE__, & %{&1 | counter: &1.counter + 1})
  end
end

# 结构体定义
defmodule Person do
  defstruct [:name, :age, :email, :id]

  # 构造函数
  def new(name, age, email \\ nil) do
    HelloElixir.increment_counter()
    %Person{
      name: name,
      age: age,
      email: email,
      id: HelloElixir.get_counter()
    }
  end

  # 方法
  def greet(%Person{name: name, age: age}) do
    "Hello, my name is #{name} and I'm #{age} years old!"
  end

  def adult?(%Person{age: age}) do
    age >= 18
  end

  def celebrate_birthday(%Person{} = person) do
    IO.puts("Happy birthday! You're now #{person.age + 1} years old.")
    %{person | age: person.age + 1}
  end
end

# 协议（Protocol）定义
defprotocol Shape do
  def area(shape)
  def perimeter(shape)
  def description(shape)
end

# 具体实现
defmodule Circle do
  defstruct [:radius]

  defimpl Shape do
    def area(%Circle{radius: r}), do: @pi * r * r
    def perimeter(%Circle{radius: r}), do: 2 * @pi * r
    def description(%Circle{radius: r}), do: "Circle with radius #{Float.round(r, 2)}"
  end
end

defmodule Rectangle do
  defstruct [:width, :height]

  defimpl Shape do
    def area(%Rectangle{width: w, height: h}), do: w * h
    def perimeter(%Rectangle{width: w, height: h}), do: 2 * (w + h)
    def description(%Rectangle{width: w, height: h}), do: "Rectangle #{Float.round(w, 2)} x #{Float.round(h, 2)}"
  end
end

defmodule Triangle do
  defstruct [:a, :b, :c]

  defimpl Shape do
    def area(%Triangle{a: a, b: b, c: c}) do
      s = (a + b + c) / 2
      :math.sqrt(s * (s - a) * (s - b) * (s - c))
    end

    def perimeter(%Triangle{a: a, b: b, c: c}), do: a + b + c

    def description(%Triangle{a: a, b: b, c: c}) do
      "Triangle with sides #{Float.round(a, 2)}, #{Float.round(b, 2)}, #{Float.round(c, 2)}"
    end
  end
end

# 泛型容器模块
defmodule Container do
  defstruct [:items]

  def new, do: %Container{items: []}

  def add(%Container{items: items} = container, item) do
    %{container | items: [item | items]}
  end

  def remove(%Container{items: items} = container, index) when index >= 0 and index < length(items) do
    {removed, rest} = List.pop_at(items, index)
    {%{container | items: rest}, removed}
  end

  def remove(container, _index), do: {container, nil}

  def get(%Container{items: items}, index) do
    Enum.at(items, index)
  end

  def size(%Container{items: items}), do: length(items)

  def map(%Container{items: items}, func), do: Enum.map(items, func)

  def filter(%Container{items: items}, predicate), do: Enum.filter(items, predicate)

  def reduce(%Container{items: items}, acc, func), do: Enum.reduce(items, acc, func)
end

# 递归函数
defmodule MathUtils do
  def factorial(0), do: 1
  def factorial(1), do: 1
  def factorial(n) when n > 1, do: n * factorial(n - 1)

  def fibonacci(0), do: []
  def fibonacci(1), do: [0]
  def fibonacci(n) when n > 1 do
    fib_helper([0, 1], n - 2)
  end

  defp fib_helper(sequence, 0), do: Enum.reverse(sequence)
  defp fib_helper([a, b | _] = sequence, count) do
    fib_helper([b + a, a, b | tl(sequence)], count - 1)
  end
end

# 高阶函数和函数式编程
defmodule FunctionalUtils do
  def process_list(list, func), do: Enum.map(list, func)
  def filter_list(list, predicate), do: Enum.filter(list, predicate)
  def reduce_list(list, acc, func), do: Enum.reduce(list, acc, func)

  def compose(f, g) do
    fn x -> f.(g.(x)) end
  end
end

# 宏定义
defmodule Macros do
  defmacro unless(condition, clauses) do
    quote do
      if !unquote(condition) do
        unquote(clauses)
      end
    end
  end

  defmacro measure_time(block) do
    quote do
      start = System.monotonic_time(:microsecond)
      result = unquote(block)
      finish = System.monotonic_time(:microsecond)
      {result, finish - start}
    end
  end
end

# 错误处理
defmodule Validation do
  defmodule ValidationError do
    defexception [:message, :field]
  end

  def validate_email(email) do
    email_regex = ~r/^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$/

    if Regex.match?(email_regex, email) do
      :ok
    else
      {:error, %ValidationError{message: "Invalid email format: #{email}", field: :email}}
    end
  end

  def validate_age(age) when age < 0 do
    {:error, %ValidationError{message: "Age cannot be negative", field: :age}}
  end

  def validate_age(age) when age > 150 do
    {:error, %ValidationError{message: "Age cannot be greater than 150", field: :age}}
  end

  def validate_age(_age), do: :ok

  def safe_divide(_a, 0), do: {:error, "Division by zero"}
  def safe_divide(a, b), do: {:ok, a / b}
end

# 字符串处理
defmodule StringUtils do
  def extract_numbers(text) do
    Regex.scan(~r/\d+/, text)
    |> Enum.map(&String.to_integer/1)
  end

  def capitalize_words(text) do
    text
    |> String.split()
    |> Enum.map(&String.capitalize/1)
    |> Enum.join(" ")
  end
end

# 并发示例
defmodule Concurrent do
  def process_numbers(numbers) do
    numbers
    |> Enum.map(&Task.async(fn -> process_number(&1) end))
    |> Enum.map(&Task.await/1)
  end

  defp process_number(n) do
    Process.sleep(100) # 模拟耗时操作
    n * n
  end
end

# 文件操作
defmodule FileUtils do
  def read_lines(filename) do
    case File.read(filename) do
      {:ok, content} ->
        content
        |> String.split("\n")
        |> Enum.reject(&(&1 == ""))

      {:error, reason} ->
        IO.puts("Error reading file: #{reason}")
        []
    end
  end

  def write_lines(filename, lines) do
    content = Enum.join(lines, "\n")

    case File.write(filename, content) do
      :ok -> IO.puts("File written: #{filename}")
      {:error, reason} -> IO.puts("Error writing file: #{reason}")
    end
  end
end

# 管道操作符示例
defmodule PipelineExample do
  def process_data(data) do
    data
    |> Enum.filter(&is_number/1)
    |> Enum.map(&(&1 * 2))
    |> Enum.sum()
  end
end

# 模式匹配和卫兵子句
defmodule PatternMatching do
  def describe_number(n) when n < 0, do: "negative"
  def describe_number(0), do: "zero"
  def describe_number(n) when n < 10, do: "small positive"
  def describe_number(n) when rem(n, 2) == 0, do: "even"
  def describe_number(_n), do: "other"

  def process_tuple({:ok, value}), do: "Success: #{value}"
  def process_tuple({:error, reason}), do: "Error: #{reason}"
  def process_tuple(_), do: "Unknown result"
end

# 主模块
defmodule Main do
  import Macros

  def run do
    IO.puts("=== Elixir Example ===\n")

    # 启动全局状态
    HelloElixir.start_link()

    # 基本数据类型
    IO.puts("--- Basic Data Types ---")
    str = "Hello, Elixir World!"
    num = 42
    float_num = 3.14159
    bool = true
    atom = :example
    nil_val = nil

    IO.puts("String: #{str}")
    IO.puts("Number: #{num}")
    IO.puts("Float: #{float_num}")
    IO.puts("Boolean: #{bool}")
    IO.puts("Atom: #{atom}")
    IO.puts("Nil: #{nil_val}")
    IO.puts("")

    # 集合操作
    IO.puts("--- Collections ---")
    numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
    even_numbers = Enum.filter(numbers, &Integer.is_even/1)
    squared_numbers = Enum.map(numbers, &(&1 * &1))

    IO.puts("Original: #{inspect(numbers)}")
    IO.puts("Even numbers: #{inspect(even_numbers)}")
    IO.puts("Squared: #{inspect(squared_numbers)}")
    IO.printf("Sum: %d, Average: %.2f%n", Enum.sum(numbers), Enum.sum(numbers) / length(numbers))
    IO.puts("")

    # 结构体使用
    IO.puts("--- Structs ---")
    person1 = Person.new("Alice", 30, "alice@example.com")
    person2 = Person.new("Bob", 25)

    IO.puts("Person 1: #{inspect(person1)}")
    IO.puts("Person 2: #{inspect(person2)}")
    IO.puts("Person 1 greeting: #{Person.greet(person1)}")
    IO.puts("Is person 1 adult? #{Person.adult?(person1)}")

    older_person1 = Person.celebrate_birthday(person1)
    IO.puts("After birthday: #{inspect(older_person1)}")
    IO.puts("")

    # 形状示例
    IO.puts("--- Shapes ---")
    shapes = [
      %Circle{radius: 5.0},
      %Rectangle{width: 4.0, height: 6.0},
      %Triangle{a: 3.0, b: 4.0, c: 5.0}
    ]

    Enum.each(shapes, fn shape ->
      IO.printf("%s -> Area: %.2f, Perimeter: %.2f%n",
                Shape.description(shape), Shape.area(shape), Shape.perimeter(shape))
    end)
    IO.puts("")

    # 泛型容器
    IO.puts("--- Generic Container ---")
    container = Container.new()
    container = Container.add(container, "Hello")
    container = Container.add(container, "World")
    container = Container.add(container, "Elixir")

    IO.puts("Container size: #{Container.size(container)}")
    IO.puts("Container contents:")
    Enum.with_index(Container.map(container, & &1), fn item, index ->
      IO.puts("  #{index}: #{item}")
    end)

    long_words = Container.filter(container, &(String.length(&1) > 4))
    IO.puts("Long words: #{inspect(long_words)}")

    lengths = Container.map(container, &String.length/1)
    IO.puts("Word lengths: #{inspect(lengths)}")
    IO.puts("")

    # 函数式编程
    IO.puts("--- Functional Programming ---")
    doubled = FunctionalUtils.process_list(numbers, &(&1 * 2))
    greater_than_5 = FunctionalUtils.filter_list(numbers, &(&1 > 5))

    IO.puts("Original: #{inspect(numbers)}")
    IO.puts("Doubled: #{inspect(doubled)}")
    IO.puts("Greater than 5: #{inspect(greater_than_5)}")

    # 函数组合
    add_5 = &(&1 + 5)
    multiply_by_2 = &(&1 * 2)
    composed = FunctionalUtils.compose(multiply_by_2, add_5)
    IO.puts("Composed function result (multiply_by_2(add_5(10))): #{composed.(10)}")
    IO.puts("")

    # 递归和数学
    IO.puts("--- Recursion and Math ---")
    IO.puts("Factorial of 5: #{MathUtils.factorial(5)}")
    IO.puts("Fibonacci sequence: #{inspect(MathUtils.fibonacci(10))}")
    IO.puts("")

    # 宏使用
    IO.puts("--- Macros ---")
    unless false do
      IO.puts("This should print (unless with false)")
    end

    unless true do
      IO.puts("This should not print (unless with true)")
    end

    {result, time} = measure_time do
      Process.sleep(100)
      MathUtils.fibonacci(15)
    end

    IO.puts("Fibonacci calculation took #{time} microseconds")
    IO.puts("")

    # 字符串处理
    IO.puts("--- String Processing ---")
    text = "The numbers are 123, 456 and 789 in this text."
    extracted_numbers = StringUtils.extract_numbers(text)

    IO.puts("Original text: #{text}")
    IO.puts("Extracted numbers: #{inspect(extracted_numbers)}")
    IO.puts("Contains 'numbers': #{String.contains?(text, "numbers")}")
    IO.puts("Uppercase: #{String.upcase(text)}")
    IO.puts("Capitalized words: #{StringUtils.capitalize_words(text)}")
    IO.puts("")

    # 错误处理
    IO.puts("--- Error Handling ---")
    emails = ["valid@example.com", "invalid-email", "another@valid.com"]

    Enum.each(emails, fn email ->
      case Validation.validate_email(email) do
        :ok -> IO.puts("#{email}: Valid")
        {:error, error} -> IO.puts("#{email}: Error - #{error.message}")
      end
    end)

    case Validation.validate_age(-5) do
      :ok -> IO.puts("Age validation passed")
      {:error, error} -> IO.puts("Age validation error: #{error.message}")
    end

    case Validation.safe_divide(10, 2) do
      {:ok, result} -> IO.puts("10 / 2 = #{result}")
      {:error, reason} -> IO.puts("Division error: #{reason}")
    end

    case Validation.safe_divide(10, 0) do
      {:ok, result} -> IO.puts("10 / 0 = #{result}")
      {:error, reason} -> IO.puts("Division error: #{reason}")
    end
    IO.puts("")

    # Map 和 Keyword
    IO.puts("--- Maps and Keywords ---")
    grades = %{"Alice" => 95, "Bob" => 87, "Charlie" => 92, "Diana" => 88}

    IO.puts("Student grades:")
    Enum.each(grades, fn {name, grade} ->
      IO.puts("  #{name}: #{grade}")
    end)

    # 按成绩排序
    sorted_grades = Enum.sort_by(grades, fn {_name, grade} -> grade end, &>=/2)
    IO.puts("Sorted by grade (descending):")
    Enum.each(sorted_grades, fn {name, grade} ->
      IO.puts("  #{name}: #{grade}")
    end)
    IO.puts("")

    # 并发
    IO.puts("--- Concurrency ---")
    concurrent_results = Concurrent.process_numbers([1, 2, 3, 4, 5])
    IO.puts("Concurrent squares: #{inspect(Enum.sort(concurrent_results))}")
    IO.puts("")

    # 模式匹配
    IO.puts("--- Pattern Matching ---")
    test_numbers = [-5, 0, 1, 2, 4, 15, 100]

    Enum.each(test_numbers, fn n ->
      IO.puts("#{n} is #{PatternMatching.describe_number(n)}")
    end)

    # 元组模式匹配
    results = [{:ok, "success"}, {:error, "failed"}, {:unknown, "maybe"}]
    Enum.each(results, fn result ->
      IO.puts(PatternMatching.process_tuple(result))
    end)
    IO.puts("")

    # 管道操作符
    IO.puts("--- Pipeline Operator ---")
    mixed_data = [1, "hello", 3.14, 2, "world", 5]
    pipeline_result = PipelineExample.process_data(mixed_data)
    IO.puts("Pipeline result (sum of doubled numbers): #{pipeline_result}")
    IO.puts("")

    # 文件操作演示
    IO.puts("--- File Operations (Demo) ---")
    temp_content = [
      "This is a test file created by Elixir example.",
      "Line 2",
      "Line 3"
    ]

    FileUtils.write_lines("temp_elixir_example.txt", temp_content)

    lines = FileUtils.read_lines("temp_elixir_example.txt")
    IO.puts("File content:")
    Enum.with_index(lines, 1, fn line, index ->
      IO.puts("  #{index}: #{line}")
    end)

    # 清理
    File.rm("temp_elixir_example.txt")
    IO.puts("Cleaned up temporary file")
    IO.puts("")

    # 列表推导式
    IO.puts("--- List Comprehensions ---")
    squares = for n <- 1..5, do: n * n
    IO.puts("Squares of 1..5: #{inspect(squares)}")

    even_squares = for n <- 1..10, Integer.is_even(n), do: n * n
    IO.puts("Squares of even numbers 1..10: #{inspect(even_squares)}")

    coordinates = for x <- 1..2, y <- 1..2, do: {x, y}
    IO.puts("Coordinates: #{inspect(coordinates)}")
    IO.puts("")

    # 日期和时间
    IO.puts("--- Date and Time ---")
    now = DateTime.utc_now()
    IO.puts("Current time: #{now}")
    IO.puts("ISO format: #{DateTime.to_iso8601(now)}")
    IO.puts("Unix timestamp: #{DateTime.to_unix(now)}")
    IO.puts("")

    IO.puts("=== Elixir Example Completed ===")
  end
end

# 脚本入口
Main.run()
