(* Standard ML Hello World Example *)
(* Demonstrates ML syntax and functional programming features *)

(* Simple hello world function *)
fun hello_world () =
    print "Hello, World from Standard ML!\n";

(* Variable bindings *)
val global_name = "SML Programmer";
val version = "1.0";
val pi = 3.14159;

(* Type definitions *)
datatype person = Person of {name: string, age: int, occupation: string};

(* Function definitions *)
fun factorial n =
    if n <= 1 then 1
    else n * factorial (n - 1);

fun fibonacci n =
    case n of
        0 => 0
      | 1 => 1
      | _ => fibonacci (n-1) + fibonacci (n-2);

(* Pattern matching examples *)
fun describe_number n =
    case n of
        0 => "zero"
      | 1 => "one"
      | 2 => "two"
      | _ => "other";

fun process_list [] = []
  | process_list (x::xs) = (x * 2) :: process_list xs;

(* Higher-order functions *)
fun apply_twice f x = f (f x);
fun add_one x = x + 1;

(* Currying and partial application *)
fun add x y = x + y;
val add_five = add 5;

(* List operations *)
fun sum_list [] = 0
  | sum_list (x::xs) = x + sum_list xs;

fun filter_even [] = []
  | filter_even (x::xs) =
    if x mod 2 = 0 then x :: filter_even xs
    else filter_even xs;

(* Records and tuples *)
val person_record = {name = "Alice", age = 30, occupation = "Developer"};

fun get_person_info (p: {name: string, age: int, occupation: string}) =
    (#name p, #age p, #occupation p);

(* Exception handling *)
exception InvalidInput of string;

fun safe_divide x y =
    if y = 0 then raise InvalidInput "Division by zero"
    else x div y;

(* Modules and signatures *)
signature MATH =
sig
    val pi: real
    val square: int -> int
    val cube: int -> int
end;

structure Math : MATH =
struct
    val pi = 3.14159;
    fun square x = x * x;
    fun cube x = x * x * x;
end;

(* Functors *)
signature SHOW =
sig
    type t
    val show: t -> string
end;

functor ShowList (S: SHOW) =
struct
    fun show_list [] = "[]"
      | show_list (x::xs) = S.show x ^ " :: " ^ show_list xs;
end;

(* String processing *)
fun string_length s = String.size s;
fun to_uppercase s = String.map Char.toUpper s;
fun concatenate s1 s2 = s1 ^ s2;

(* Input/Output *)
fun read_and_greet () =
    let val name = TextIO.inputLine TextIO.stdIn
    in
        case name of
            SOME s => print ("Hello, " ^ String.substring (s, 0, String.size s - 1) ^ "!\n")
          | NONE => print "Hello, anonymous!\n"
    end;

(* Main execution *)
val () = hello_world ();

(* Variable demonstrations *)
val () = print ("Hello, " ^ global_name ^ "!\n");
val () = print ("Version: " ^ version ^ "\n");
val () = print ("Pi is approximately: " ^ Real.toString pi ^ "\n");

(* Function calls *)
val () = print ("5! = " ^ Int.toString (factorial 5) ^ "\n");
val () = print ("Fibonacci(10) = " ^ Int.toString (fibonacci 10) ^ "\n");

(* Pattern matching *)
val () = print ("Number 3 is: " ^ describe_number 3 ^ "\n");

(* List processing *)
val numbers = [1, 2, 3, 4, 5];
val () = print ("Original list: " ^ String.concatWith ", " (map Int.toString numbers) ^ "\n");
val () = print ("Doubled: " ^ String.concatWith ", " (map Int.toString (process_list numbers)) ^ "\n");
val () = print ("Sum: " ^ Int.toString (sum_list numbers) ^ "\n");
val () = print ("Even numbers: " ^ String.concatWith ", " (map Int.toString (filter_even numbers)) ^ "\n");

(* Higher-order functions *)
val () = print ("Apply twice add_one to 3: " ^ Int.toString (apply_twice add_one 3) ^ "\n");

(* Records *)
val (name, age, occupation) = get_person_info person_record;
val () = print ("Person: " ^ name ^ ", " ^ Int.toString age ^ " years old, " ^ occupation ^ "\n");

(* Modules *)
val () = print ("5 squared: " ^ Int.toString (Math.square 5) ^ "\n");
val () = print ("5 cubed: " ^ Int.toString (Math.cube 5) ^ "\n");

(* String operations *)
val test_string = "Standard ML";
val () = print ("String: " ^ test_string ^ "\n");
val () = print ("Uppercase: " ^ to_uppercase test_string ^ "\n");
val () = print ("Length: " ^ Int.toString (string_length test_string) ^ "\n");
val () = print ("Concatenated: " ^ concatenate "Hello, " "World!" ^ "\n");

(* Exception handling *)
val () = (safe_divide 10 2; print "Division successful\n")
         handle InvalidInput msg => print ("Error: " ^ msg ^ "\n");

print "Standard ML example completed!\n";
