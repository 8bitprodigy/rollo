# Rollo Kernel Specification

Version 1.1 (Draft)

The Rollo kernel defines the minimal parsing and evaluation rules from which all Rollo programs are built. It specifies values, containers, operators, scope, application, and command invocation. It does not define commands, control flow constructs, platform bindings, rendering systems, document structures, or standard library behavior.

Rollo is a value oriented language. The kernel is intentionally small and defines only what every Rollo implementation must share.

---

# I. Design Principles

**Everything is a value.**  
Numbers, strings, containers, code blocks, and callables are values.

**Programs are executable structure.**  
A Rollo program is a sequence of commands and expressions evaluated according to kernel rules.

**Delimiters have fixed meaning.**  
`[]` denotes all container literals, `()` denotes an expression evaluated immediately, and `{}` denotes a code block with deferred execution.

**Commands are extensible.**  
The kernel defines no commands. Commands are provided by libraries, runtime services, or foreign interfaces.

**Application is structural.**  
Structured values may be applied to executable values by binding fields into lexical names.

**The kernel is minimal.**  
Rendering, UI structure, networking, storage, and event systems belong outside the kernel.

---

# II. Program Structure

A Rollo program is a sequence of commands separated by newlines.

Example:

```text
show "hello"
show "world"
```

A newline terminates a command unless:

1. a delimiter remains open  
2. the following line begins with an operator  
3. a closing delimiter is followed by additional tokens  
4. the command is still collecting its argument sequence

Commas are ignored everywhere except inside string literals.

A command consists of a command name followed by zero or more arguments. Those arguments are treated semantically as an implicit ordered argument list. The argument list is not written explicitly in surface syntax.

Example:

```text
show "hello" 42
```

is treated as a command invocation with the heterogeneous argument sequence:

```text
["hello" 42]
```

## Command Argument Splitting

Command arguments are parsed greedily as full expressions from left to right.

This means an argument consumes as much syntax as needed to form a complete expression under the operator and delimiter rules before the next argument begins.

Examples:

```text
show x + y
```

is treated as:

```text
show (x + y)
```

and therefore has one argument.

```text
show x + y z
```

is treated as:

```text
show (x + y) z
```

and therefore has two arguments.

```text
show [1 2] ^ [3] name
```

is treated as:

```text
show ([1 2] ^ [3]) name
```

This rule keeps command invocation consistent with the ordinary expression grammar while preserving command oriented surface syntax.

---

# III. Values

Core value types:

- number
- string
- boolean
- nil
- array
- list
- map
- code block
- callable

Values may be stored, passed, and returned.

---

# IV. Containers

Rollo provides four container types.

## String

Immutable ordered sequence of characters.

```text
"hello"
```

Strings behave as sequence containers but remain a distinct type because they are extremely common and performance sensitive.

## Array

Ordered homogeneous sequence.

```text
[1 2 3]
```

Arrays are optimized for uniform data.

## List

Ordered heterogeneous sequence.

```text
[1 "hello" true]
```

Lists allow mixed data and flexible structural composition.

## Map

Key value container.

```text
[name:"Ada" age:32]
```

Multiple keys may inherit a value.

```text
["a":"b":"c":42]
```

## Bracket Container Resolution

Bracket containers resolve to array, list, or map by content.

- if all entries are keyed, the container is an ordered map
- if no entries are keyed and the entries are homogeneous, the container is an array
- if no entries are keyed and the entries are heterogeneous, the container is a list
- if keyed and unkeyed entries are mixed, the container remains ordered and is always addressable by index; keyed entries additionally expose their keys

Examples:

```text
[1 2 3]              -> array
[1 "a" true]         -> list
[name:"Ada" age:32]  -> map
["a":"b":"c":42]     -> map
[a:1 2]              -> mixed ordered container
```

## Container Design Note

Container types are distinguished by semantics, not by required storage layout. An implementation may represent them using native strings, dynamic arrays, tables, or other optimized structures. The kernel defines container behavior, not internal representation.

This separation allows implementations to optimize:

- strings using native string representations
- arrays using compact homogeneous dynamic arrays
- lists using heterogeneous arrays or tables
- maps using ordered associative data structures

Maps are ordered as well as key addressable. The distinction is also necessary for predictable container promotion when merging unlike container types.

---

# V. Container Promotion

When structurally combining unlike container types, promotion follows a deterministic hierarchy.

```text
string < array < list < map
```

The resulting container type is the higher type in the hierarchy.

Examples:

```text
"abc" ^ "def"        -> string
"abc" ^ ["d"]        -> array
[1 2] ^ [3 "x"]      -> list
[1 "x"] ^ [a:1]      -> map
```

---

# VI. Code Blocks

Code blocks defer evaluation.

```text
{
  show "hello"
}
```

They introduce a lexical scope and execute only when evaluated or applied.

Code blocks are executable values, not containers.

---

# VII. Immediate Evaluation

Parentheses denote immediate evaluation of an expression.

```text
(x + y)
```

The enclosed expression is parsed and evaluated immediately according to ordinary precedence and evaluation rules.

Parentheses do not denote container literals.

---

# VIII. Variables and Scope

Binding uses `=`.

```text
x = 10
title = "Example"
```

Scope is lexical. `{}` creates a new scope.

Unresolved variables evaluate to `nil`.

---

# IX. Operators

Kernel operators:

