#!/usr/bin/env node

/**
 * JavaScript 示例文件
 * 展示 ES6+ 特性
 */

// 导入模块
const fs = require('fs');
const path = require('path');

// 全局常量
const PI = 3.14159;
const APP_NAME = "JavaScript Example";

// 类定义
class Animal {
    constructor(name, age) {
        this.name = name;
        this.age = age;
    }

    speak() {
        console.log(`${this.name} makes a sound`);
    }

    getInfo() {
        return `${this.name} is ${this.age} years old`;
    }

    static createDog(name) {
        return new Dog(name, 0);
    }
}

class Dog extends Animal {
    constructor(name, age) {
        super(name, age);
    }

    speak() {
        console.log(`${this.name} barks!`);
    }

    fetch(item) {
        console.log(`${this.name} fetches the ${item}`);
    }
}

// 函数声明
function greet(name) {
    return `Hello, ${name}!`;
}

// 箭头函数
const calculateArea = (radius) => {
    return PI * radius * radius;
};

const multiply = (a, b) => a * b;

// 异步函数
async function fetchData(url) {
    try {
        const response = await fetch(url);
        const data = await response.json();
        return data;
    } catch (error) {
        console.error('Error fetching data:', error);
        throw error;
    }
}

// 生成器函数
function* fibonacciGenerator() {
    let a = 0, b = 1;
    while (true) {
        yield a;
        [a, b] = [b, a + b];
    }
}

// 模板字符串和解构赋值
function processUser({name, age, email = 'unknown@example.com'}) {
    const message = `
        User Information:
        Name: ${name}
        Age: ${age}
        Email: ${email}
        Greeting: ${greet(name)}
    `;
    return message;
}

// 数组方法
const numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];

const doubled = numbers.map(num => num * 2);
const evens = numbers.filter(num => num % 2 === 0);
const sum = numbers.reduce((acc, curr) => acc + curr, 0);

// 对象字面量增强
const createPerson = (name, age) => ({
    name,
    age,
    greet() {
        return `Hi, I'm ${this.name}`;
    },
    get isAdult() {
        return this.age >= 18;
    },
    set updateAge(newAge) {
        this.age = newAge;
    }
});

// Promise 示例
const delay = (ms) => new Promise(resolve => setTimeout(resolve, ms));

async function main() {
    console.log(greet("JavaScript"));

    // 变量声明
    let mutableVar = "I can change";
    const constantVar = "I cannot change";

    // 数据类型
    const string = "Hello World";
    const number = 42;
    const boolean = true;
    const array = [1, 2, 3];
    const object = {key: "value"};

    // 条件语句
    if (number > 40) {
        console.log("Number is greater than 40");
    } else if (number === 42) {
        console.log("The answer to everything");
    } else {
        console.log("Number is small");
    }

    // 循环
    for (let i = 0; i < 3; i++) {
        console.log(`Iteration ${i}`);
    }

    // for...of 循环
    for (const num of numbers) {
        if (num > 5) break;
        console.log(`Number: ${num}`);
    }

    // 类使用
    const dog = new Dog("Buddy", 3);
    dog.speak();
    dog.fetch("ball");
    console.log(dog.getInfo());

    // 工厂方法
    const anotherDog = Animal.createDog("Max");
    anotherDog.speak();

    // 函数调用
    console.log(`Area of circle with radius 5: ${calculateArea(5)}`);
    console.log(`5 * 3 = ${multiply(5, 3)}`);

    // 数组操作
    console.log(`Doubled numbers: ${doubled.join(', ')}`);
    console.log(`Even numbers: ${evens.join(', ')}`);
    console.log(`Sum: ${sum}`);

    // 对象使用
    const person = createPerson("Alice", 30);
    console.log(person.greet());
    console.log(`Is adult: ${person.isAdult}`);
    person.updateAge = 25;
    console.log(`New age: ${person.age}`);

    // 用户处理
    const user = {name: "Bob", age: 25};
    console.log(processUser(user));

    // 生成器
    const fibGen = fibonacciGenerator();
    console.log("Fibonacci numbers:");
    for (let i = 0; i < 10; i++) {
        console.log(fibGen.next().value);
    }

    // 异步操作
    console.log("Starting async operation...");
    await delay(1000);
    console.log("Async operation completed");

    // 可选链和空值合并
    const obj = {a: {b: {c: "value"}}};
    const safeAccess = obj?.a?.b?.c ?? "default";
    console.log(`Safe access result: ${safeAccess}`);

    // BigInt
    const bigNum = 1234567890123456789012345678901234567890n;
    console.log(`BigInt: ${bigNum}`);

    // Symbol
    const sym = Symbol("unique");
    console.log(`Symbol: ${sym.toString()}`);

    console.log("JavaScript example completed!");
}

// 导出模块（如果作为模块使用）
module.exports = {
    Animal,
    Dog,
    greet,
    calculateArea,
    fibonacciGenerator
};

// 如果直接运行此文件
if (require.main === module) {
    main().catch(console.error);
}
