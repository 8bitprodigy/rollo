# Changelog

## v1.6 ordered maps, mixed containers, applicator refinement, and command naming

- clarified that maps are ordered as well as key addressable
- allowed mixed keyed and unkeyed bracket entries with index based access semantics
- added `pow` as an infix arithmetic operator candidate in the draft
- clarified the intended split between `.` and `@` for outer scope effects
- documented higher level command naming guidance for `show`, `text`, and `log`
- added `examples/text-and-log.rollo`
- added `examples/pow.rollo`

## v1.5 styling and symbol update

- extended styling docs with class-like selector metadata
- added discussion of `symbol` as a candidate type
- updated type model for symbol discussion
- added `examples/style-class-like.rollo`

## v1.4 complete project refresh

- rebuilt the complete GitHub ready repository package
- preserved the 0BSD license
- included the full specification set
- added `spec/rollo-styling.md`
- added `docs/styling-notes.md`
- added styling examples

## v1.0 spec correction milestone

- corrected the delimiter model
- all containers now use `[]`
- `()` now denotes immediate evaluation
- `{}` remains deferred code blocks
- updated kernel, grammar, parser, evaluation, containers, runtime, and examples