```text
=               bind
.               structural application
@               runtime application
?               conditional execution
^               structural merge / insertion
pop             array element removal, mutative
without         structural removal
+ - * / % pow   arithmetic
& | ~ ! << >>   bitwise
== != < > <= >= comparison
and or not      logical
```

`pow` is treated as an infix arithmetic operator.

```text
2 pow 8
```

---

# X. Operator Precedence

Highest to lowest:

```text
() [] {}
. @
^
* / %
+ -
<< >>
&
|
~
== != < > <= >=
and or not
?
=
```

---

# XI. Application

Rollo uses application operators to apply structured values to executable values.

| Operator | Meaning                |
| -------- | ---------------------- |
| `.`      | structural application |
| `@`      | runtime application    |

The core action of application is binding.

When a map is applied to a code block or callable, its keys are bound as lexical names within the application scope.

Example:

```text
person = [name:"Ada" age:32]

person . {
    show name
}
```

Within the applied block, `name` resolves to `"Ada"` and `age` resolves to `32`.

This allows maps to act as named argument carriers.

## Binding Collisions During Application

Application creates a fresh application scope.

Bindings introduced from the applied map populate that scope before the executable value is evaluated.

If an applied key has the same name as an outer binding, the applied binding shadows the outer binding within the application scope.

Example:

```text
name = "outer"

[name:"inner"] . {
    show name
}
```

Within the application, `name` resolves to `"inner"`.

Bindings created inside the block still follow ordinary lexical scope rules. A binding created inside the block may shadow an applied binding in an inner nested scope.

Example:

```text
[name:"inner"] . {
    show name
    {
        name = "nested"
        show name
    }
    show name
}
```

This evaluates conceptually as:

```text
"inner"
"nested"
"inner"
```

The kernel does not require application to mutate outer scopes. Application only establishes bindings for the duration of the applied evaluation.

### Structural application

`.` applies bindings without implying external runtime interaction by itself. It is intended for local structural evaluation and does not require mutation of outer scopes.

### Runtime application

`@` applies bindings in a runtime facing context. It is intended for cases where the applied executable may interact with services or other runtime systems. A higher level runtime may also allow `@` to participate in controlled outer scope modification or other contextual effects.

Examples:

```text
[name:"Ada"] . greet
[url:"https://example.com"] @ fetch
```

The exact behavior of application with non-map left operands is runtime defined, but map application is the kernel model.

---

# XII. Conditional Execution

```text
condition ? { block }
```

If the condition is truthy the block executes and returns its value.

Otherwise the falsy condition is returned.

---

# XIII. Structural Merge and Insertion

`^` structurally combines values.

```text
left ^ right
```

Maps merge keys:

```text
[a:1 b:2] ^ [b:3]
→ [a:1 b:3]
```

Arrays concatenate:

```text
[1 2] ^ [3 4]
→ [1 2 3 4]
```

Strings concatenate:

```text
"hello " ^ "world"
→ "hello world"
```

Lists concatenate according to list semantics.

```text
[1 "a"] ^ [2 "b"]
→ [1 "a" 2 "b"]
```

Mixed sequence merges promote according to the container hierarchy.

```text
[1 2] ^ [3 "x"]
→ [1 2 3 "x"]
```

---

# XIV. Arithmetic

Operators:

| Operator | Meaning        |
| -------- | -------------- |
| +        | addition       |
| -        | subtraction    |
| *        | multiplication |
| /        | division       |
| %        | remainder      |

---

# XV. Bitwise Operations

Bitwise operators apply to integers.

| Operator | Meaning     |
| -------- | ----------- |
| `&`      | AND         |
| `\|`     | OR          |
| `~`      | XOR         |
| `!`      | NOT         |
| `<<`     | left shift  |
| `>>`     | right shift |

Example:

```text
flags & mask
value << 1
```

---

# XVI. Comparison

```text
== != < > <= >=
```

Return boolean values.

---

# XVII. Logical Operations

Logical operators operate on truthiness.

| Operator | Meaning                        |
| -------- | ------------------------------ |
| `and`    | return right if left is truthy |
| `or`     | return left if truthy          |
| `not`    | logical negation               |

---

# XVIII. Truthiness

Falsy values:

```text
0
false
nil
uninitialized variables
```

All others are truthy.

---

# XIX. Commands

A command is any identifier not defined as an operator.

```text
show "hello"
fetch url
```

Commands may come from:

- standard libraries
- bootstrapped operations
- FFI bindings
- runtime services

Command arguments are heterogeneous and are most naturally modeled as an implicit list. A command line therefore behaves like a command name followed by an implicit list of argument expressions.

---

# XX. Kernel Boundary

The kernel defines:

- syntax
- values
- containers
- promotion
- operators
- binding
- application
- scope
- evaluation
- command argument collection

The kernel does not define:

- rendering
- document structure
- interface widgets
- networking
- storage
- event systems
- standard library commands

These belong to higher level specifications or runtime environments.

---

# XXI. Execution Model

Execution stages:

1. parse program
2. construct values
3. apply bindings
4. evaluate commands
5. produce resulting values

The kernel therefore functions as a minimal execution and transformation model rather than a complete application environment.

---

# XXII. Minimal Example

```text
person = [name:"Ada" age:32]

person . {
    show name
}
```

This example demonstrates binding, application, lexical scope, and command evaluation without depending on higher level structures outside the kernel.
