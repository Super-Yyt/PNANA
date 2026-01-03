;; Clojure 示例文件
;; 展示 Clojure 语言的各种语法特性

(ns hello-clojure
  (:require [clojure.string :as str]
            [clojure.set :as set]))

;; 全局常量
(def APP-NAME "Clojure Example")
(def VERSION "1.0.0")
(def PI 3.14159)

;; 全局变量
(def global-counter (atom 0))

;; 记录（Record）定义
(defrecord Person [name age email id])

;; 构造函数
(defn make-person
  ([name age] (make-person name age nil))
  ([name age email]
   (swap! global-counter inc)
   (->Person name age email @global-counter)))

;; 协议（Protocol）定义
(defprotocol Shape
  (area [this])
  (perimeter [this])
  (description [this]))

;; 类型扩展
(defrecord Circle [radius]
  Shape
  (area [_] (* Math/PI radius radius))
  (perimeter [_] (* 2 Math/PI radius))
  (description [_] (format "Circle with radius %.2f" (double radius))))

(defrecord Rectangle [width height]
  Shape
  (area [_] (* width height))
  (perimeter [_] (* 2 (+ width height)))
  (description [_] (format "Rectangle %.2f x %.2f" (double width) (double height))))

(defrecord Triangle [a b c]
  Shape
  (area [_]
    (let [s (/ (+ a b c) 2)]
      (Math/sqrt (* s (- s a) (- s b) (- s c)))))
  (perimeter [_] (+ a b c))
  (description [_] (format "Triangle with sides %.2f, %.2f, %.2f"
                          (double a) (double b) (double c))))

;; 多重方法
(defmulti greet :type)

(defmethod greet :person [person]
  (format "Hello, my name is %s and I'm %d years old!"
          (:name person) (:age person)))

(defmethod greet :animal [animal]
  (format "Hello, I'm a %s!" (:species animal)))

;; Person 的方法
(defn person-greet [person]
  (format "Hello, my name is %s and I'm %d years old!"
          (:name person) (:age person)))

(defn person-adult? [person]
  (>= (:age person) 18))

(defn person-celebrate-birthday [person]
  (println (format "Happy birthday! You're now %d years old." (inc (:age person))))
  (update person :age inc))

;; 递归函数
(defn factorial [n]
  (if (<= n 1)
    1
    (* n (factorial (dec n)))))

(defn fibonacci [n]
  (cond
    (<= n 0) []
    (= n 1) [0]
    :else (loop [a 0 b 1 count 2 result [0 1]]
            (if (>= count n)
              result
              (recur b (+ a b) (inc count) (conj result (+ a b)))))))

;; 高阶函数
(defn process-collection [coll f]
  (map f coll))

(defn filter-collection [coll pred]
  (filter pred coll))

(defn reduce-collection [coll f initial]
  (reduce f initial coll))

