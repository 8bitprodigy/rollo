# Spindle Parser and AST Notes
Version 1.2 (Draft)

This document outlines a practical parser model and abstract syntax tree for Spindle, the kernel of Rollo.

## 1. Goals

The parser should:

- preserve kernel semantics
- keep command parsing simple
- support newline sensitive command termination
- treat parentheses as immediate expression evaluation only
- preserve application and access semantics as distinct forms
- resolve bracket collections by content
- leave command meaning to the Rollo runtime

## 2. Parse Pipeline

A typical implementation may proceed in stages:

1. lex source into tokens
2. apply newline continuation rules
3. parse expressions according to precedence
4. parse application and access forms distinctly
5. resolve bracket collections
6. build command nodes
7. lower concrete syntax into AST nodes

## 3. Tokens

Core token classes:

- identifiers
- integers
- decimals
- fractions
- characters
- strings
- symbols (`` ` `` prefix)
- symbol interns (`$` prefix)
- operators
- delimiters
- newlines
- continuations (`\`)

Delimiters:

- `[` `]` — all collection literals
- `(` `)` — immediately evaluated expressions
- `{` `}` — code blocks

## 4. Newline Handling

A newline terminates a command unless:

- a delimiter is still open
- the next line begins with an operator
- a closing delimiter is followed by another token on the same line
- the command is still collecting argument expressions
- the line ends with `\`

The `\` continuation is explicit and always overrides newline termination. The other rules are implicit and context sensitive.

## 5. AST Nodes

Top level nodes:

```text
Program
Command
Binding
CallableConstructIsolated
CallableConstruct
Access
Reference
Conditional
StructuralMerge
BinaryOp
UnaryOp
Pop
Without
Range
Immediate
Block
Identifier
Literal
  NilLiteral
  BoolLiteral
  IntegerLiteral
  DecimalLiteral
  FractionLiteral
  CharacterLiteral
  StringLiteral
  SymbolLiteral
  SymbolIntern
String
Array
List
Pair
Map
Table
```

Notes:

- `Literal` covers all atomic value nodes. Subtypes are listed above.
- `SymbolLiteral` represents `` `name `` — the symbol itself, not a variable lookup.
- `SymbolIntern` represents `$name` or `$"string"` — intern and bind a symbol.
- `FractionLiteral` represents `1/3` with no surrounding whitespace.
- `CharacterLiteral` represents `'x'`.
- `CallableConstructIsolated` represents `.` — constructs a callable with a fresh isolated scope. Never executes.
- `CallableConstruct` represents `;` — constructs a callable with the current scope captured. Never executes.
- `Access` represents `@` — collection access by evaluated key or index.
- `Reference` represents `reference` — returns a named callable as a value without invoking it.
- `Pop` and `Without` are distinct nodes rather than generic binary ops because their semantics are collection-specific.
- `Block` contains a sequence of statements and introduces a scope.
- Invocation is not an AST node — it happens at evaluation time when a callable appears in command position.

## 6. Bracket Collection Resolution

When parsing `[` `]`:

1. parse entries from left to right
2. if all entries are character literals, lower to `String`
3. if all entries are keyed, lower to `Map`
4. if no entries are keyed and entries are homogeneous, lower to `Array`
5. if no entries are keyed and entries are heterogeneous, lower to `List`
6. if keyed and unkeyed entries are mixed, lower to `Table`

Examples:

```text
['h' 'e' 'l' 'l' 'o']   -> String
[1 2 3]                  -> Array
[1 "a" true]             -> List
[a:1 b:2]                -> Map
["a":"b":"c":42]         -> Map
[a:1 2]                  -> Table
```

Homogeneity for array resolution is determined by value type. Two values are homogeneous if they share the same Spindle type.

## 7. Application and Callable Construction Semantics in the AST

`CallableConstructIsolated` (`.`) must remain a distinct node and must not be lowered into a generic binary operation. It creates an isolated scope at evaluation time and this distinction must be preserved through all lowering passes.

`CallableConstruct` (`;`) must also remain distinct. It binds a map to a block producing a new callable but never executes. The evaluator must not treat it as an invocation.

`Reference` must remain distinct to signal that the named callable should be returned as a value rather than invoked.

## 8. Access Semantics in the AST

`Access` (`@`) is a distinct node from application and binary operations.

```text
Access
 ├ collection: expression
 └ key: expression
```

The key expression is evaluated at runtime. The resulting value is used to look up the entry — by symbol, string, integer index, or range.

Examples:

```text
person @ `name
```

AST:

```text
Access
 ├ Identifier(person)
 └ SymbolLiteral(name)
```

```text
list @ 0
```

AST:

```text
Access
 ├ Identifier(list)
 └ IntegerLiteral(0)
```

```text
list @ 1..3
```

AST:

```text
Access
 ├ Identifier(list)
 └ Range(exclusive, IntegerLiteral(1), IntegerLiteral(3))
```

## 9. Parentheses

Parentheses denote immediate evaluation of an expression, not a collection literal.

```text
(x + y)      -> Immediate(BinaryOp(+, Identifier(x), Identifier(y)))
(name)       -> Immediate(Identifier(name))
```

## 10. Command Argument Collection

A command line consists of a command identifier followed by zero or more argument expressions parsed greedily from left to right.

```text
show x + y
```

AST:

```text
Command
 ├ name: show
 └ args: List(BinaryOp(+, Identifier(x), Identifier(y)))
```

```text
show x + y z
```

AST:

```text
Command
 ├ name: show
 └ args: List(
     BinaryOp(+, Identifier(x), Identifier(y)),
     Identifier(z)
   )
```

```text
show "hello" 42
```

AST:

```text
Command
 ├ name: show
 └ args: List(StringLiteral("hello"), IntegerLiteral(42))
```

The implicit argument list does not appear in source syntax. It is introduced by the parser as a heterogeneous argument carrier.

## 11. Example AST

Source:

```text
person = [name:"Ada" age:32]

person . {
    show person @ `name
}
```

AST:

```text
Program
 ├ Binding
 │   ├ Identifier(person)
 │   └ Map
 │       ├ Pair(SymbolLiteral(name), StringLiteral("Ada"))
 │       └ Pair(SymbolLiteral(age), IntegerLiteral(32))
 └ CallableConstructIsolated
     ├ Identifier(person)
     └ Block
         └ Command
             ├ name: show
             └ args: List(
                 Access
                  ├ Identifier(person)
                  └ SymbolLiteral(name)
               )
```

## 12. Lowering Model

A parser may first build a concrete syntax tree and then lower to AST forms. Lowering passes may handle:

- bracket collection resolution
- command argument collection
- immediate expression unwrapping
- fraction literal disambiguation from division
- newline continuation joining

This keeps the parser simple and separates syntactic from semantic concerns.

## 13. Fraction Disambiguation

`1/3` with no surrounding whitespace is a `FractionLiteral`. `1 / 3` with whitespace is a `BinaryOp(/, IntegerLiteral(1), IntegerLiteral(3))`. The lexer resolves this by checking for whitespace around `/`.
