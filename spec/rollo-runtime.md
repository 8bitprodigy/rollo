# Rollo Runtime Architecture
Version 1.1 (Draft)

The Rollo runtime provides the environment in which kernel evaluation occurs. It implements commands, services, and system integration.

## 1. Runtime Layers

```text
Rollo Runtime
 ├ Parser
 ├ Evaluator
 ├ Container System
 ├ Application Engine
 ├ Command Registry
 └ External Services
```

## 2. Parser

The parser reads source text and produces an internal representation suitable for evaluation.

## 3. Evaluator

The evaluator applies kernel semantics, resolves variables, invokes commands, applies structured bindings, and produces values.

## 4. Container System

The container system implements strings, arrays, lists, and maps according to kernel behavior, while allowing optimized internal storage.

## 5. Application Engine

The application engine handles applicator semantics.

It should:

- evaluate the left operand
- establish an application scope
- bind map keys into lexical names
- shadow outer bindings of the same name inside that scope
- invoke the right operand as a block or callable
- optionally establish runtime context for `@`
- optionally allow controlled outer scope modification for contextual application semantics

## 6. Command Registry

Commands may be supplied by:

- standard library modules
- bootstrapped operations
- FFI bindings
- external runtime services

## 7. External Services

The runtime may expose networking, storage, timers, system calls, and other services through commands or runtime application.

## 8. Determinism Boundary

Kernel semantics remain deterministic except where runtime services are invoked through `@` or through explicitly impure commands.

## 9. Capability Style Design

A runtime may restrict command availability by capability. This is a good fit for deterministic, sandboxed, or distributed execution environments.

## 10. Relationship to the Kernel

The kernel specifies semantics. The runtime provides implementation and integration.
