# Rollo Formal Grammar
Version 1.1 (Draft)

This document defines a minimal EBNF style grammar for Rollo.

```ebnf
program        = { command newline } ;

command        = identifier { expression } ;   (* arguments form an implicit heterogeneous list *)

expression     = assignment
               | conditional
               | application
               | binary
               | unary
               | primary ;

assignment     = identifier "=" expression ;

conditional    = expression "?" block ;

application    = expression "." expression
               | expression "@" expression ;

binary         = expression binary_operator expression ;

unary          = unary_operator expression ;

primary        = number
               | string
               | boolean
               | "nil"
               | identifier
               | container
               | block
               | "(" expression ")" ;

container      = "[" { container_entry } "]" ;

container_entry = map_entry | expression ;

map_entry      = key ":" expression ;

key            = identifier | string ;

block          = "{" { command newline } "}" ;

binary_operator = "^"
                | "+"
                | "-"
                | "*"
                | "/"
                | "%"
                | "pow"
                | "&"
                | "|"
                | "~"
                | "<<"
                | ">>"
                | "=="
                | "!="
                | "<"
                | ">"
                | "<="
                | ">="
                | "and"
                | "or" ;

unary_operator = "!"
               | "not" ;

identifier     = letter { letter | digit | "_" } ;

number         = digit { digit } [ "." digit { digit } ] ;

string         = '"' { character } '"' ;

boolean        = "true" | "false" ;

newline        = "\n" ;
```

## Notes

- This grammar is intentionally minimal.
- Application operators are modeled separately because they have applicator semantics rather than ordinary binary operator semantics.
- `()` denotes an immediately evaluated expression, not a list literal.
- `[]` denotes all container literals.
- Bracket containers resolve by content: all keyed entries form ordered maps, unkeyed homogeneous entries form arrays, unkeyed heterogeneous entries form lists.
- Mixed keyed and unkeyed entries are allowed and remain index addressable; keyed entries additionally expose keys.
- Newline continuation rules from the kernel specification still apply.
- Commands are not enumerated here because the kernel does not define a fixed command set.
