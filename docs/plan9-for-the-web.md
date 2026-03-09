# Rollo as Plan 9 for the Web

Rollo is best understood not merely as a scripting language, but as a unified document and application language with a Plan 9 style philosophy.

## The problem with the modern web

The modern web splits application construction across multiple overlapping systems:

- HTML for structure
- CSS for presentation
- JavaScript for behavior
- DOM mutation for interaction

This layering creates conceptual and implementation complexity.

## The Rollo approach

Rollo uses one language to describe structured values, logic, and interaction.

Programs produce values. Some of those values may later be interpreted by renderers or other runtime systems according to their environment.

## Why this resembles Plan 9

Plan 9 separated mechanism from policy and unified interfaces around simple, composable abstractions. Rollo aims for the same kind of simplification in networked application construction.

In Rollo:

- the kernel is small
- commands are extensible
- runtime services are external
- interaction is structured message handling
- maps can act as named binding carriers

## Structural application

A major part of the current design is structural application. Instead of treating application as positional argument passing, Rollo can bind structured values into executable scopes. This makes attributes, event payloads, and runtime requests fit naturally into the language.

## Why this matters

A single language that unifies structured values, application logic, and interaction can reduce accidental complexity and make the platform easier to reason about, implement, and evolve.
