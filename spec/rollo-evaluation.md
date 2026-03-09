# Rollo Evaluation Model
Version 1.1 (Draft)

The Rollo evaluation model defines how expressions are executed and how values are produced during program execution.

Rollo evaluates programs as transformations of values. Commands consume values and produce new values.

## 1. Evaluation Units

The smallest executable unit in Rollo is an expression.

Expressions may be:

- literals
- variables
- containers
- immediate expressions
- code blocks
- application expressions
- operator expressions
- command invocations

Each expression evaluates to a value.

## 2. Evaluation Order

Rollo evaluates expressions left to right within the precedence rules defined by the operator table.

Immediate expressions, via `()`, are parsed and evaluated as ordinary enclosed expressions.

Application operators are evaluated before structural merge and ordinary binary operators. The infix arithmetic operator `pow` participates in ordinary arithmetic precedence.

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

This form is an expression node, not a container literal.

## 5. Block Evaluation

Code blocks defer evaluation.

```text
{
    show "hello"
}
```

The block itself evaluates to a block value, not the result of its contents.

Execution occurs only when a command or applicator evaluates the block.

## 6. Variable Resolution

Variable lookup follows lexical scope.

```text
x = 10

{
    x = 20
    show x
}

show x
```

This evaluates to `20` inside the block and `10` outside the block.

## 7. Application Evaluation

Application is a first class part of evaluation.

### Structural application

```text
[name:"Ada" age:32] . {
    show name
}
```

Evaluation steps:

1. evaluate the left operand
2. evaluate the right operand to an executable value
3. create an application scope
4. bind map keys from the left operand into that scope
5. shadow outer bindings of the same name for the duration of the application
6. evaluate the executable value in that scope

Example:

```text
name = "outer"

[name:"inner"] . {
    show name
}
```

During the application, `name` resolves to `"inner"`.

### Runtime application

```text
[url:"https://example.com"] @ fetch
```

Runtime application follows the same binding pattern but may additionally establish a runtime facing invocation context. A higher level runtime may also allow `@` to participate in controlled outer scope modification or other contextual effects.

## 8. Structural Evaluation

Operators transform values directly.

```text
[1 2] ^ [3]
→ [1 2 3]

[a:1] ^ [b:2]
→ [a:1 b:2]
```

Structural operators produce new values.

## 9. Lazy vs Immediate Evaluation

Rollo is primarily eagerly evaluated.

Expressions are evaluated immediately except for:

- code blocks
- values passed to commands that delay execution
- executable values awaiting application

## 10. Value Immutability

Values are conceptually immutable. Operations produce new values rather than mutating existing ones.

```text
a = [1 2]
b = a ^ [3]
```

This preserves:

```text
a = [1 2]
b = [1 2 3]
```

## 11. Program Result

A Rollo program returns the value of its final command or expression.

```text
x = 1
y = 2
x + y
```

Result:

```text
3
```

## 12. Determinism

Rollo programs are deterministic except where runtime application or impure commands are used.

Purely structural programs given the same input values always produce the same result.

This supports:

- caching
- replay
- distributed evaluation
- deterministic builds

## 13. Relationship to the Runtime

The evaluation model is independent of runtime environment. The runtime provides command implementations and system services, while the kernel evaluation model remains consistent across environments.
