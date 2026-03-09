# Rollo Type Model
Version 1.2 (Draft)

Rollo is dynamically typed. Every value carries its own type, and operations determine behavior based on the types of their operands.

The kernel defines a small set of fundamental value classes. These classes determine how values participate in operations such as structural merge, container promotion, application, and command invocation.

## 1. Value Classes

All Rollo values belong to one of four classes.

```text
scalar
sequence
associative
executable
```

These classes describe behavior rather than memory layout.

## 2. Scalar Values

Scalars represent atomic data.

Types:

- number
- boolean
- nil
- symbol

### Symbol

A symbol is a stable name-like value distinct from a string.

Symbols are useful for:

- tags
- roles
- selectors
- map keys used as data
- identifiers passed as values

A string represents text. A symbol represents identity or name.

Conceptually:

```text
"card"   -> string
card     -> identifier in source, and potentially a symbol when treated as data
```

## 3. Sequence Values

Sequences represent ordered collections.

Types:

- string
- array
- list

Properties:

- ordered
- indexable
- iterable

### String

Immutable sequence of characters.

### Array

Homogeneous sequence of values.

### List

Heterogeneous sequence of values.

A mixed keyed and unkeyed bracket container may be treated as a higher level ordered container representation until a more final normalization rule is chosen by the language.

## 4. Associative Values

Associative containers store key value pairs.

Type:

- map

Maps are the primary kernel carrier for named application bindings.

## 5. Executable Values

Executable values represent deferred computation.

Types:

- code block
- callable

Executable values are the right hand side targets of application.

## 6. Value Hierarchy

```text
value
 ├ scalar
 ├ sequence
 │   ├ string
 │   ├ array
 │   └ list
 ├ associative
 │   └ map
 └ executable
     ├ block
     └ callable
```

## 7. Structural Merge Behavior

The structural merge operator `^` behaves according to value class.

### Scalar

Scalars do not merge structurally by default. Implementations may promote scalars to lists.

```text
1 ^ 2
→ [1 2]
```

### Sequence

Sequences concatenate or structurally combine according to their concrete type.

### Associative

Maps merge keys.

## 8. Application Behavior

Application combines associative values with executable values.

Canonical kernel form:

```text
map . block
map @ callable
```

The left operand supplies bindings. The right operand consumes them in an application scope.

Applied bindings shadow outer bindings with the same names during the application, but they do not mutate outer scopes.

## 9. Container Promotion

When combining different container types, promotion follows this hierarchy.

```text
string < array < list < map
```

## 10. Type Introspection

Commands may inspect value types.

Example:

```text
type x
```

Possible results include:

- number
- string
- symbol
- array
- list
- map
- block
- callable

Type introspection is provided by runtime commands rather than kernel operators.

## 11. Type Flexibility

Variables may be rebound to values of different types.

```text
x = 10
x = "hello"
```

## 12. Type Stability of Containers

Although variables are dynamically typed, container values themselves remain stable once created. This supports efficient implementations and predictable merge behavior.
