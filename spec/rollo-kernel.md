# Spindle Kernel Specification
Version 1.3 (Draft)

Spindle defines the minimal parsing and evaluation rules from which Rollo is built. It specifies values, collections, operators, scope, application, and command invocation. It does not define commands, control flow constructs, platform bindings, rendering systems, document structures, or standard library behavior.

Spindle is a value oriented kernel. It is intentionally small and defines only what every Rollo implementation must share.

---

# I. Design Principles

**Everything is a value.**
Numbers, strings, collections, code blocks, and characters are values.

**Programs are executable structure.**
A Spindle program is a sequence of commands and expressions evaluated according to kernel rules.

**Delimiters have fixed meaning.**
`[]` denotes all collection literals, `()` denotes an expression evaluated immediately, and `{}` denotes a code block with deferred execution.

**Commands are extensible.**
The kernel defines no commands. Commands are provided by Rollo libraries, runtime services, or foreign interfaces.

**Application is structural.**
Structured values may be applied to executable values by binding fields into lexical names.

**The kernel is minimal.**
Rendering, UI structure, networking, storage, and event systems belong outside Spindle and are defined by Rollo and its runtimes.

---

# II. Program Structure

A Spindle program is a sequence of commands separated by newlines.

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
5. the line ends with `\`

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

Scalar types:

- nil
- boolean
- number (integer, decimal, fraction)
- character
- symbol

Collection types:

- string
- array
- list
- pair
- map
- table

Executable types:

- block

Values may be stored, passed, and returned.

---

# IV. Collections

All collection literals use `[]`. The kind of collection is resolved by content.

## String

Immutable ordered sequence of characters.

```text
"hello"
```

Strings behave as sequence collections but remain a distinct type because they are extremely common and performance sensitive.

A string may also be constructed from a homogeneous array of character literals:

```text
['h' 'e' 'l' 'l' 'o']   -> "hello"
```

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

## Pair

A single key-value entry, produced by the `:` operator.

```text
name:"Ada"
```

A pair is its own collection type — not a sequence and not a map. It carries exactly one key and one value. Pairs are the building blocks of maps.

## Map

An ordered associative container of key-value pairs.

```text
[name:"Ada" age:32]
```

Maps are ordered as well as key addressable. They are the primary carrier for named application bindings.

Multiple keys may share a value using chained `:` operators.

```text
["a":"b":"c":42]
```

## Table

An ordered container supporting both keyed and unkeyed entries. Produced when a sequence and a map are structurally merged.

```text
[a:1 2 b:3]
```

All entries in a table are addressable by index. Keyed entries are additionally addressable by key.

## Bracket Collection Resolution

Bracket collections resolve by content:

- if no entries are keyed and all entries are characters, the collection is a string
- if all entries are keyed, the collection is an ordered map
- if no entries are keyed and the entries are homogeneous, the collection is an array
- if no entries are keyed and the entries are heterogeneous, the collection is a list
- if keyed and unkeyed entries are mixed, the collection is a table

Examples:

```text
['h' 'e' 'l' 'l' 'o']   -> "hello"
[1 2 3]                  -> array
[1 "a" true]             -> list
[name:"Ada" age:32]      -> map
["a":"b":"c":42]         -> map
[a:1 2]                  -> table
```

## Collection Design Note

Collection types are distinguished by semantics, not by required storage layout. An implementation may represent them using native strings, dynamic arrays, or ordered associative data structures. The kernel defines collection behavior, not internal representation.

---

# V. Collection Promotion

When structurally combining unlike collection types, promotion follows a deterministic hierarchy.

```text
string < array < list < pair < map < table
```

The resulting collection type is the higher type in the hierarchy.

Examples:

```text
"abc" ^ "def"        -> string
"abc" ^ ["d"]        -> list
[1 2] ^ [3 "x"]      -> list
[1 "x"] ^ [a:1]      -> table
[a:1] ^ [b:2]        -> map
name:"Ada" ^ age:32  -> map
[a:1] ^ [1 2]        -> table
```

---

# VI. Blocks

Blocks defer evaluation.

```text
{
    show "hello"
}
```

A block introduces a lexical scope and executes only when applied or invoked.

Blocks are executable values, not collections.

A block with a bound external argument map is referred to as a callable, but the underlying type remains block.

---

# VII. Immediate Evaluation

Parentheses denote immediate evaluation of an expression.

```text
(x + y)
```

The enclosed expression is parsed and evaluated immediately according to ordinary precedence and evaluation rules.

Parentheses do not denote collection literals.

---

# VIII. Variables and Scope

Binding uses `=`.

```text
x = 10
title = "Example"
```

By default, a block shares its parent scope. Reads and writes inside the block propagate to the enclosing scope.

```text
x = 10

