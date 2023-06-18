# Scielang
"Salve ad te" or "Hello!". As you might have guessed, my language is based on Latin, which is the language of science or the "scientific language"... Or "Scielang" :)
# About Scielang

## General information

This language is similar to the C language. Features of my language:
* works with numbers of type `int`
* does not work with arrays and pointers
* supports functions and their calls
* supports global variables
* supports `if`, `else`
* supports `while`

The program starts with the `main` function, which cannot be skipped. It supports calling functions whose definitions will be lower than the call location. 

## Introduction

The project consists of 4 parts: 
1) **frontend** compiles the source code into the [AST tree](#ast-tree) and performs some optimizations; 
2) **backend** from the AST tree, according to the [grammar](#grammar) of the language, restores it to the source code, which is then translated into custom assembly code; 
3) Custom assembly code is translated using [an assembler](https://github.com/Waloid24/Processor) into a set of commands for a virtual processor; 
4) [The virtual processor](https://github.com/Waloid24/Processor) executes this set of commands.

The intermediate representation in the AST-tree format allows you to do *cross compilation* with those languages that support the same standard.

> 4-ый пункт можно заменить на [JIT-компиляцию](./jit/README.md), которая сильно ускоряет выполнение программы.

## Build

To compile the project, type:
```
git clone git@github.com:Waloid24/Language.git
cd Scielang
make compile
```
All binaries will be in the `Scieland/bin` folder.
To clear a project after it has been built, type:
```
make clean
```

## Usage

To use only one stage of the execution of the source code file, you need to enter:
```
./bin/NAME_OF_STAGE --- FILE --- RESULT_FILE
```
where `FILE` is the file that was created at the previous stage or the source code file if you are working at the first stage (frontend). `RESULT_FILE` is the name of the output file, which then needs to be passed as an argument in the next step.

For example:
```
./bin/frontend --- factorial.txt --- frontResult.txt
```

If you want to execute the file immediately, that is, automatically go through all the steps, then enter:

```
make run FILE
```
where `FILE` is the name of one of the files located in the folder `./Scielang/tests`.

For example:
```
make run factorial.txt
```

## Syntax

### Types
There is only an integer type `int`, you **do not need to** specify it additionally. For example:
```
a assignare 5 ora
```

Despite this, the language supports increased accuracy: *three decimal places*. This happens due to the internal representation.

### Punctuators and operators

`salve`, `vale`
- used to group multiple instructions into one block;
- equivalent to `{`, `}` in C.

`aurora`, `umbra`
- used for passing arguments;
- used for grouping;
- equivalent to `()`, `)` in C.

`>=`, `<=`, `>`, `<`, `==`, `!=`
- equivalent to binary operators in С.

`assignare`
- assignment operator;
- equivalent to the `=` operator in C.

`ora`
- designation end of instruction;
- equivalent to `;` in C.

`-`
- unary minus.

### Mathematical operators

`mul`
- multiplication, equivalent to `*` in C.

`partem`
- division, equivalent to `/` in C.

`subtrahere`
- subtraction, equivalent to `-` in C.

`crescere`
- addition, equivalent to `+` in C.

`sqrt`
A built-in mathematical function that returns the square root of the argument passed to it. For example:
```
sqrt aurora some_number umbra
```

### Functions

In Scielang, there is no need to declare functions in advance, they are visible globally regardless of where they are defined in the program. Each function must have a `reditum` operator, which is an analog of `return` in C.

The definition of the function has the following form:
```
name_function aurora arguments umbra
salve
function body
vale
```
For example:
```
factorial aurora num umbra
salve
    inceptus aurora num == 0 umbra
    salve
        reditum 1 ora
    vale
    a assignare num mul factorial aurora num subtrahere 1 umbra ora
    reditum a ora
vale
```

The executable file must have the `main` function, since it is the entry point. It does not accept arguments.

### Embedded functions

External functions are not supported, only embedded ones. They do not require definition and are called as usual.

`print`
- accepts a single argument - an integer or a variable, which it then writes to standard output as a number.

`scan`
- accepts one argument from the standard input - the name of the variable, where the entered value will then be written.

### Global variables

Scielang supports global variables, but there are some differences from the C language.

- all global variables must be declared at the beginning of the program; 
- can only be initialized with a number.

For example:
```
SOME_NUMBER assignare 5 ora

main aurora umbra
salve
    some_func aurora umbra ora
    reditum 0 ora
vale

<you can't declare SOME_NUMBER here! Only at the very beginning of the program>

some_func aurora umbra
salve
    print aurora SOME_NUMBER umbra ora
    reditum 0 ora
vale
```

### Local variables

Similar to local variables in C. Can be declared anywhere in the function. For example:
```
main aurora umbra
salve
    a assignare 5 ora
    inceptus aurora a >= 0 umbra
    salve
        b assignare a crescere 5 ora
    vale
    cruciatus
    salve
        b assignare a ora
    vale
    reditum 0 ora
vale
```

### Loop

There is only one type of loops -- 'while' loop. In Scielang `cursus` stays for `while`.

```
...

iter assignare 5 ora

cursus aurora iter != 0 umbra
salve
    iter assignare iter subtrahere 1 ora
vale

...
```

### Conditional
There is only one type of conditional -- 'if' statement. In C if statement consists of 'if' branch and optional 'else' branch. Scielang uses the same form, with `inceptus` staying for `if` and `cruciatus` staying for `else`.

Example:

```
...

a assignare 0 ora
b assignare 0 ora
c assignare 0 ora

inceptus aurora a == 0 umbra
salve
    inceptus aurora b == 0 umbra
    salve
        inceptus aurora c != 0 umbra
        salve
            print aurora 666 umbra ora
            reditum 0 ora
        vale
        cruciatus
        salve
            print aurora 777 umbra ora
            reditum 0 ora
        vale
    vale
    cruciatus
    salve
        x assignare -c partem b ora
        print aurora x umbra ora
        reditum 0 ora
    vale
vale
```

## Grammar

The grammar of my language is very similar to the C language, and this is not accidental! I created it largely based on the existing one, you can see what happened below:

```
    Define          ::= TYPE_ID '(' {TYPE_ID {',' TYPE_ID}* }? ')' '{' Statement+ '}'

    Statement       ::= Assign          |
                        Conditional     |
                        Cycle           |
                        Terminational   |
                        Expression ';'

    Assign          ::= TYPE_ID '=' Expression ';'
    Terminational   ::= 'return' Expression ';'
    Cycle           ::= 'while'  '(' Expression ')' '{' {Statement}+ '}'
    Conditional     ::= 'if'     '(' Expression ')' '{' {Statement}+ '}' { else {'{' {Statement}+ '}'} }?

    Expression      ::= Boolean {['&&', '||'] Boolean}*

    Boolean         ::= Arithmetic {['==', '!=', '>=', '<=', '>', '<'] Arithmetic}*

    Arithmetic      ::= Term {['+', '-'] Term}*

    Term            ::= Primary {['*', '/'] Primary}*

    Primary         ::= '-' Primary             |
                        '(' Expression ')'      |
                        TYPE_NUM                |
                        TYPE_ID                 |
                        TYPE_ID  '(' {Expression {',' Expression}*}? ')' | // Function call
                        TYPE_EMB '(' {Expression {',' Expression}?}? ')'   // Embedded-function call
```

