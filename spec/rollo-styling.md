# Rollo Styling and Override Model
Version 0.3 (Draft)

This document describes a higher level direction for styling, defaults, and override behavior in Rollo. It is not part of the minimal kernel, but it follows naturally from kernel primitives such as application, rebinding, scope, and structural merge.

## 1. Main Idea

Styling may be expressed through ordinary language semantics rather than through a separate styling language.

Useful primitives include:

- rebinding
- structural application
- runtime application
- override maps
- structural merge
- controlled parent scope modification
- late style application passes

## 2. Rebinding and Defaults

If replacement arguments can be rebound onto an existing block or callable, then a base definition can be specialized into a derived one with new defaults.

Conceptually:

```text
baseCard = some-callable
styledCard = [padding: 2 color: "blue"] . baseCard
```

This is analogous to default styling for an element family.

## 3. Override Behavior

A later application may override previously established defaults by rebinding the same names.

Conceptually:

```text
base = [padding: 1 color: "gray"] . card
warning = [color: "red"] . base
```

In that model, `warning` inherits earlier defaults and replaces only what changes.

## 4. Scope and Parent Modification

Chris's observation suggests that higher level styling may require selected child scopes to modify values in an enclosing parent scope.

That would allow nested style declarations to accumulate or refine defaults that affect later structures in the same viewer facing file.

This should not be treated as an unrestricted kernel behavior. It is better modeled as a controlled higher level mechanism.

## 5. Late Style Application

Some styles may need to be applied after all relevant structures have already been defined.

That suggests a style function or style pass that traverses previously constructed values and applies defaults or overrides afterward.

Conceptually:

```text
viewer = define-cards
viewer = apply-styles viewer
```

## 6. Relationship to Structural Merge

Because style data is naturally map shaped, style application can be expressed as structural merge in many cases.

Conceptually:

```text
style = [padding: 2 color: "blue"]
attrs = attrs ^ style
```

## 7. Why this matters

This direction allows Rollo to express something like CSS default behavior through the ordinary semantics of:

- named rebinding
- override application
- structural transformation

rather than through a fully separate styling system.

## 8. Open Design Questions

The following questions remain open:

- whether derived styled callables should be first class values
- how controlled parent scope modification should work
- whether style application should be purely structural or may use runtime passes
- whether late style application should be standardized or remain a library convention


## 9. Selector-Like Metadata and Class Equivalents

A CSS-like "class" in Rollo should emerge from structured metadata rather than from a separate styling language primitive.

A value may carry metadata such as:

```text
[kind: card role: warning]
```

or

```text
[tags: [card warning]]
```

A later style rule or style pass can then match these fields across an entire structure and apply style defaults or overrides to every matching element.

Conceptually:

```text
styleRule = [
    match: [kind: card]
    apply: [padding: 2 color: "blue"]
]
```

and later:

```text
viewer = applyStyleRule viewer styleRule
```

This makes class-like behavior a consequence of:

- structured metadata
- selector matching
- transformation passes
- structural merge

rather than a separate styling sublanguage.

## 10. Why Symbol Helps Styling

A symbol type is a strong fit for tags, roles, kinds, and selector values.

For example:

```text
[kind: card role: warning]
```

is semantically cleaner if `card` and `warning` are symbols rather than strings.

That distinction helps because:

- strings represent text content
- symbols represent names or identities
- selectors often want stable identity rather than user-facing text

## 11. Open Questions

Additional open questions include:

- whether selector metadata should prefer single symbols or symbol lists
- whether style rules should be standardized as maps, callables, or both
- how structure-wide style passes should traverse nested values


## 12. Relationship to `.` and `@`

The distinction between `.` and `@` is especially important for styling.

- `.` is a good fit for isolated rebinding of defaults into a fresh application scope
- `@` is a better fit for contextual styling effects, including controlled updates to enclosing style environments or late style application passes

That split aligns well with a structured rendering architecture in which local structure and contextual viewer state should remain distinguishable.
