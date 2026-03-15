# Rollo Runtime Architecture
Version 1.2 (Draft)

The Rollo runtime provides the environment in which Spindle evaluation occurs. It implements commands, services, and system integration on top of the Spindle kernel.

## 1. Runtime Layers

```text
Rollo Runtime
 ├ Parser
 ├ Evaluator
 ├ Collection System
 ├ Application Engine
 ├ Access Engine
 ├ Command Registry
 └ External Services
```

## 2. Parser

The parser reads source text and produces an AST suitable for evaluation, following the rules defined in the Spindle parser and AST specification.

## 3. Evaluator

The evaluator applies Spindle kernel semantics — resolves symbols, invokes commands, applies structured bindings, evaluates access expressions, and produces values.

## 4. Collection System

The collection system implements strings, arrays, lists, pairs, maps, and tables according to Spindle kernel behavior, while allowing optimized internal storage.

Implementations may use:

- native strings for strings
- compact dynamic arrays for arrays and lists
- a single key-value struct for pairs
- ordered associative structures for maps
- ordered associative structures with index support for tables

## 5. Callable Construction Engine

The callable construction engine handles `.` and `;` construction semantics. Neither operator executes — both produce callables.

For isolated callable construction (`.`):

- evaluate the left operand to a map
- create a fresh isolated scope
- bind map keys into that scope
- return the callable with the isolated scope — do not execute

For captured callable construction (`;`):

- evaluate the left operand to a map
- evaluate the right operand to a block or callable
- merge the map into the block's external argument map
- capture the current scope
- return the resulting callable — do not execute

## 6. Access Engine

The access engine handles `@` access semantics.

- evaluate the left operand to a collection
- evaluate the right operand to a key or index value
- look up the entry by symbol, string, integer index, or range
- return the value or `nil` if not found

## 7. Command Registry

Commands may be supplied by:

- Rollo standard library modules
- bootstrapped operations
- FFI bindings
- external runtime services

The command registry maps symbol identifiers to their implementations. At evaluation time, a command name is looked up in the registry and invoked with its collected argument list.

## 8. External Services

The runtime may expose networking, storage, timers, system calls, and other services through commands or runtime application via `;`.

## 9. Determinism Boundary

Spindle kernel semantics remain deterministic except where runtime services are invoked through `;` or through explicitly impure Rollo commands.

Purely structural programs — those using only `^`, `.`, `@`, and collection construction — are always deterministic.

## 10. Capability Style Design

A Rollo runtime may restrict command availability by capability. This is a good fit for deterministic, sandboxed, or distributed execution environments where only a controlled subset of commands should be available.

## 11. Relationship to Spindle

Spindle specifies semantics. Rollo and its runtimes provide implementation, commands, and system integration.

The Spindle kernel evaluation model remains consistent across all Rollo runtime environments. What varies between environments is the command set and the external services available.
