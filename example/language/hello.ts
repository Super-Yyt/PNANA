// TypeScript 示例文件
// 展示 TypeScript 语言的各种语法特性

// 全局常量
const APP_NAME: string = "TypeScript Example";
const VERSION: string = "1.0.0";
const PI: number = 3.14159;

// 全局变量
let globalCounter: number = 0;

// 接口定义
interface Person {
    name: string;
    age: number;
    email?: string; // 可选属性
    readonly id: number; // 只读属性
}

// 接口继承
interface Employee extends Person {
    department: string;
    salary: number;
}

// 类型别名
type ShapeType = "circle" | "rectangle" | "triangle";
type Coordinate = [number, number];
type PersonCallback = (person: Person) => void;

// 枚举
enum Status {
    Pending = "pending",
    Approved = "approved",
    Rejected = "rejected"
}

// 泛型接口
interface Container<T> {
    items: T[];
    add(item: T): void;
    remove(index: number): T | undefined;
    get(index: number): T | undefined;
    size(): number;
    map<R>(transform: (item: T) => R): R[];
    filter(predicate: (item: T) => boolean): T[];
}

// 类定义
class PersonClass implements Person {
    public readonly id: number;
    public name: string;
    public age: number;
    public email?: string;

    constructor(name: string, age: number, email?: string) {
        this.name = name;
        this.age = age;
        this.email = email;
        this.id = ++globalCounter;
    }

    greet(): string {
        return `Hello, my name is ${this.name} and I'm ${this.age} years old!`;
    }

    isAdult(): boolean {
        return this.age >= 18;
    }

    celebrateBirthday(): void {
        this.age++;
        console.log(`Happy birthday! You're now ${this.age} years old.`);
    }

    toString(): string {
        const email = this.email ?? "N/A";
        return `${this.name} (${this.age}) - ${email}`;
    }
}

// 抽象类
abstract class Shape {
    abstract area(): number;
    abstract perimeter(): number;
    abstract description(): string;

    // 具体方法
    isLarge(): boolean {
        return this.area() > 100;
    }
}

// 具体实现类
class Circle extends Shape {
    constructor(private radius: number) {
        super();
    }

    area(): number {
        return Math.PI * this.radius * this.radius;
    }

    perimeter(): number {
        return 2 * Math.PI * this.radius;
    }

    description(): string {
        return `Circle with radius ${this.radius.toFixed(2)}`;
    }

    getRadius(): number {
        return this.radius;
    }
}

class Rectangle extends Shape {
    constructor(private width: number, private height: number) {
        super();
    }

    area(): number {
        return this.width * this.height;
    }

    perimeter(): number {
        return 2 * (this.width + this.height);
    }

    description(): string {
        return `Rectangle ${this.width.toFixed(2)} x ${this.height.toFixed(2)}`;
    }

    getWidth(): number { return this.width; }
    getHeight(): number { return this.height; }
}

class Triangle extends Shape {
    constructor(private a: number, private b: number, private c: number) {
        super();
    }

    area(): number {
        const s = (this.a + this.b + this.c) / 2;
        return Math.sqrt(s * (s - this.a) * (s - this.b) * (s - this.c));
    }

    perimeter(): number {
        return this.a + this.b + this.c;
    }

    description(): string {
        return `Triangle with sides ${this.a.toFixed(2)}, ${this.b.toFixed(2)}, ${this.c.toFixed(2)}`;
    }
}

// 泛型类实现
class ArrayContainer<T> implements Container<T> {
    items: T[] = [];

    add(item: T): void {
        this.items.push(item);
    }

    remove(index: number): T | undefined {
        if (index >= 0 && index < this.items.length) {
            return this.items.splice(index, 1)[0];
        }
        return undefined;
    }

    get(index: number): T | undefined {
        return this.items[index];
    }

    size(): number {
        return this.items.length;
    }

    map<R>(transform: (item: T) => R): R[] {
        return this.items.map(transform);
    }

    filter(predicate: (item: T) => boolean): T[] {
        return this.items.filter(predicate);
    }

    forEach(callback: (item: T, index: number) => void): void {
        this.items.forEach(callback);
    }
}

// 工具函数
function greet(name: string): string {
    return `Hello, ${name}!`;
}