{
    x = 20
}

show x      -> 20
```

Unresolved variables evaluate to `nil`.

---

# IX. Operators

| Operator | Meaning |
| -------- | ------- |
| `=` | bind |
| `:` | pair construction |
| `.` | callable construction, isolated scope |
| `;` | callable construction, captured scope |
| `@` | collection access |
| `?` | conditional execution |
| `^` | structural merge |
| `` ` `` | symbol literal |
| `$` | intern string as symbol |
| `reference` | return callable value without invoking |
| `..` | exclusive range |
| `...` | inclusive range |
| `pop` | mutating element removal |
| `without` | non-mutating structural removal |
| `+ - * / % pow` | arithmetic |
| `& \| ~ ! << >>` | bitwise |
| `== != < > <= >=` | comparison |
| `and or not` | logical |

`pow` is treated as an infix arithmetic operator.

```text
2 pow 8
```

---

# X. Operator Precedence

Highest to lowest:

```text
() [] {}
` $ reference
. ; @
:
.. ...
^
pop  without
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

# XI. Application and Access

# XI. Callable Construction and Access

Both `.` and `;` construct callables. Neither executes. The distinction is scope:

| Operator | Meaning |
| -------- | ------- |
| `.` | callable construction, isolated scope |
| `;` | callable construction, captured scope |

## Isolated Callable Construction

`.` constructs a callable with a fresh isolated scope populated only by the applied map. The callable has no access to the outer scope and cannot mutate it. This is the pure form.

```text
person = [name:"Ada" age:32]

greet = person . {
    show name
}
```

The callable `greet` has `name` and `age` bound from `person` and no access to anything else.

When invoked, it executes in that isolated scope:

```text
greet
```

If an outer variable needs to be available inside, it must be passed explicitly through the map:

```text
x = 10

increment = [x: x] . {
    show x
}
```

### Binding Collisions

If a map key has the same name as an outer binding, the map binding takes precedence within the isolated scope.

```text
name = "outer"

greet = [name:"inner"] . {
    show name   -> "inner"
}
```

### Pure Function Pattern

`.` is the primary way to define pure functions in Spindle — callables that depend only on their explicit inputs and produce no side effects on the outer scope.

## Captured Callable Construction

`;` constructs a callable with the current scope captured. When invoked, the callable can read and mutate the captured scope.

```text
myCommand = [x y] ; { x + y }
```

Re-applying in the current scope to capture it:

```text
newCommand = [] ; myCommand
```

This produces a new callable identical to `myCommand` but with the current scope captured instead of the original.

Overriding specific arguments:

```text
newCommand = [x: 10] ; myCommand
```

## Reference

`reference` returns a named callable as a value without invoking it.

```text
reference myCommand    -> the callable value itself
```

Without `reference`, a named identifier in command position is always invoked. `reference` suppresses invocation so the callable can be passed as a value, stored, or re-applied.

```text
myVal = reference myCommand    -> assigns the callable, does not invoke
callbacks = [reference increment reference decrement]
```

Block literals never require `reference` since they are always values and never auto-invoke.

## Access

`@` accesses a collection entry using the evaluated value of its right operand.

```text
collection @ expression
```

The right operand is evaluated normally. The resulting value is used as the key or index:

