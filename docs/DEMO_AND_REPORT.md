# Demo, Evidence, and Report Checklist

Record only completed work and reproducible evidence.

## Test matrix

| Scenario | Expected result | Save |
| --- | --- | --- |
| Normal input | Normal return | Status + memory screenshot |
| Exactly 8 words | Normal return | Status + final buffer observation |
| Fixed oversized vulnerable model | Harmless `win()` outcome | Before/after return-target model state + status |
| Same input, bounds check | Input rejected | Rejection status; no `win()` |
| Same input, canary stretch | Canary detected | Canary observation + status |

Record input, expected result, actual result, pass/fail, commit hash, and screenshot/video location for every test.

## Live demo

1. State that this is an isolated emulator model only.
2. Show the frame/control-flow diagram.
3. Run normal input and explain the observation.
4. Run the vulnerable model and explain the harmless `win()` status.
5. Run the exact same input against the bounds check.
6. Show the canary comparison if complete.

## Report outline

1. Cover: title, two members, repository link.
2. Core goals and stretch features.
3. Frame/control-flow diagram and exact offsets.
4. Work division and integration.
5. Objective progress with evidence.
6. Routine design, register plan, and status display.
7. Build/run instructions for the single handwritten assembly file.
8. Test table: scenario → input → expected → actual → pass/fail.
9. Screenshots.
10. Challenges and lessons learned.

## Viva prompts

- What do `sp` and `ra` mean in your program?
- Why is index 8 outside an eight-word buffer?
- Why does the vulnerable model produce the `win()` status?
- Why does the bounds check stop the same case?
- What does a canary add, and why is it not a replacement for bounds checking?
- Which memory, register, or screen observation proves each result?

## Final checks

- [ ] Replace all workbook `TBD` fields and link this repository.
- [ ] Run every planned test and record actual results.
- [ ] Verify the single assembly file in the Playground.
- [ ] Ensure screenshots match documented test inputs.
- [ ] Make sure both teammates can modify and explain their own routines.
- [ ] Include an honest AI-use disclosure that follows course policy.
