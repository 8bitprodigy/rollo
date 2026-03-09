# Rollo Parser and AST Notes
Version 1.1 (Draft)

This document outlines a practical parser model and abstract syntax tree for Rollo.

## 1. Goals

The parser should:

- preserve kernel semantics
- keep command parsing simple
- support newline sensitive command termination
- treat parentheses as immediate expression evaluation only
- preserve application semantics
- leave command meaning to the runtime

## 2. Parse Pipeline

A typical implementation may proceed in stages:

1. lex source into tokens
2. apply newline continuation rules
3. parse expressions according to precedence
4. parse application forms distinctly
5. resolve bracket containers
6. build command nodes
7. lower concrete syntax into AST values

## 3. Tokens

Core token classes include:

- identifiers
- numbers
- strings
- operators
- delimiters
- newlines
- colons

Suggested delimiters:

- `[` `]` for all container literals
- `(` `)` for immediately evaluated expressions
- `{` `}` for code blocks

## 4. Newline Handling

A newline terminates a command unless:

- a delimiter is still open
- the next line begins with an operator
- a closing delimiter is followed by another token on the same line
- the command is still collecting argument expressions

This suggests a parser should not treat raw newlines as unconditional separators.

## 5. AST Shape

Suggested top level nodes:

```text
Program
Command
Binding
ApplyStructural
ApplyRuntime
Conditional
Merge
BinaryOp
UnaryOp
Immediate
Block
Identifier
Literal
Array
List
Map
Callable
```

## 6. Example AST

Source:

```text
person = [name:"Ada"]

person . {
    show name
}
```

Conceptual AST:

```text
Program
 ├ Binding
 │   ├ Identifier(person)
 │   └ Map(name -> "Ada")
 └ ApplyStructural
     ├ Identifier(person)
     └ Block
         └ Command(show, Identifier(name))
```

## 7. Application Semantics in the AST

Application should remain visible in the AST and should not be lowered immediately into a generic binary operation.

This matters because:

- map application introduces lexical bindings
- runtime application may establish a runtime context
- applicators are more than infix syntax sugar

## 8. Parentheses

In the current kernel draft, parentheses denote immediate evaluation of an expression.

Examples:

```text
(x + y)      -> Immediate(BinaryOp(...))
(name)       -> Immediate(Identifier(name))
```

Implementations should therefore not reinterpret parenthesized forms as container literals.

## 9. Bracket Container Resolution

When parsing `[` `]`:

1. parse entries from left to right
2. detect whether any keyed entry appears
3. if all entries are keyed, lower to `Map`
4. if no entries are keyed, inspect element uniformity
5. if entries are homogeneous, lower to `Array`
6. if entries are heterogeneous, lower to `List`
7. if keyed and unkeyed entries are mixed, lower to an ordered mixed container form or to the runtime's chosen ordered container representation

Examples:

```text
[1 2 3]        -> Array
[1 "a"]        -> List
[a:1 b:2]      -> Map
[a:1 2]        -> Mixed ordered container
```

## 10. Command Argument Collection

A command line consists of a command identifier followed by zero or more argument expressions.

Arguments should be parsed greedily as full expressions from left to right.

Examples:

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
 └ args:
     List(
       BinaryOp(+, Identifier(x), Identifier(y)),
       Identifier(z)
     )
```

```text
show "hello" 42
```

A reasonable AST shape is:

```text
Command
 ├ name: show
 └ args: List(String("hello"), Number(42))
```

This does not mean the list delimiter appears in source. It is an implicit heterogeneous argument carrier introduced by parsing.

## 11. Lowering Model

A parser may first build a concrete syntax tree and then lower to AST forms such as:

- command invocation
- container construction
- immediate evaluation
- application
- operator application

This helps keep the parser simple and implementation friendly.