;; 宏定义
(defmacro unless [test & body]
  `(when (not ~test)
     ~@body))

(defmacro with-timeout [timeout-ms & body]
  `(let [future# (future (do ~@body))
         timeout# (Thread/sleep ~timeout-ms)]
     (or (deref future# ~timeout-ms :timeout)
         (do (future-cancel future#)
             :timeout))))

;; 延迟求值
(def lazy-fibonacci
  (lazy-seq
   (cons 0
         (cons 1
               (map + lazy-fibonacci (rest lazy-fibonacci))))))

;; 工具函数
(defn safe-divide [a b]
  (if (zero? b)
    (throw (ArithmeticException. "Division by zero"))
    (/ a b)))

(defn validate-email [email]
  (let [email-regex #"^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$"]
    (if (re-matches email-regex email)
      true
      (throw (IllegalArgumentException. (str "Invalid email: " email))))))

(defn validate-age [age]
  (cond
    (neg? age) (throw (IllegalArgumentException. "Age cannot be negative"))
    (> age 150) (throw (IllegalArgumentException. "Age cannot be greater than 150"))
    :else true))

;; 集合操作
(defn analyze-numbers [numbers]
  {:original numbers
   :even (filter even? numbers)
   :odd (filter odd? numbers)
   :sum (reduce + numbers)
   :average (/ (reduce + numbers) (count numbers))
   :max (apply max numbers)
   :min (apply min numbers)})

;; 字符串处理
(defn extract-numbers [text]
  (let [number-regex #"\d+"]
    (->> (re-seq number-regex text)
         (map #(Integer/parseInt %)))))

;; 文件操作
(defn read-file-lines [filename]
  (try
    (with-open [reader (clojure.java.io/reader filename)]
      (doall (line-seq reader)))
    (catch Exception e
      (println (str "Error reading file: " (.getMessage e)))
      [])))

(defn write-to-file [filename lines]
  (try
    (with-open [writer (clojure.java.io/writer filename)]
      (doseq [line lines]
        (.write writer (str line "\n"))))
    (println (str "File written: " filename))
    (catch Exception e
      (println (str "Error writing file: " (.getMessage e))))))

;; 并发示例
(defn process-numbers-concurrent [numbers]
  (let [results (atom [])]
    (doseq [num numbers]
      (future
        (Thread/sleep 100) ; 模拟耗时操作
        (swap! results conj (* num num))))
    (Thread/sleep 200) ; 等待所有future完成
    @results))

;; 状态管理
(def person-store (atom {}))

(defn add-person [person]
  (swap! person-store assoc (:id person) person))

(defn get-person [id]
  (get @person-store id))

(defn list-people []
  (vals @person-store))

;; DSL 示例
(defmacro define-shape [name & {:keys [area perimeter description]}]
  `(defrecord ~name []
     Shape
     ~@(when area `((area [_] ~area)))
     ~@(when perimeter `((perimeter [_] ~perimeter)))
     ~@(when description `((description [_] ~description)))))

;; 主函数
(defn -main []
  (println "=== Clojure Example ===\n")

  ;; 基本数据类型
  (println "--- Basic Data Types ---")
  (def str-var "Hello, Clojure World!")
  (def num-var 42)
  (def float-var 3.14159)
  (def bool-var true)
  (def nil-var nil)

  (println (str "String: " str-var))
  (println (str "Number: " num-var))
  (println (str "Float: " float-var))
  (println (str "Boolean: " bool-var))
  (println (str "Nil: " nil-var))
  (println)

  ;; 集合操作
  (println "--- Collections ---")
  (def numbers [1 2 3 4 5 6 7 8 9 10])
  (def even-numbers (filter even? numbers))
  (def squared-numbers (map #(* % %) numbers))

  (println (str "Original: " numbers))
  (println (str "Even numbers: " (vec even-numbers)))
  (println (str "Squared: " (vec squared-numbers)))
  (printf "Sum: %d, Average: %.2f%n"
          (reduce + numbers)
          (/ (reduce + numbers) (count numbers)))
  (println)

  ;; Record 使用
  (println "--- Records ---")
  (def person1 (make-person "Alice" 30 "alice@example.com"))
  (def person2 (make-person "Bob" 25))

  (println (str "Person 1: " person1))
  (println (str "Person 2: " person2))
  (println (str "Person 1 greeting: " (person-greet person1)))
  (println (str "Is person 1 adult? " (person-adult? person1)))

  (def older-person1 (person-celebrate-birthday person1))
  (println (str "After birthday: " older-person1))
  (println)

  ;; 形状示例
  (println "--- Shapes ---")
  (def shapes [(->Circle 5.0) (->Rectangle 4.0 6.0) (->Triangle 3.0 4.0 5.0)])

  (doseq [shape shapes]
    (printf "%s -> Area: %.2f, Perimeter: %.2f%n"
            (description shape) (area shape) (perimeter shape)))
  (println)

  ;; 函数式编程
  (println "--- Functional Programming ---")
  (def doubled (process-collection numbers #(* 2 %)))
  (def greater-than-5 (filter-collection numbers #(> % 5)))

  (println (str "Original: " numbers))
  (println (str "Doubled: " (vec doubled)))
  (println (str "Greater than 5: " (vec greater-than-5)))
  (println)

  ;; 递归和数学
  (println "--- Recursion and Math ---")
  (println (str "Factorial of 5: " (factorial 5)))
  (println (str "Fibonacci sequence: " (fibonacci 10)))
  (println (str "Lazy Fibonacci (first 10): " (take 10 lazy-fibonacci)))
  (println)

  ;; 字符串处理
  (println "--- String Processing ---")
  (def text "The numbers are 123, 456 and 789 in this text.")
  (def extracted-numbers (extract-numbers text))

  (println (str "Original text: " text))
  (println (str "Extracted numbers: " extracted-numbers))
  (println (str "Contains 'numbers': " (str/includes? text "numbers")))
  (println (str "Uppercase: " (str/upper-case text)))
  (println)

  ;; 错误处理
  (println "--- Error Handling ---")
  (def emails ["valid@example.com" "invalid-email" "another@valid.com"])

  (doseq [email emails]
    (try
      (validate-email email)
      (println (str email ": Valid"))
      (catch IllegalArgumentException e
        (println (str email ": Error - " (.getMessage e))))))

  (try
    (validate-age -5)
    (catch IllegalArgumentException e
      (println (str "Age validation error: " (.getMessage e)))))
  (println)

  ;; Map 和 Set
  (println "--- Maps and Sets ---")
  (def grades {"Alice" 95, "Bob" 87, "Charlie" 92, "Diana" 88})

  (println "Student grades:")
  (doseq [[name grade] grades]
    (println (str "  " name ": " grade)))

  ;; 按成绩排序
  (def sorted-grades (sort-by val > grades))
  (println "Sorted by grade (descending):")
  (doseq [[name grade] sorted-grades]
    (println (str "  " name ": " grade)))
  (println)

  ;; 宏使用
  (println "--- Macros ---")
  (unless false
    (println "This should print (unless with false)"))

  (unless true
    (println "This should not print (unless with true)"))
  (println)

  ;; 并发
  (println "--- Concurrency ---")
  (def concurrent-results (process-numbers-concurrent [1 2 3 4 5]))
  (println (str "Concurrent squares: " (sort concurrent-results)))
  (println)

  ;; 集合分析
  (println "--- Collection Analysis ---")
  (def analysis (analyze-numbers numbers))
  (println "Number analysis:")
  (doseq [[key value] analysis]
    (println (str "  " (name key) ": " value)))
  (println)

  ;; 文件操作演示
  (println "--- File Operations (Demo) ---")
  (def temp-content ["This is a test file created by Clojure example."
                    "Line 2"
                    "Line 3"])

  (write-to-file "temp_clojure_example.txt" temp-content)

  (def lines (read-file-lines "temp_clojure_example.txt"))
  (println "File content:")
  (doseq [[i line] (map-indexed vector lines)]
    (println (str "  " (inc i) ": " line)))

  ;; 清理
  (clojure.java.io/delete-file "temp_clojure_example.txt")
  (println "Cleaned up temporary file")
  (println)

  ;; 状态管理
  (println "--- State Management ---")
  (add-person person1)
  (add-person person2)
  (println (str "People in store: " (count (list-people))))
  (println (str "Person with ID 1: " (get-person 1)))
  (println)

  ;; 序列操作
  (println "--- Sequence Operations ---")
  (def mixed-seq [1 "hello" 3.14 true nil])
  (println (str "Mixed sequence: " mixed-seq))
  (println (str "Types: " (map type mixed-seq)))
  (println (str "Non-nil values: " (filter some? mixed-seq)))
  (println)

  (println "=== Clojure Example Completed ==="))

;; 如果直接运行此文件
(when (= *file* (System/getProperty "babashka.file"))
  (-main))

;; 导出函数（如果作为库使用）
(def exports
  {:factorial factorial
   :fibonacci fibonacci
   :make-person make-person
   :person-greet person-greet})
