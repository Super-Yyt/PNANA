#!/usr/bin/env perl

# Perl 示例文件
# 展示 Perl 语言的各种语法特性

use strict;
use warnings;
use feature qw(say state);
use List::Util qw(sum min max);
use Scalar::Util qw(looks_like_number);

# 全局变量
our $global_counter = 0;
our @global_array = qw(apple banana cherry);

# 包定义
package Person;

# 构造函数
sub new {
    my ($class, $name, $age, $email) = @_;
    my $self = {
        name  => $name,
        age   => $age,
        email => $email,
        id    => generate_id()
    };
    bless $self, $class;
    return $self;
}

# 类方法
sub generate_id {
    state $id_counter = 0;
    return ++$id_counter;
}

# 实例方法
sub greet {
    my $self = shift;
    return "Hello, my name is $self->{name} and I'm $self->{age} years old!";
}

sub get_name {
    my $self = shift;
    return $self->{name};
}

sub get_age {
    my $self = shift;
    return $self->{age};
}

sub set_age {
    my ($self, $new_age) = @_;
    $self->{age} = $new_age;
}

sub celebrate_birthday {
    my $self = shift;
    $self->{age}++;
    return "Happy birthday! You're now $self->{age} years old.";
}

# 重载字符串化
use overload '""' => sub {
    my $self = shift;
    return "$self->{name} ($self->{age})";
};

package main;

# 标量变量
my $string_var = "Hello, Perl World!";
my $number_var = 42;
my $float_var = 3.14159;
my $boolean_var = 1;  # Perl 中没有真正的布尔值，使用 1/0

