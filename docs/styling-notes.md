# Styling Notes

These notes capture a higher level direction for styling in Rollo.

## Main idea

Styling can be expressed through the same mechanisms already present in the language:

- rebinding
- application
- override maps
- structural merge
- late transformation passes

This avoids introducing a separate styling language too early.

## Rebinding and defaults

If replacement arguments can be rebound onto existing executable values, then a base definition can be specialized into derived variants with new defaults.

That means a base element can behave much like an element selector with default style values.

## Nested styling

If selected child scopes are able to modify values in parent scopes, then nested structures can accumulate or refine style defaults in context.

This is useful when a nested element contributes style information to an enclosing styling environment.

## Late style application

A style may need to be applied after relevant structures have already been defined. That suggests a callable style pass that walks already constructed values and applies defaults or overrides afterward.

## Recommendation

These behaviors should be treated as higher level language or runtime design, built on kernel primitives such as:

- application
- rebinding
- lexical scope
- controlled parent scope modification
- structural merge


## Class-Like Styling

A class-like mechanism can be modeled as metadata plus a later transformation pass.

For example, a structure may mark values with `kind`, `role`, or `tags`, and a later style pass may apply a style map to every matching element in a larger structure.

This gives Rollo something analogous to CSS classes without requiring a separate CSS layer.

## Symbol as a Candidate Type

A symbol type would strengthen this model by letting selectors, tags, roles, and kinds exist as stable identities distinct from strings.

That would make metadata such as `card`, `warning`, or `button` feel more like language values and less like plain text.


## Show, Text, and Log

A useful higher level naming convention is:

- `show` for presenting a card to the viewer
- `text` for drawing or emitting text into the current presentation context
- `log` for console or diagnostic output

This is not a kernel rule, but it is a clearer higher level distinction than using `show` for all visible text output.
