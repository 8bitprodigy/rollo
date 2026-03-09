# Rollo Containers and Promotion
Version 1.1 (Draft)

Rollo defines four container types:

- string
- array
- list
- map

These are semantic types. Implementations may represent them using different underlying data structures.

## Container Roles

### String

An immutable ordered sequence of characters.

### Array

An ordered homogeneous sequence.

### List

An ordered heterogeneous sequence.

### Map

A key value container.

Maps are ordered as well as key addressable.

## Delimiter Rule

All container literals use `[]`.

Container kind is resolved by content:

- keyed entries produce an ordered map
- unkeyed homogeneous entries produce an array
- unkeyed heterogeneous entries produce a list
- mixed keyed and unkeyed entries remain ordered and are always addressable by index; keyed entries additionally expose their keys

## Implementation Note

The kernel does not mandate storage layout. An implementation may use:

- native strings for strings
- compact dynamic arrays for arrays
- heterogeneous arrays or tables for lists
- associative structures for maps

Strings are treated as a distinct type even though they are sequence-like, because they are common, performance sensitive, and semantically special.

## Promotion Hierarchy

When structurally combining unlike containers, promotion follows this hierarchy.

```text
string < array < list < map
```

The higher type dominates.

## Promotion Examples

```text
"abc" ^ "def"        -> string
"abc" ^ ["d"]        -> array
[1 2] ^ [3 "x"]      -> list
[1 "x"] ^ [a:1]      -> map
```

## Structural Merge Rules

### String

```text
"hello " ^ "world"
→ "hello world"
```

### Array

```text
[1 2] ^ [3 4]
→ [1 2 3 4]
```

### List

```text
[1 "a"] ^ [2 "b"]
→ [1 "a" 2 "b"]
```

### Map

```text
[a:1 b:2] ^ [b:3 c:4]
→ [a:1 b:3 c:4]
```

## Mixed Container Note

Allowing mixed keyed and unkeyed entries makes bracket containers more flexible for structured authoring. This is especially relevant for document and styling use cases where order matters but occasional keyed access is also useful.

## Scalar Promotion

Scalars do not merge structurally on their own. If needed, runtimes may promote scalars into lists during structural composition.

Example:

```text
1 ^ 2
→ [1 2]
```

That behavior should be consistent within an implementation.

## Application Relevance

The separation of container types is also important for application. Maps are the primary carrier for named bindings, while other container forms may support runtime defined application patterns.
