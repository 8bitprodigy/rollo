# Rollo Type Model
Version 1.3 (Draft)

Rollo is dynamically typed. Every value carries its own type, and operations determine behavior based on the types of their operands.

The kernel defines a small set of fundamental value classes. These classes determine how values participate in operations such as structural merge, container promotion, application, and command invocation.

## 1. Value Classes

All Rollo values belong to one of three classes.

```text
scalar
collection
executable
```

These classes describe behavior rather than memory layout.

## 2. Scalar Values

Scalars represent atomic, indivisible data.

Types:

- nil
- boolean
- number
  - integer
  - decimal
  - fraction
- character
- symbol

### Number

Numbers represent numeric values. There are three numeric subtypes.

An integer is a whole number. A decimal is a floating point number. A fraction is a rational number represented as a numerator and denominator.

```text
42        -> integer
3.14      -> decimal
1/3       -> fraction
```

### Character

A character is a single Unicode code point, written with single quotes.

```text
'a'
'€'
```

Characters are scalar values. A sequence of characters forms a string.

### Symbol

A symbol is a stable name-like value distinct from a string.

Symbols are useful for:

- tags
- roles
- selectors
- map keys used as data
- identifiers passed as values

A string represents text. A symbol represents identity or name.

```text
"card"   -> string
`card    -> symbol literal
$name    -> symbol interned from variable or string
```

## 3. Collection Values

Collections represent structured groupings of values. All collection literals use `[]`.

Types:

- string
- array
- list
- pair
- map
- table

Properties shared by all collections:

- ordered
- indexable
- iterable

### String

Immutable ordered sequence of characters.

```text
"hello"
```

Strings are a distinct collection type because they are extremely common and performance sensitive.

### Array

Ordered homogeneous sequence of values.

```text
[1 2 3]
```

### List

Ordered heterogeneous sequence of values.

```text
[1 "hello" true]
```

### Pair

A single key-value entry. Produced by the `:` operator.

```text
name:"Ada"
```

A pair is its own type — not a sequence and not a map. It is atomic in the sense that it carries exactly one key and one value.

### Map

An ordered associative container of key-value pairs.

```text
[name:"Ada" age:32]
```

Maps are ordered as well as key addressable. They are the primary carrier for named application bindings.

### Table

An ordered container supporting both keyed and unkeyed entries. Produced when a sequence and a map are structurally merged.

```text
[a:1 2 b:3]
```

All entries in a table are addressable by index. Keyed entries are additionally addressable by key.

## 4. Executable Values

Executable values represent deferred computation.

Type:

- block

A block is a deferred sequence of commands enclosed in `{}`. It introduces a lexical scope and executes only when applied or invoked.

A block with a bound external argument map is referred to as a callable, but the underlying type remains block.

```text
{
    show "hello"
}
```

Executable values are the right hand side targets of application.

## 5. Value Hierarchy

```text
value
 ├ scalar
 │   ├ nil
 │   ├ boolean
 │   ├ number
 │   │   ├ integer
 │   │   ├ decimal
 │   │   └ fraction
 │   ├ character
 │   └ symbol
 ├ collection
 │   ├ string
 │   ├ array
 │   ├ list
 │   ├ pair
 │   ├ map
 │   └ table
 └ executable
     └ block
```

## 6. Collection Promotion Hierarchy

When combining different collection types, promotion follows this hierarchy.

```text
string < array < list < pair < map < table
```

The higher type dominates.

## 7. Structural Merge Behavior

The structural merge operator `^` behaves according to value class and type.

### Scalar

Scalars promoted into collections when merged.

```text
1 ^ 2
→ [1 2]
```

### Character

Characters merge into strings when combined with other characters or strings.

```text
'a' ^ 'b'    -> "ab"
'a' ^ "bc"   -> "abc"
```

When merged with other collection types, a character is treated as an element.

```text
'a' ^ [1 2]  -> [1 2 'a'] or ['a' 1 2]
```

### String

Strings concatenate with other strings. When merged with arrays or lists, a string explodes into its constituent characters.

```text
"hello " ^ "world"   -> "hello world"
"ab" ^ [1 2]         -> ['a' 'b' 1 2]
```

### Array

Arrays concatenate when homogeneous. Mixed content promotes to list.

```text
[1 2] ^ [3 4]        -> [1 2 3 4]
[1 2] ^ [3 "x"]      -> [1 2 3 "x"]
```

### List

Lists concatenate.

```text
[1 "a"] ^ [2 "b"]    -> [1 "a" 2 "b"]
```

### Pair

A pair merged with a sequence produces a table. A pair merged with another pair produces a map, unless the keys are identical, which is an error.

```text
name:"Ada" ^ age:32          -> [name:"Ada" age:32]
name:"Ada" ^ name:"Lovelace" -> name:"Lovelace"
```

### Map

Maps merge keys. A later key overwrites an earlier one.

```text
[a:1 b:2] ^ [b:3 c:4] -> [a:1 b:3 c:4]

[a:1 b:2] ^ [1 2]     -> table
[a:1 b:2] ^ "hello"   -> table
```
A map merged with any sequence type produces a table.

### Table

Any merge involving a table produces a table.

## 8. Application Behavior

Application combines collection values with executable values.

Canonical kernel forms:

```text
map . block
map @ block
```

The left operand supplies bindings. The right operand consumes them.

### Structural application

`.` creates an isolated scope populated only by the applied map. The block cannot read or write the outer scope.

```text
x = 10

[x: 20] . {
    show x      -> 20
}

show x          -> 10
```

To pass an outer variable into an isolated scope:

```text
x = 10

[x: x] . {
    show x      -> 10
}
```

### Runtime application

`@` exposes the calling scope to the block. The block shares the parent scope and mutations propagate back up.

```text
x = 10

[] @ {
    x = 20
}

show x          -> 20
```

### Default block execution

A block executed without application shares the parent scope directly.

```text
x = 10

{
    x = 20
}

show x          -> 20
```

## 9. Type Introspection

Commands may inspect value types.

```text
type x
```

Possible results include:

- nil
- boolean
- integer
- decimal
- fraction
- character
- symbol
- string
- array
- list
- pair
- map
- table
- block

Type introspection is provided by runtime commands rather than kernel operators.

## 10. Type Flexibility

Variables may be rebound to values of different types.

```text
x = 10
x = "hello"
```

## 11. Type Stability of Collections

Although variables are dynamically typed, collection values themselves remain stable once created. This supports efficient implementations and predictable merge behavior.
