# Xon Grammar Specification

Xon is JSON-compatible for data literals and extends that shape with an optional runtime expression layer.

## Core Data

```ebnf
root       ::= object | list
object     ::= '{' pair_list? '}'
pair_list  ::= pair (',' pair)* ','?
pair       ::= key ':' expr
            | 'let' IDENTIFIER '=' expr
            | 'const' IDENTIFIER '=' expr
key        ::= STRING | IDENTIFIER

list       ::= '[' value_list? ']'
value_list ::= value (',' value)* ','?

value      ::= STRING
            | NUMBER
            | TRUE
            | FALSE
            | NULL_VAL
            | object
            | list
            | expr
```

`STRING`, `NUMBER`, `TRUE`, `FALSE`, and `NULL_VAL` are parsed as literal values.

## Runtime Expressions (Round-1)

```ebnf
expr            ::= ternary_expr
ternary_expr    ::= nullish_expr
                 | nullish_expr '?' ternary_expr ':' ternary_expr
                 | 'if' '(' expr ')' ternary_expr 'else' ternary_expr

nullish_expr    ::= or_expr
                 | or_expr '??' or_expr

or_expr         ::= or_expr '||' and_expr | and_expr
and_expr        ::= and_expr '&&' eq_expr | eq_expr

eq_expr         ::= eq_expr '==' rel_expr | eq_expr '!=' rel_expr | rel_expr
rel_expr        ::= rel_expr '<' add_expr
                 | rel_expr '<=' add_expr
                 | rel_expr '>' add_expr
                 | rel_expr '>=' add_expr
                 | add_expr

add_expr        ::= add_expr '+' mul_expr
                 | add_expr '-' mul_expr
                 | mul_expr

mul_expr        ::= mul_expr '*' unary_expr
                 | mul_expr '/' unary_expr
                 | mul_expr '%' unary_expr
                 | unary_expr

unary_expr      ::= '!' unary_expr
                 | '+' unary_expr
                 | '-' unary_expr
                 | postfix_expr

postfix_expr    ::= postfix_expr '(' arg_list_opt ')'
                 | postfix_expr '.' IDENTIFIER
                 | primary_expr

primary_expr    ::= IDENTIFIER
                 | STRING
                 | NUMBER
                 | TRUE
                 | FALSE
                 | NULL_VAL
                 | object
                 | list
                 | '(' expr ')'
                 | '(' param_list_opt ')' '=>' expr

param_list_opt  ::= /* empty */ | param_list
param_list      ::= IDENTIFIER (',' IDENTIFIER)*
arg_list_opt    ::= /* empty */ | arg_list
arg_list        ::= expr (',' expr)*
```

## Comments

- `//` line comments
- `#` line comments
- `/* ... */` block comments

## Notes

- `src/xon.lemon` contains the full parser grammar and parser actions.
- `src/xon_api.c` enforces evaluation-time precedence and type errors while evaluating expression trees.
