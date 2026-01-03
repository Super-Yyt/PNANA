-- Haskell 示例文件
-- 展示 Haskell 的各种语法特性

-- 模块定义
module Main where

-- 导入模块
import Data.List
import Data.Char
import System.IO
import Control.Monad
import qualified Data.Map as Map

-- 类型定义
data Person = Person {
    name :: String,
    age :: Int,
    email :: Maybe String
} deriving (Show, Eq)

-- 代数数据类型
data Shape = Circle Double
           | Rectangle Double Double
           | Triangle Double Double Double
           deriving (Show, Eq)

-- 类型类实例
instance Ord Person where
    compare p1 p2 = compare (age p1) (age p2)

-- 函数定义
greet :: String -> String
greet name = "Hello, " ++ name ++ "!"

-- 模式匹配
factorial :: Integer -> Integer
factorial 0 = 1
factorial n = n * factorial (n - 1)

-- 递归函数
fibonacci :: Int -> Int
fibonacci 0 = 0
fibonacci 1 = 1
fibonacci n = fibonacci (n - 1) + fibonacci (n - 2)

-- 高阶函数
applyTwice :: (a -> a) -> a -> a
applyTwice f x = f (f x)

-- Lambda 表达式
doubleList :: [Int] -> [Int]
doubleList = map (\x -> x * 2)

-- 列表推导式
squares :: [Int]
squares = [x * x | x <- [1..10], even x]

-- 守卫 (Guards)
classifyNumber :: Int -> String
classifyNumber n
    | n < 0     = "negative"
    | n == 0    = "zero"
    | n < 10    = "small"
    | otherwise = "large"

-- 记录语法
getPersonName :: Person -> String
getPersonName Person{name = n} = n

-- 运算符定义
(<+>) :: String -> String -> String
a <+> b = a ++ " " ++ b

-- 中缀运算符
infixl 6 <+>

-- 类型别名
type Name = String
type Age = Int
type People = [Person]

-- 新的类型
newtype Email = Email String deriving (Show, Eq)

-- 函子实例
instance Functor Shape where
    fmap f (Circle r) = Circle (f r)
    fmap f (Rectangle w h) = Rectangle (f w) (f h)
    fmap f (Triangle a b c) = Triangle (f a) (f b) (f c)

-- 面积计算
area :: Shape -> Double
area (Circle r) = pi * r * r
area (Rectangle w h) = w * h
area (Triangle a b c) = sqrt (s * (s - a) * (s - b) * (s - c))
    where s = (a + b + c) / 2

-- IO 操作
greetUser :: IO ()
greetUser = do
    putStrLn "What is your name?"
    name <- getLine
    putStrLn $ "Hello, " ++ name ++ "!"

-- Maybe 处理
safeDivide :: Double -> Double -> Maybe Double
safeDivide _ 0 = Nothing
safeDivide x y = Just (x / y)

-- Either 处理
safeSqrt :: Double -> Either String Double
safeSqrt x
    | x < 0     = Left "Cannot take square root of negative number"
    | otherwise = Right (sqrt x)

-- 列表操作
processList :: [Int] -> [String]
processList xs = do
    x <- xs
    let desc = classifyNumber x
    return $ show x ++ " is " ++ desc

-- 使用 do 表示法
calculateAreas :: [Shape] -> IO ()
calculateAreas shapes = do
    forM_ shapes $ \shape -> do
        let a = area shape
        putStrLn $ show shape ++ " has area " ++ show a

-- 状态管理 (使用 State monad 概念)
data Calculator = Calculator {
    value :: Double,
    history :: [String]
} deriving (Show)

initialCalc :: Calculator
initialCalc = Calculator 0 []

add :: Double -> Calculator -> Calculator
add x calc = Calculator (value calc + x) (history calc ++ [show x ++ " added"])

multiply :: Double -> Calculator -> Calculator
multiply x calc = Calculator (value calc * x) (history calc ++ [show x ++ " multiplied"])

-- 泛型函数
findMax :: Ord a => [a] -> Maybe a
findMax [] = Nothing
findMax xs = Just (maximum xs)

-- 部分应用
add5 :: Int -> Int
add5 = (+) 5

-- 函数组合
composedFunction :: Int -> String
composedFunction = show . add5 . (*2)

-- 主函数
main :: IO ()
main = do
    -- 基本输出
    putStrLn "=== Haskell Example ==="

    -- 基本数据类型
    let num = 42 :: Int
    let str = "Hello World" :: String
    let bool = True :: Bool

    putStrLn $ "Number: " ++ show num
    putStrLn $ "String: " ++ str
    putStrLn $ "Boolean: " ++ show bool

    -- 列表操作
    let numbers = [1, 2, 3, 4, 5]
    let doubled = map (*2) numbers
    putStrLn $ "Original: " ++ show numbers
    putStrLn $ "Doubled: " ++ show doubled

    -- 函数调用
    putStrLn $ "Factorial of 5: " ++ show (factorial 5)
    putStrLn $ "Fibonacci 10: " ++ show (fibonacci 10)

    -- 高阶函数
    let result = applyTwice (*2) 3
    putStrLn $ "Apply twice (*2) to 3: " ++ show result

    -- 模式匹配
    let shapes = [Circle 5, Rectangle 4 6, Triangle 3 4 5]
    putStrLn "Shapes and their areas:"
    forM_ shapes $ \shape -> do
        putStrLn $ show shape ++ " -> " ++ show (area shape)

    -- Maybe 和 Either
    case safeDivide 10 2 of
        Just result -> putStrLn $ "10 / 2 = " ++ show result
        Nothing -> putStrLn "Division by zero"

    case safeSqrt (-4) of
        Right result -> putStrLn $ "sqrt(-4) = " ++ show result
        Left error -> putStrLn $ "Error: " ++ error

    -- 自定义类型
    let person = Person "Alice" 30 (Just "alice@example.com")
    putStrLn $ "Person: " ++ show person

    -- 列表推导式
    putStrLn $ "Squares of even numbers: " ++ show squares

    -- 函数组合
    putStrLn $ "Composed function result: " ++ composedFunction 10

    -- 运算符使用
    let combined = "Hello" <+> "World"
    putStrLn $ "Combined string: " ++ combined

    putStrLn "\n=== Haskell Example Completed ==="
