# Scielang
"Salve ad te" or "Hello!". As you might have guessed, my language is based on Latin, which is the language of science or the "scientific language"... Or "Scielang" :)
# About Scielang
## General information
This language is similar to the C language. Features of my language:
* works with numbers of type `int`
* does not work with arrays and pointers
* ssupports functions and their calls
* supports global variables
* supports `if`, `else`
* supports `while`    
Program starts with the `main` function, which cannot be called. Functions can be used even in situations where they are defined after the called place. 


## Grammar
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

