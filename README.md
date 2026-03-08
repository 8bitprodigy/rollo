# Rollo

Rollo is a document oriented language for networked interactive systems.

This repository contains the current draft specification set for the language, including:

- kernel semantics
- formal grammar
- parser and AST notes
- container behavior and promotion
- evaluation model
- type model
- runtime architecture
- event and interaction model

Rollo treats values, containers, application, and execution as part of one small kernel, while leaving higher level services to runtimes and libraries.

## Repository Status

This repository is a draft language design repository. It is intended for specification work, discussion, and future implementation planning.

The current draft now makes these points explicit:

- `[]` denotes all container literals
- `()` denotes immediate evaluation of an expression
- `{}` denotes a code block with deferred execution
- command arguments are collected as an implicit heterogeneous list during parsing and evaluation
- command arguments are greedily parsed as full expressions from left to right
- bracket containers resolve to array, list, or map by content
- mixing keyed and unkeyed bracket entries is invalid
- application defines an explicit binding collision rule
- applied bindings populate a fresh application scope and shadow outer bindings during that application

## Repository Layout

```text
.
├── .github/
├── docs/
├── examples/
├── spec/
├── .gitattributes
├── .gitignore
├── CHANGELOG.md
├── CONTRIBUTING.md
├── LICENSE
├── README.md
```

## Quick Start

Start with these files:

1. `spec/rollo-kernel.md`
2. `spec/rollo-grammar.md`
3. `spec/rollo-parser-ast.md`
4. `spec/rollo-evaluation.md`

Then review:

- `spec/rollo-containers.md`
- `spec/rollo-types.md`
- `spec/rollo-runtime.md`
- `spec/rollo-events.md`

Examples are in `examples/`.

## GitHub Ready Notes

This repository now includes:

- a root `LICENSE`
- a `CONTRIBUTING.md`
- a `CHANGELOG.md`
- a `.gitignore`
- a `.gitattributes`
- documentation index files
- example index files
- GitHub issue templates
- a pull request template

The included license is **BSD 0 Clause (0BSD)** as a reasonable default for a language specification repository. Change it if you want a different policy.
