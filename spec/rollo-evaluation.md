# Spindle Evaluation Model
Version 1.2 (Draft)

The Spindle evaluation model defines how expressions are executed and how values are produced during program execution.

Spindle evaluates programs as transformations of values. Commands consume values and produce new values.

## 1. Evaluation Units

The smallest executable unit in Spindle is an expression.

Expressions may be:

- literals
- symbols
- collections
- immediate expressions
- blocks
- application expressions
- access expressions
- operator expressions
- command invocations

Each expression evaluates to a value.

## 2. Evaluation Order

Spindle evaluates expressions left to right within the precedence rules defined by the operator table.

Immediate expressions, via `()`, are parsed and evaluated as ordinary enclosed expressions.

Application and access operators are evaluated before structural merge and ordinary binary operators. The infix arithmetic operator `pow` participates in ordinary arithmetic precedence.

## 3. Command Evaluation

Commands are evaluated by first collecting and evaluating their arguments.

The argument portion of a command line is treated semantically as an implicit heterogeneous list.

Arguments are parsed greedily as full expressions from left to right.

Example:

```text
show "hello" 42
```

is treated as a command receiving:

```text
["hello" 42]
```

Example:

```text
show x + y
```

is treated as a command with one argument:

```text
[(x + y)]
```

Example:

```text
show x + y z
```

is treated as a command with two arguments:

```text
[(x + y) z]
```

## 4. Immediate Evaluation

Parentheses force immediate evaluation of the enclosed expression.

```text
(x + y)
```

This form is an expression node, not a collection literal.

## 5. Block Evaluation

Blocks defer evaluation.

```text
{
    show "hello"
}
```

The block itself evaluates to a block value, not the result of its contents.

Execution occurs only when a command or applicator evaluates the block.

## 6. Symbol Resolution

A symbol evaluates to the value it is bound to in the current scope. If the symbol is unbound, it evaluates to itself as a symbol value.

```text
x = 10
show x       -> 10

show y       -> y  (unbound, evaluates to the symbol y itself)
```

The `` ` `` operator forces a symbol to evaluate to itself regardless of whether it is bound.

```text
x = 10
show `x      -> x  (the symbol x, not 10)
```

Symbol lookup walks the scope chain from the innermost scope outward.

## 7. Scope Model

By default, a block shares its parent scope. Reads and writes propagate to the enclosing scope.

```text
x = 10

{
    x = 20
    show x     -> 20
}

show x         -> 20
```

This makes control flow constructs like `if` work naturally without special scoping rules.

```text
x = integer 42

if (x > 0) {
    x = x + 25
}

show x         -> 67
```

## 8. Isolated Callable Construction Evaluation

`.` constructs a callable with a fresh isolated scope. It does not execute.

```text
greet = [name:"Ada" age:32] . {
    show name
}
```

Construction steps:

1. evaluate the left operand to a map
2. evaluate the right operand to a block
3. create a fresh isolated scope
4. bind map keys into that scope
5. return the callable — do not execute

The isolated scope has no access to the outer scope. Outer bindings are not visible and mutations cannot propagate outward.

```text
name = "outer"

greet = [name:"inner"] . {
    show name
}

greet               -> "inner"
show name           -> "outer"
```

## 9. Captured Callable Construction Evaluation

`;` constructs a callable with the current scope captured. It does not execute.

```text
myCommand = [x y] ; { x + y }
```

Construction steps:

1. evaluate the left operand to a map
2. evaluate the right operand to a block or callable
3. merge the map into the block's external argument map
4. capture the current scope
5. return the resulting callable — do not execute

When later invoked, the callable executes in its captured scope and can read and mutate it.

Re-applying in the current scope:

```text
newCommand = [] ; myCommand
```

## 10. Reference Evaluation

`reference` returns a named callable as a value without invoking it.

```text
myVal = reference myCommand
```

Without `reference`, a named identifier in command position is always invoked. Block literals are always values and never require `reference`.

## 11. Access Evaluation

`@` evaluates the right operand and uses the resulting value to look up an entry in the left operand collection.

```text
person @ `name     -> value keyed by symbol name
person @ "name"    -> value keyed by string "name"
person @ name      -> value keyed by whatever name holds
list   @ 0         -> value at index 0
list   @ 1..3      -> values at indices 1 and 2
```

If the key or index is not found, the result is `nil`.

## 12. Structural Evaluation

Structural operators transform values and produce new values.

```text
[1 2] ^ [3]
→ [1 2 3]

[a:1] ^ [b:2]
→ [a:1 b:2]

name:"Ada" ^ age:32
→ [name:"Ada" age:32]
```

## 13. Lazy vs Immediate Evaluation

Spindle is primarily eagerly evaluated.

Expressions are evaluated immediately except for:

- blocks
- values passed to commands that delay execution
- callables awaiting invocation

## 14. Value Immutability

Collection values are conceptually immutable once constructed. Operations produce new values rather than mutating existing ones.

```text
a = [1 2]
b = a ^ [3]
```

This preserves:

```text
a = [1 2]
b = [1 2 3]
```

Exceptions:

- `pop` mutatively removes elements from arrays and lists
- `<^` and `^>` mutatively merge into an existing collection

## 15. Program Result

A Spindle program returns the value of its final command or expression.

```text
x = 1
y = 2
x + y
```

Result:

```text
3
```

## 16. Determinism

Spindle programs are deterministic except where impure Rollo commands are used.

Purely structural programs given the same input values always produce the same result.

This supports:

- caching
- replay
- distributed evaluation
- deterministic builds

## 17. Relationship to Rollo

The Spindle evaluation model is independent of runtime environment. Rollo and its runtimes provide command implementations and system services, while the Spindle evaluation model remains consistent across all environments.
