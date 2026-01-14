;; Common Lisp Hello World Example
;; Demonstrates basic Lisp syntax and features

;; Simple hello world function
(defun hello-world ()
  "Prints a greeting message"
  (format t "Hello, World from Common Lisp!~%"))

;; Variable definitions
(defvar *global-name* "Lisp Programmer"
  "A global variable example")

(defparameter *version* "1.0"
  "Program version")

;; Constants
(defconstant +pi+ 3.14159
  "Mathematical constant pi")

;; Struct definition
(defstruct person
  name
  age
  occupation)

;; Macro definition
(defmacro when-let ((var expr) &body body)
  "Execute body when expr is not nil, binding result to var"
  `(let ((,var ,expr))
     (when ,var ,@body)))

;; Example functions
(defun factorial (n)
  "Calculate factorial recursively"
  (if (<= n 1)
      1
      (* n (factorial (1- n)))))

(defun fibonacci (n)
  "Calculate nth Fibonacci number"
  (cond ((= n 0) 0)
        ((= n 1) 1)
        (t (+ (fibonacci (- n 1))
              (fibonacci (- n 2))))))

;; List manipulation
(defun process-list (lst)
  "Demonstrate list processing"
  (format t "Original list: ~A~%" lst)
  (format t "Length: ~A~%" (length lst))
  (format t "First element: ~A~%" (first lst))
  (format t "Rest: ~A~%" (rest lst))
  (format t "Reversed: ~A~%" (reverse lst)))

;; Object-oriented example
(defclass animal ()
  ((name :initarg :name :accessor animal-name)
   (species :initarg :species :accessor animal-species)))

(defclass dog (animal)
  ((breed :initarg :breed :accessor dog-breed)))

(defmethod speak ((animal animal))
  "Generic speak method"
  (format t "~A makes a sound~%" (animal-name animal)))

(defmethod speak ((dog dog))
  "Dog specific speak method"
  (format t "~A barks: Woof!~%" (animal-name dog)))

;; Main function
(defun main ()
  "Main entry point"
  (hello-world)

  ;; Variable usage
  (format t "Hello, ~A!~%" *global-name*)
  (format t "Version: ~A~%" *version*)
  (format t "Pi is approximately: ~A~%" +pi+)

  ;; Struct usage
  (let ((p (make-person :name "Alice" :age 30 :occupation "Developer")))
    (format t "Person: ~A, ~A years old, ~A~%"
            (person-name p) (person-age p) (person-occupation p)))

  ;; Macro usage
  (when-let (result (factorial 5))
    (format t "5! = ~A~%" result))

  ;; List processing
  (process-list '(1 2 3 4 5))

  ;; OOP example
  (let ((my-dog (make-instance 'dog
                               :name "Buddy"
                               :species "Canine"
                               :breed "Golden Retriever")))
    (speak my-dog))

  ;; Fibonacci
  (format t "Fibonacci(10) = ~A~%" (fibonacci 10))

  ;; String manipulation
  (let ((str "Common Lisp"))
    (format t "String: ~A~%" str)
    (format t "Uppercase: ~A~%" (string-upcase str))
    (format t "Length: ~A~%" (length str))))

;; Execute main when script is loaded
(main)
