# Rollo Event and Interaction Model
Version 1.0 (Draft)

The Rollo event and interaction model defines how structured runtime values participate in user interaction and system driven message flow.

Rollo should treat interaction as structured message handling rather than as arbitrary mutation of a global hidden tree.

## 1. Core Principles

- events are values
- handlers are executable values
- interaction should preserve deterministic structure where possible
- side effects occur through explicit runtime application or commands

## 2. Event Values

An event is a structured value, typically represented as a map.

Example:

```text
[type:"click" target:"submit" x:10 y:20]
```

Common fields may include:

- type
- target
- source
- timestamp
- payload

## 3. Handler Evaluation

A handler is typically a block or callable that receives an event value and returns a value.

Example:

```text
handleSubmit = {
    log "submitted"
}
```

A natural model is that the runtime applies the event map to the handler.

Conceptually:

```text
event @ handleSubmit
```

Handlers should be invoked by the runtime when the corresponding event occurs.

## 4. Message Propagation

A runtime may support propagation through structured hierarchies. If so, propagation should be modeled explicitly and consistently.

Possible phases:

- capture
- target
- bubble

These phases are runtime policy, not kernel policy.

## 5. State and Update

Rollo should prefer explicit state values over hidden ambient mutation.

A handler may return:

- a replacement value
- a patch value
- a command request
- `nil`

A runtime may interpret returned values to update state or other structured results.

## 6. Structural Updates

Updates should ideally be expressed as structural replacement or structural merge using `^`, rather than direct mutation of an implicit global structure.

Example:

```text
state = state ^ [count: state.count + 1]
```

## 7. Runtime Boundary

Event delivery itself is a runtime action. Handler internals may still perform structural transformation until they explicitly cross into runtime application or commands.

## 8. Plan 9 Style Direction

The interaction model should remain close to message passing and structured resources rather than adopting a hidden mutation heavy model.

## 9. Open Questions

The following areas remain intentionally open for future refinement:

- exact handler binding syntax
- propagation defaults
- state ownership model
- async and timer semantics
