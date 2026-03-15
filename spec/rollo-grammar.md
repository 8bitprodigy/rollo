# Spindle Formal Grammar
Version 1.3 (Draft)

This document defines a minimal EBNF style grammar for Spindle, the kernel of Rollo.

```ebnf
program         = { statement } ;

statement       = command newline
                | command continuation ;

command         = identifier { expression } @   (* arguments form an implicit heterogeneous list *)

expression      = assignment
                | conditional
                | application
                | callable_construction
                | access
                | binary
                | unary
                | reference
                | primary ;

assignment      = identifier "=" expression ;

conditional     = expression "?" block ;

application          = expression "." expression ;

callable_construction = expression ";" expression ;

access               = expression "@" expression ;

binary          = expression binary_operator expression ;

unary           = unary_operator expression ;

primary         = number
                | fraction
                | character
                | string
                | boolean
                | symbol_literal
                | symbol_intern
                | "nil"
                | identifier
                | collection
                | block
                | range
                | "(" expression ")" ;

collection      = "[" { collection_entry } "]" ;

collection_entry = map_entry | expression ;

map_entry       = key { ":" key } ":" expression ;

pair_entry      = key ":" expression ;

key             = identifier | string | symbol_literal ;

block           = "{" { statement } "}" ;

range           = expression ".." expression
                | expression "..." expression ;

symbol_literal  = "`" identifier ;

symbol_intern   = "$" ( identifier | string ) ;

reference       = "reference" expression ;

binary_operator = ":"
                | "^"
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
                | "or"
                | "pop"
                | "without" ;

unary_operator  = "!"
                | "not" ;

identifier      = letter { letter | digit | "_" } ;

number          = integer | decimal ;

integer         = digit { digit } ;

decimal         = digit { digit } "." digit { digit } ;

fraction        = integer "/" integer ;

character       = "'" unicode_char "'" ;

string          = '"' { string_char } '"' ;

boolean         = "true" | "false" ;

newline         = "\n" ;

continuation    = "\" "\n" ;

unicode_char    = (* any Unicode code point *) ;

string_char     = (* any Unicode code point except unescaped '"' *) ;

letter          = (* Unicode letter *) ;

digit           = "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;
```

## Notes

- This grammar is intentionally minimal.
- `.` constructs a callable with a fresh isolated scope. Pure — no access to outer scope.
- `;` constructs a callable with the current scope captured. Can read and mutate outer scope.
- Neither `.` nor `;` executes. Invocation happens only when a callable appears in command position.
- `@` is collection access — evaluates the right operand and uses it as a key or index.
- `reference` is a unary prefix operator that returns a named callable as a value without invoking it. Block literals never require `reference`.
- `()` denotes an immediately evaluated expression, not a collection literal.
- `[]` denotes all collection literals. Collection kind is resolved by content at parse time.
- A homogeneous array of character literals resolves to a string.
- All keyed entries produce a map. All unkeyed homogeneous entries produce an array. All unkeyed heterogeneous entries produce a list. Mixed keyed and unkeyed entries produce a table.
- `map_entry` allows multiple keys to share a value via chained `:` operators. A `pair_entry` carries exactly one key and one value.
- `:` is a binary operator producing a pair. Inside `[]` it appears as part of `map_entry` or `pair_entry` syntax.
- Range expressions `..` and `...` produce arrays of values. `..` is exclusive of the end, `...` is inclusive.
- `symbol_literal` with `` ` `` suppresses variable lookup and produces the symbol itself.
- `symbol_intern` with `$` interns a string or identifier as a symbol. `$` followed immediately by a digit is reserved for Rollo monetary literals and is not defined by Spindle.
- `pop` and `without` are word operators and participate in binary expression parsing.
- `fraction` is written as two integers separated by `/`. The parser resolves ambiguity with division by context.
- A newline terminates a statement unless a continuation rule applies. See the kernel specification for the full list of continuation rules. An explicit `\` at the end of a line always continues to the next line.
- Commands are not enumerated here because Spindle does not define a fixed command set. Commands are provided by Rollo libraries and runtime environments.
