# Beginner Guide

This guide assumes no prior C, RISC-V, or security background. It explains the class project model, not a method for affecting real software.

## The machine in three ideas

- A **register** is small storage inside the CPU. RISC-V names registers `x0` through `x31`.
- **Memory** is a large collection of addressed storage locations.
- A **load** reads memory into a register; a **store** writes a register value to memory.

The Playground also shows a **framebuffer**: memory whose values become pixels. A simple color or label can communicate the current test outcome.

## Functions, return addresses, and the stack

A **function** completes one task. A caller transfers control to it and expects execution to resume afterward. The expected next location is the **return address**; RISC-V conventionally keeps it in `ra`.

The **stack** stores temporary function data. Each active function has a **stack frame**, conceptually like this:

```text
logical buffer[0] ... buffer[7]
optional canary marker
modelled saved return target
```

`sp` is the stack pointer. Your report must state the exact layout and offsets used by your own assembly, and every routine must follow that agreement.

## The controlled buffer model

The project’s logical buffer has eight words: valid indexes are 0–7. A write at index 8 crosses the intended boundary.

Real out-of-bounds behavior is dangerous and unpredictable. [`reference_model.c`](../reference_model.c) avoids that: it represents the frame as labelled slots, making the demonstration deterministic. The vulnerable course model permits one fixed oversized input to cross the logical boundary and select a harmless `win()` outcome. It is confined to the model/emulator.

## Required defense: bounds checking

Check the requested count *before* writing:

```text
if count > capacity:
    reject input
else:
    copy count values
```

Here, capacity is eight words. The required protected path must reject the exact oversized input used by the vulnerable model.

## Stretch defense: stack canary

A **canary** is a known marker between a buffer and important stored state. Check it before returning. If it changed, report detection instead of accepting the result. Canaries help detect overwrites but do not replace bounds checks.

## Evidence you need

For each case, capture a visible status plus the relevant Playground memory/register observation.

| Case | Expected result |
| --- | --- |
| Normal, up to 8 words | Normal return |
| Boundary, exactly 8 words | Normal return |
| Fixed oversized model input | Benign `win()` result in vulnerable model |
| Same input, bounds-checked path | Input rejected |
| Same input, canary stretch | Canary detected |

## Before writing assembly

- Draw the exact frame layout and byte offsets.
- Decide register ownership and save/restore rules.
- Write the normal path first.
- Use the same fixed cases in every comparison.
- Add stretch work only after required tests pass.

Continue with the [translation plan](TRANSLATION_PLAN.md).
