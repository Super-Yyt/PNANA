import strformat

type
  Person = object
    name: string
    age: int

proc greet(person: Person): string =
  fmt"Hello, {person.name}! You are {person.age} years old."

proc main() =
  let person = Person(name: "World", age: 2024)
  echo greet(person)

  # Simple loop example
  for i in 1..5:
    echo fmt"Count: {i}"

  # Conditional example
  let number = 42
  if number > 40:
    echo "Number is greater than 40"
  elif number > 30:
    echo "Number is greater than 30"
  else:
    echo "Number is small"

when isMainModule:
  main()