```text
person @ `name       -> value keyed by symbol name
person @ "name"      -> value keyed by string "name"
person @ name        -> value keyed by whatever value name holds
list   @ 0           -> value at index 0
list   @ 1..3        -> values at indices 1 and 2
list   @ 1...3       -> values at indices 1, 2, and 3
```

---

# XII. Conditional Execution

```text
condition ? { block }
```

If the condition is truthy the block executes and returns its value.

Otherwise the value the condition evaluated to is returned.

---

# XIII. Structural Merge

`^` structurally combines values.

```text
left ^ right
```

### Characters

Characters merge into strings.

```text
'a' ^ 'b'        -> "ab"
'a' ^ "bc"       -> "abc"
```

### Strings

Strings concatenate.

```text
"hello " ^ "world"
-> "hello world"
```

When merged with a non-string collection, a string explodes into its constituent characters.

```text
"ab" ^ [1 2]
-> ['a' 'b' 1 2]
```

### Arrays

Arrays concatenate when homogeneous. Mixed content promotes to list.

```text
[1 2] ^ [3 4]    -> [1 2 3 4]
[1 2] ^ [3 "x"]  -> [1 2 3 "x"]
```

### Lists

Lists concatenate.

```text
[1 "a"] ^ [2 "b"]
-> [1 "a" 2 "b"]
```

### Pairs

Two pairs produce a map. If the keys are identical, the right value overwrites the left.

```text
name:"Ada" ^ age:32          -> [name:"Ada" age:32]
name:"Ada" ^ name:"Lovelace" -> name:"Lovelace"
```

A pair merged with a sequence produces a table.

```text
name:"Ada" ^ [1 2]           -> table
```

### Maps

Maps merge keys. A right-side key overwrites a matching left-side key.

```text
[a:1 b:2] ^ [b:3 c:4]
-> [a:1 b:3 c:4]
```

A map merged with any sequence produces a table.

```text
[a:1 b:2] ^ [1 2]
-> table
```

### Tables

Any merge involving a table produces a table.

### Scalars

Scalars are promoted into a collection when merged. The resulting type depends on the scalar types involved — two characters produce a string, two numbers produce an array, mixed scalars produce a list.

```text
1 ^ 2      -> [1 2]
'a' ^ 'b'  -> "ab"
1 ^ "x"    -> [1 "x"]
```

---

# XIV. Ranges

The range operators produce an array of values from two numeric or character operands.

| Operator | Meaning       |
| -------- | ------------- |
| `..`     | exclusive end |
| `...`    | inclusive end |

```text
1..5          -> [1 2 3 4]
1...5         -> [1 2 3 4 5]
'a'...'e'     -> ['a' 'b' 'c' 'd' 'e']
```

Ranges may be used as operands to `pop` and `without`, and may participate in iteration.

---

# XV. Collection Removal

## pop

`pop` mutatively removes one or more elements from an array or list and returns the removed value or values.

```text
array pop 0          -> removes and returns first element
array pop [0 2]      -> removes and returns elements at indices 0 and 2
array pop 1..3       -> removes and returns elements at indices 1 and 2
array pop 1...3      -> removes and returns elements at indices 1, 2, and 3
```

`pop` is not valid on maps or tables.

## without

`without` non-mutatively produces a new collection with the specified entries removed.

```text
array without 0           -> new array without element at index 0
map without `name         -> new map without the entry keyed by symbol name
map without $nameVar      -> new map without the entry keyed by the symbol in nameVar
map without [`name `age]  -> new map without entries keyed by name and age
array without 1..3        -> new array without elements at indices 1 and 2
```

`without` works on all collection types.

---

# XVI. Symbol Operations

## Symbol Literal

The `` ` `` prefix operator suppresses variable lookup and produces the symbol itself.

```text
`name    -> the symbol name, not the variable name
```

## Symbol Interning

The `$` prefix operator interns a string or identifier as a symbol and binds it to the current scope. If the string does not meet identifier rules, an error is produced.

```text
$name            -> interns the string held in variable name as a symbol
$"mySymbol"      -> interns the string literal as a symbol
```

`$` followed immediately by a digit with no whitespace is reserved for monetary literals in Rollo and is not defined at the Spindle kernel level.

---

# XVII. Arithmetic

| Operator | Meaning        |
| -------- | -------------- |
| +        | addition       |
| -        | subtraction    |
| *        | multiplication |
| /        | division       |
| %        | remainder      |
| pow      | exponentiation |

---

# XVIII. Bitwise Operations

Bitwise operators apply to integers.

| Operator | Meaning     |
| -------- | ----------- |
| `&`      | AND         |
| `\|`     | OR          |
| `~`      | XOR         |
| `!`      | NOT         |
| `<<`     | left shift  |
| `>>`     | right shift |

---

# XIX. Comparison

```text
== != < > <= >=
```

Return boolean values.

---

# XX. Logical Operations

| Operator | Meaning |
| -------- | ------- |
| `and`    | return right if left is truthy, otherwise return left |
| `or`     | return left if truthy, otherwise return evaluation of right |
| `not`    | logical negation |

---

# XXI. Truthiness

Falsy values:

```text
0
false
nil
uninitialized values
```

All others are truthy.

---

# XXII. Commands

A command is any identifier not defined as an operator or keyword.

```text
show "hello"
fetch url
```

Commands may come from:

- Rollo standard libraries
- bootstrapped operations
- FFI bindings
- runtime services

Command arguments are treated as an implicit heterogeneous list.

---

# XXIII. Kernel Boundary

Spindle defines:

- syntax
- values
- collections
- promotion
- operators
- binding
- application and access
- scope
- evaluation
- command argument collection

Spindle does not define:

- rendering
- document structure
- interface widgets
- networking
- storage
- event systems
- standard library commands

These belong to Rollo and its runtime environments.

---

# XXIV. Execution Model

Execution stages:

1. parse program
2. construct values
3. apply bindings
4. evaluate commands
5. produce resulting values

---

# XXV. Minimal Example

```text
person = [name:"Ada" age:32]

person . {
    show name
}

show person @ `name
```

This example demonstrates binding, structural application, isolated scope, collection access, and command evaluation without depending on higher level structures outside the Spindle kernel.
