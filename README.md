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
- styling and override model

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
- mixed keyed and unkeyed bracket entries are allowed as ordered containers with index access semantics
- application defines an explicit binding collision rule
- applied bindings populate a fresh application scope and shadow outer bindings during that application
- higher level styling may be modeled through rebinding, overrides, selector-like metadata, and late style application passes
- symbol is now discussed as a candidate type for tags, roles, selectors, and keys used as data
- maps are ordered as well as key addressable
- `pow` is discussed as an infix arithmetic operator candidate
- higher level command naming may distinguish `show`, `text`, and `log`
- `@` is discussed as the applicator that may support controlled outer scope effects

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
└── REPO_SETUP.md
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
- `spec/rollo-styling.md`

Examples are in `examples/`.



# License:

Public domain, but since there are jurisdictions in which the public domain is not legally recognized, it is also made available under the terms of the **BSD 0-Clause (0BSD)** license.
See `LICENSE` for terms.
