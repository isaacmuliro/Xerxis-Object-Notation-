# Xon Grammar Specification

## EBNF Notation

```ebnf
root       ::= object | list

object     ::= '{' pair_list? '}'
pair_list  ::= pair (',' pair)* ','?
pair       ::= key ':' value
key        ::= STRING | IDENTIFIER

list       ::= '[' value_list? ']'
value_list ::= value (',' value)* ','?

value      ::= STRING | NUMBER | BOOL | NULL | object | list

STRING     ::= '"' char* '"'
NUMBER     ::= DECIMAL | HEXADECIMAL
DECIMAL    ::= '-'? [0-9]+ ('.' [0-9]+)?
HEXADECIMAL ::= '0' [xX] [0-9a-fA-F]+
BOOL       ::= 'true' | 'false'
NULL       ::= 'null'
IDENTIFIER ::= [a-zA-Z_][a-zA-Z0-9_]*
```

## Grammar Details

### Terminals
- `LBRACE` - `{`
- `RBRACE` - `}`
- `LBRACKET` - `[`
- `RBRACKET` - `]`
- `COLON` - `:`
- `COMMA` - `,`
- `STRING` - Quoted text
- `IDENTIFIER` - Unquoted keys
- `NUMBER` - Decimal or hex numbers
- `TRUE` - Boolean true
- `FALSE` - Boolean false
- `NULL_VAL` - Null value

### Non-Terminals
- `root` - Top-level entry point
- `object` - Key-value pairs
- `list` - Ordered collections
- `pair` - Key-value association
- `value` - Any valid data type

See `src/xon.lemon` for the complete Lemon grammar specification.
