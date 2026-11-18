# Translation Plan: C Model to Hand-Written RISC-V

`reference_model.c` is a design specification. Do not mechanically convert it or submit generated assembly. Each student must write, trace, and explain their own routines.

## Build in this order

1. Initialize the model frame.
2. Copy and display one normal fixed input.
3. Add the fixed vulnerable-model comparison and harmless `win()` status.
4. Add the required bounds check and retest the identical input.
5. Add the canary only after the required path is stable.

## Model layout

| Slot | Meaning | Requirement |
| ---: | --- | --- |
| 0–7 | Logical eight-word buffer | Required |
| 8 | Canary marker | Stretch |
| 9 | Modelled saved return target | Required for comparison |

Each word offset is `slot × 4` bytes. Decide the base address/frame layout yourselves and document it in the report.

## Routine checklist

| Model routine | Your assembly responsibility | Evidence |
| --- | --- | --- |
| `init_frame` | Clear slots and write known markers. | Initial memory observation. |
| `copy_vulnerable_model` | Controlled copy across *logical* boundary for fixed class model. | Changed marker + harmless status. |
| `copy_with_bound` | Compare requested count with 8 before any copy. | Rejection status; no `win()`. |
| `copy_with_canary` | Stretch: check marker before accepting return. | Canary-detected status. |
| Status/display | Map outcome to simple framebuffer/MMIO state. | Screenshot per outcome. |

## Register plan template

| Role | Your chosen register | Preservation note |
| --- | --- | --- |
| Frame base / `sp` | TBD | Keep valid through routine. |
| Input pointer | TBD | Advances during copy. |
| Count | TBD | Compare to capacity. |
| Loop index | TBD | Computes word offset. |
| Outcome | TBD | Passed to display routine. |

## Frequent mistakes

- Forgetting that a word is four bytes.
- Using a temporary across a function call without preserving it.
- Not restoring saved registers or stack space before return.
- Comparing different inputs on vulnerable and protected paths.
- Starting the canary before the bounds check works.

The required version is finished when normal and boundary tests return normally, the vulnerable model reaches only the harmless `win()` outcome for the fixed case, and the bounds-checked version rejects that same case.