function factorial(n: number): number {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

function fibonacci(n: number): number[] {
    if (n <= 0) return [];
    if (n === 1) return [0];

    const sequence: number[] = [0, 1];
    for (let i = 2; i < n; i++) {
        sequence.push(sequence[i - 1] + sequence[i - 2]);
    }
    return sequence;
}

// 泛型函数
function processArray<T, R>(array: T[], transform: (item: T) => R): R[] {
    return array.map(transform);
}

function filterArray<T>(array: T[], predicate: (item: T) => boolean): T[] {
    return array.filter(predicate);
}

// 高级类型
type ReadonlyPerson = Readonly<Person>;
type PartialPerson = Partial<Person>;
type RequiredPerson = Required<Person>;
type PersonKeys = keyof Person;

// 联合类型和类型守卫
function describeValue(value: string | number | boolean): string {
    if (typeof value === "string") {
        return `String: ${value} (length: ${value.length})`;
    } else if (typeof value === "number") {
        return `Number: ${value} (${value % 2 === 0 ? 'even' : 'odd'})`;
    } else {
        return `Boolean: ${value}`;
    }
}

// 装饰器（实验性功能）
function logMethod(target: any, propertyName: string, descriptor: PropertyDescriptor) {
    const method = descriptor.value;
    descriptor.value = function(...args: any[]) {
        console.log(`Calling ${propertyName} with arguments:`, args);
        const result = method.apply(this, args);
        console.log(`Result:`, result);
        return result;
    };
}

// 异步函数
async function fetchUserData(userId: number): Promise<Person> {
    // 模拟异步操作
    return new Promise((resolve) => {
        setTimeout(() => {
            resolve(new PersonClass(`User${userId}`, 25 + userId, `user${userId}@example.com`));
        }, 100);
    });
}

// 错误处理
class ValidationError extends Error {
    constructor(message: string, public field: string) {
        super(message);
        this.name = "ValidationError";
    }
}

function validateEmail(email: string): void {
    const emailRegex = /^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$/;
    if (!emailRegex.test(email)) {
        throw new ValidationError(`Invalid email format: ${email}`, "email");
    }
}

function validateAge(age: number): void {
    if (age < 0) {
        throw new ValidationError("Age cannot be negative", "age");
    }
    if (age > 150) {
        throw new ValidationError("Age cannot be greater than 150", "age");
    }
}

// 泛型约束
function findMax<T extends number | string>(array: T[]): T | undefined {
    if (array.length === 0) return undefined;

    let max = array[0];
    for (let i = 1; i < array.length; i++) {
        if (array[i] > max) {
            max = array[i];
        }
    }
    return max;
}

// 条件类型
type IsString<T> = T extends string ? true : false;
type StringCheck = IsString<string>;  // true
type NumberCheck = IsString<number>;  // false

// 映射类型
type ReadonlyShape<T> = {
    readonly [P in keyof T]: T[P];
};

// 工具类型使用
type ReadonlyPersonType = ReadonlyShape<Person>;

// 主函数
async function main() {
    console.log("=== TypeScript Example ===\n");

    // 基本数据类型
    console.log("--- Basic Data Types ---");
    const str: string = "Hello, TypeScript World!";
    const num: number = 42;
    const floatNum: number = 3.14159;
    const bool: boolean = true;
    const nil: null = null;
    const undef: undefined = undefined;

    console.log(`String: ${str}`);
    console.log(`Number: ${num}`);
    console.log(`Float: ${floatNum}`);
    console.log(`Boolean: ${bool}`);
    console.log(`Null: ${nil}`);
    console.log(`Undefined: ${undef}`);
    console.log();

    // 数组和元组
    console.log("--- Arrays and Tuples ---");
    const numbers: number[] = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
    const evenNumbers = filterArray(numbers, (n) => n % 2 === 0);
    const squaredNumbers = processArray(numbers, (n) => n * n);

    const sum = numbers.reduce((acc, n) => acc + n, 0);
    const average = sum / numbers.length;

    console.log(`Original: ${numbers}`);
    console.log(`Even numbers: ${evenNumbers}`);
    console.log(`Squared: ${squaredNumbers}`);
    console.log(`Sum: ${sum}, Average: ${average.toFixed(2)}`);
    console.log();

    // 对象和类
    console.log("--- Classes and Objects ---");
    const person1 = new PersonClass("Alice", 30, "alice@example.com");
    const person2 = new PersonClass("Bob", 25);

    console.log(`Person 1: ${person1}`);
    console.log(`Person 2: ${person2}`);
    console.log(`Person 1 greeting: ${person1.greet()}`);
    console.log(`Is person 1 adult? ${person1.isAdult()}`);

    person1.celebrateBirthday();
    console.log(`After birthday: ${person1}`);
    console.log();

    // 形状示例
    console.log("--- Shapes ---");
    const shapes: Shape[] = [
        new Circle(5.0),
        new Rectangle(4.0, 6.0),
        new Triangle(3.0, 4.0, 5.0)
    ];

    shapes.forEach(shape => {
        console.log(`${shape.description()} -> Area: ${shape.area().toFixed(2)}, Perimeter: ${shape.perimeter().toFixed(2)}`);
    });
    console.log();

    // 泛型容器
    console.log("--- Generic Container ---");
    const stringContainer = new ArrayContainer<string>();
    stringContainer.add("Hello");
    stringContainer.add("World");
    stringContainer.add("TypeScript");

    console.log(`Container size: ${stringContainer.size()}`);
    console.log("Container contents:");
    stringContainer.forEach((item, index) => console.log(`  ${index}: ${item}`));

    const longWords = stringContainer.filter(s => s.length > 4);
    console.log(`Long words: ${longWords}`);

    const lengths = stringContainer.map(s => s.length);
    console.log(`Word lengths: ${lengths}`);
    console.log();

    // 异步操作
    console.log("--- Asynchronous Operations ---");
    try {
        const userData = await fetchUserData(123);
        console.log(`Fetched user: ${userData}`);
    } catch (error) {
        console.error("Error fetching user data:", error);
    }
    console.log();

    // 错误处理
    console.log("--- Error Handling ---");
    const emails = ["valid@example.com", "invalid-email", "another@valid.com"];

    emails.forEach(email => {
        try {
            validateEmail(email);
            console.log(`${email}: Valid`);
        } catch (error) {
            if (error instanceof ValidationError) {
                console.log(`${email}: Error - ${error.message}`);
            } else {
                console.log(`${email}: Unexpected error - ${error}`);
            }
        }
    });

    try {
        validateAge(-5);
    } catch (error) {
        console.log(`Age validation error: ${(error as ValidationError).message}`);
    }
    console.log();

    // 高级类型示例
    console.log("--- Advanced Types ---");
    const values: (string | number | boolean)[] = ["hello", 42, true, "world", 3.14];

    values.forEach(value => {
        console.log(describeValue(value));
    });
    console.log();

    // 工具函数
    console.log("--- Utility Functions ---");
    console.log(`Factorial of 5: ${factorial(5)}`);
    console.log(`Fibonacci sequence: ${fibonacci(10)}`);
    console.log(`Max in [3, 1, 4, 1, 5, 9, 2, 6]: ${findMax([3, 1, 4, 1, 5, 9, 2, 6])}`);
    console.log();

    // Map 和 Set
    console.log("--- Maps and Sets ---");
    const grades = new Map<string, number>();
    grades.set("Alice", 95);
    grades.set("Bob", 87);
    grades.set("Charlie", 92);
    grades.set("Diana", 88);

    console.log("Student grades:");
    grades.forEach((grade, name) => console.log(`  ${name}: ${grade}`));

    // 按成绩排序
    const sortedGrades = Array.from(grades.entries())
        .sort((a, b) => b[1] - a[1]);

    console.log("Sorted by grade (descending):");
    sortedGrades.forEach(([name, grade]) => console.log(`  ${name}: ${grade}`));
    console.log();

    // 符号和迭代器
    console.log("--- Symbols and Iterators ---");
    const uniqueId = Symbol("id");
    const person = {
        name: "Alice",
        [uniqueId]: 123
    };

    console.log(`Person: ${person.name}`);
    console.log(`Unique ID: ${person[uniqueId]}`);

    // 自定义迭代器
    const range = {
        start: 1,
        end: 5,
        *[Symbol.iterator]() {
            for (let i = this.start; i <= this.end; i++) {
                yield i;
            }
        }
    };

    console.log("Range iteration:", [...range]);
    console.log();

    // 模块导入导出（在实际项目中会使用）
    console.log("--- Module System ---");
    // 这部分在浏览器/Node.js环境中会有所不同
    console.log("TypeScript supports ES6 modules, CommonJS, and AMD");
    console.log();

    console.log("=== TypeScript Example Completed ===");
}

// 立即执行的主函数调用
main().catch(console.error);
