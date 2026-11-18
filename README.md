# Stack Smash → Defenses

A two-person CPE 2300 project that demonstrates a memory-safety concept in an **isolated RISC-V course emulator**: an unchecked logical buffer write can alter stored control-flow state; a bounds check and optional stack canary prevent or detect it.

This repository is a classroom-only model. It is not a guide to testing, attacking, or modifying real programs or systems.

## Schedule and scope

| Item | Plan |
| --- | --- |
| Start | Monday, November 17 |
| Checkpoints | Wednesday, November 18 and Wednesday, December 2 |
| Final submission | Friday, December 4, 11:59 PM |
| Required defense | Bounds check |
| Stretch feature | Stack canary |
| Optional comparison | W^X discussion/model, time permitting |

## Learning objectives

1. Explain stack frames, functions, calls, and returns.
2. Make frame/control-flow state observable through Playground memory/register panels and a minimal framebuffer/MMIO status display.
3. Reject an oversized fixed test input before it crosses an eight-word logical buffer boundary.

## Repository map

| File | Purpose |
| --- | --- |
| [`CPE2300-Stack-Smash-Workbook.html`](CPE2300-Stack-Smash-Workbook.html) | Filled project workbook: objectives, owners, checkpoints, and test cases. Open in a browser. |
| [`reference_model.c`](reference_model.c) | Deterministic C design model for hand translation. It is not a host-machine exploit. |
| [`stack-smash-control-flow.svg`](stack-smash-control-flow.svg) | Report/demo control-flow diagram. |
| [`docs/BEGINNER_GUIDE.md`](docs/BEGINNER_GUIDE.md) | Plain-language explanation of all project concepts. |
| [`docs/TRANSLATION_PLAN.md`](docs/TRANSLATION_PLAN.md) | Routine-by-routine plan for hand-written RISC-V. |
| [`docs/DEMO_AND_REPORT.md`](docs/DEMO_AND_REPORT.md) | Testing, evidence, demo, report, and viva checklist. |

## Expected demonstration

Run fixed, known class test cases only:

1. A normal input of at most eight words returns normally.
2. In the vulnerable **model**, a fixed oversized class input produces a harmless `win()` outcome.
3. The same input on the required bounds-checked path is rejected.
4. Stretch: the canary path reports detection before accepting a return.

The display should clearly distinguish `Normal return`, `Benign win()`, `Input rejected`, and, if complete, `Canary detected`.

## Getting started

1. Add both team members as repository collaborators.
2. Open the workbook and replace every `TBD` with team names, ownership, and this repository URL.
3. Read the [beginner guide](docs/BEGINNER_GUIDE.md) together.
4. Make a frame-offset diagram and register-allocation table, then write the RISC-V assembly yourselves.
5. Use the [demo checklist](docs/DEMO_AND_REPORT.md) to record evidence as you work.

## Team workflow

- **Member 1:** normal/vulnerable model, stack-frame explanation, visible status, and evidence capture.
- **Member 2:** bounds check, optional canary, test matrix, integration, report, and demo rehearsal.

Ownership is not isolation. Both students must understand the full project and be able to explain and modify their own code.

## Academic integrity

Planning, documentation, test design, and a C reference model may use AI as permitted by the course. **All submitted RISC-V assembly must be handwritten, understood, and modifiable by the team.** Do not submit generated assembly; disclose allowed AI use honestly.

## Current status

Planning documentation is complete. Next: replace workbook `TBD` values, agree on the frame layout, and build the smallest normal-return slice by hand in the Playground.