# 数组
my @numbers = (1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
my @fruits = qw(apple banana cherry date elderberry);
my @mixed = ($string_var, $number_var, \@numbers);

# 哈希
my %person_data = (
    name  => "Alice",
    age   => 30,
    email => "alice\@example.com",
    skills => ["Perl", "Python", "JavaScript"]
);

my %config = (
    debug => 1,
    timeout => 30,
    database => {
        host => "localhost",
        port => 5432,
        name => "myapp"
    }
);

# 函数定义
sub greet {
    my ($name) = @_;
    return "Hello, $name!";
}

sub calculate_area {
    my ($shape, $radius, $width, $height) = @_;

    if ($shape eq "circle") {
        return 3.14159 * $radius * $radius;
    } elsif ($shape eq "rectangle") {
        return $width * $height;
    } elsif ($shape eq "triangle") {
        return $width * $height / 2;
    } else {
        die "Unknown shape: $shape";
    }
}

# 递归函数
sub factorial {
    my ($n) = @_;
    return 1 if $n <= 1;
    return $n * factorial($n - 1);
}

# Fibonacci 数列
sub fibonacci {
    my ($n) = @_;
    return () if $n <= 0;
    return (0) if $n == 1;

    my @sequence = (0, 1);
    for my $i (2..$n-1) {
        push @sequence, $sequence[$i-1] + $sequence[$i-2];
    }
    return @sequence;
}

# 引用和解引用
sub process_array {
    my ($array_ref) = @_;
    my @doubled = map { $_ * 2 } @$array_ref;
    my @even = grep { $_ % 2 == 0 } @$array_ref;
    my $sum = sum(@$array_ref);

    return {
        original => $array_ref,
        doubled  => \@doubled,
        even     => \@even,
        sum      => $sum,
        average  => $sum / scalar(@$array_ref)
    };
}

# 正则表达式
sub validate_email {
    my ($email) = @_;
    my $email_regex = qr/^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$/;
    return $email =~ $email_regex;
}

sub extract_numbers {
    my ($text) = @_;
    my @numbers = $text =~ /(\d+)/g;
    return @numbers;
}

# 文件操作
sub read_file_lines {
    my ($filename) = @_;
    open my $fh, '<', $filename or die "Cannot open $filename: $!";
    my @lines = <$fh>;
    close $fh;
    chomp @lines;
    return @lines;
}

sub write_to_file {
    my ($filename, @content) = @_;
    open my $fh, '>', $filename or die "Cannot open $filename: $!";
    print $fh join("\n", @content);
    close $fh;
}

# 排序和搜索
sub custom_sort {
    my @array = @_;
    return sort { $a <=> $b } @array;  # 数值排序
}

sub find_max {
    my @array = @_;
    return max(@array);
}

# 面向对象使用
sub demonstrate_oop {
    my $person1 = Person->new("Alice", 30, "alice\@example.com");
    my $person2 = Person->new("Bob", 25);

    print "Person 1: $person1\n";
    print "Person 2: $person2\n";
    print "Person 1 greeting: ", $person1->greet(), "\n";

    $person1->celebrate_birthday();
    print "After birthday: $person1\n";
}

# 错误处理
sub safe_divide {
    my ($a, $b) = @_;
    eval {
        die "Division by zero" if $b == 0;
        return $a / $b;
    };
    if ($@) {
        warn "Error in division: $@";
        return undef;
    }
}

# 主程序
sub main {
    print "=== Perl Example ===\n\n";

    # 基本数据类型
    print "Basic Data Types:\n";
    print "String: $string_var\n";
    print "Number: $number_var\n";
    print "Float: $float_var\n";
    print "Boolean (1=true, 0=false): $boolean_var\n\n";

    # 数组操作
    print "Array Operations:\n";
    print "Numbers: @numbers\n";
    print "Fruits: @fruits\n";
    print "Array length: ", scalar(@numbers), "\n";
    print "First element: $numbers[0]\n";
    print "Last element: $numbers[-1]\n";
    print "Slice [2..4]: @numbers[2..4]\n\n";

    # 哈希操作
    print "Hash Operations:\n";
    print "Person name: $person_data{name}\n";
    print "Person age: $person_data{age}\n";
    print "Person skills: @{$person_data{skills}}\n";
    print "Hash keys: ", join(", ", keys %person_data), "\n";
    print "Hash values: ", join(", ", values %person_data), "\n\n";

    # 函数调用
    print "Function Calls:\n";
    print greet("Perl"), "\n";
    print "Circle area (r=5): ", calculate_area("circle", 5), "\n";
    print "Rectangle area (4x6): ", calculate_area("rectangle", 0, 4, 6), "\n";
    print "Factorial of 5: ", factorial(5), "\n";
    print "Fibonacci sequence: ", join(", ", fibonacci(10)), "\n\n";

    # 引用和复杂数据结构
    print "References and Complex Data:\n";
    my $result = process_array(\@numbers);
    print "Original: @{$result->{original}}\n";
    print "Doubled: @{$result->{doubled}}\n";
    print "Even numbers: @{$result->{even}}\n";
    print "Sum: $result->{sum}\n";
    print "Average: $result->{average}\n\n";

    # 正则表达式
    print "Regular Expressions:\n";
    my @emails = ("valid\@example.com", "invalid-email", "another\@valid.com");
    foreach my $email (@emails) {
        my $is_valid = validate_email($email) ? "Valid" : "Invalid";
        print "$email: $is_valid\n";
    }

    my $text = "The numbers are 123, 456 and 789 in this text.";
    my @extracted = extract_numbers($text);
    print "Extracted numbers from text: @extracted\n\n";

    # 控制结构
    print "Control Structures:\n";

    # if-elsif-else
    my $score = 85;
    if ($score >= 90) {
        print "Grade: A\n";
    } elsif ($score >= 80) {
        print "Grade: B\n";
    } elsif ($score >= 70) {
        print "Grade: C\n";
    } else {
        print "Grade: F\n";
    }

    # for 循环
    print "For loop (1 to 5): ";
    for my $i (1..5) {
        print "$i ";
    }
    print "\n";

    # foreach 循环
    print "Foreach loop on fruits: ";
    foreach my $fruit (@fruits) {
        print "$fruit ";
    }
    print "\n";

    # while 循环
    print "While loop: ";
    my $counter = 1;
    while ($counter <= 3) {
        print "$counter ";
        $counter++;
    }
    print "\n\n";

    # 面向对象
    print "Object-Oriented Programming:\n";
    demonstrate_oop();
    print "\n";

    # 错误处理
    print "Error Handling:\n";
    print "10 / 2 = ", safe_divide(10, 2), "\n";
    print "10 / 0 = ", safe_divide(10, 0), "\n\n";

    # 排序和搜索
    print "Sorting and Searching:\n";
    my @unsorted = (64, 34, 25, 12, 22, 11, 90);
    my @sorted = custom_sort(@unsorted);
    print "Original: @unsorted\n";
    print "Sorted: @sorted\n";
    print "Maximum: ", find_max(@unsorted), "\n\n";

    # 文件操作（演示）
    print "File Operations (Demo):\n";
    my $temp_content = "This is a temporary file created by Perl example.\nLine 2\nLine 3";
    write_to_file("temp_perl_example.txt", split(/\n/, $temp_content));
    print "Created temporary file: temp_perl_example.txt\n";

    # 清理临时文件
    unlink "temp_perl_example.txt";
    print "Cleaned up temporary file\n\n";

    # 特殊变量和列表上下文
    print "Special Variables and Contexts:\n";
    my @array = (1, 2, 3, 4, 5);
    my $scalar = @array;  # 标量上下文
    print "Array in scalar context: $scalar\n";
    print "Array length: ", scalar(@array), "\n";
    print "Last index: $#array\n";
    print "Array reference: ", \@array, "\n\n";

    print "=== Perl Example Completed ===\n";
}

# 脚本入口
if (__FILE__ eq $0) {
    main();
}

1;  # Perl 模块需要以真值结束
