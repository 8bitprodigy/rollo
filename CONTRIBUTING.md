# Contributing

Thank you for contributing to the Rollo specification work.

## Scope

This repository is primarily for:

- specification corrections
- grammar improvements
- semantic clarifications
- parser and AST refinement
- runtime model discussion
- worked examples

## Contribution Style

Please prefer:

- small focused pull requests
- explicit rationale for semantic changes
- examples alongside rule changes
- consistency across kernel, grammar, parser, and evaluation documents

## When Changing Syntax

If you change syntax or delimiter rules, update all relevant files:

- `spec/rollo-kernel.md`
- `spec/rollo-grammar.md`
- `spec/rollo-parser-ast.md`
- `spec/rollo-evaluation.md`
- `spec/rollo-containers.md` if container behavior is affected
- examples in `examples/`

## When Changing Semantics

Please include:

1. the old behavior
2. the new behavior
3. one or more examples
4. any parser or AST consequences
5. any runtime consequences

## Pull Request Checklist

- the affected documents are internally consistent
- examples match the current syntax
- README references are still accurate
- no undefined commands or structures were introduced into the kernel spec
- delimiter and container rules remain consistent across documents

## Licensing

By contributing, you agree that your contribution may be distributed under the repository license.
