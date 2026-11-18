/*
 * Stack Smash -> Defenses: reference C model
 *
 * This is planning/reference code for the CPE 2300 RISC-V Playground project.
 * It intentionally models a stack frame as an array of slots instead of relying
 * on a host compiler's stack layout or performing an actual out-of-bounds write.
 * Translate the design to RISC-V by hand; do not submit AI-generated assembly.
 */

#include <stdint.h>
#include <stddef.h>

enum {
    BUFFER_WORDS = 8,
    CANARY_SLOT = 8,
    RETURN_SLOT = 9,
    FRAME_WORDS = 10,
    CANARY_VALUE = 0xC0DEF00Du,
    RETURN_TO_CALLER = 0x11111111u,
    RETURN_TO_WIN = 0x22222222u
};

typedef enum {
    NORMAL_RETURN,
    BENIGN_WIN_REACHED,
    INPUT_REJECTED,
    CANARY_DETECTED,
    INVALID_CASE
} Outcome;

typedef struct {
    /* Logical layout, from lower to higher slot index:
       buffer[0..7] | canary | saved return target */
    uint32_t slot[FRAME_WORDS];
} FrameModel;

static void init_frame(FrameModel *frame) {
    size_t i;

    for (i = 0; i < FRAME_WORDS; ++i) {
        frame->slot[i] = 0;
    }
    frame->slot[CANARY_SLOT] = CANARY_VALUE;
    frame->slot[RETURN_SLOT] = RETURN_TO_CALLER;
}

/*
 * Controlled vulnerable model: it permits writes beyond the logical eight-word
 * buffer but keeps them inside the ten-word model. This is a deterministic
 * representation of the memory-safety bug, not an actual C buffer overflow.
 */
static Outcome copy_vulnerable_model(FrameModel *frame,
                                     const uint32_t input[], size_t count) {
    size_t i;

    if (count > FRAME_WORDS) {
        return INVALID_CASE;
    }

    for (i = 0; i < count; ++i) {
        frame->slot[i] = input[i];
    }

    if (frame->slot[RETURN_SLOT] == RETURN_TO_WIN) {
        return BENIGN_WIN_REACHED;
    }
    return NORMAL_RETURN;
}

/* Required defense: reject an input before it crosses the logical buffer. */
static Outcome copy_with_bound(FrameModel *frame,
                               const uint32_t input[], size_t count) {
    size_t i;

    if (count > BUFFER_WORDS) {
        return INPUT_REJECTED;
    }

    for (i = 0; i < count; ++i) {
        frame->slot[i] = input[i];
    }
    return NORMAL_RETURN;
}

/* Stretch defense: detect a logical overwrite before accepting the return. */
static Outcome copy_with_canary(FrameModel *frame,
                                const uint32_t input[], size_t count) {
    Outcome result = copy_vulnerable_model(frame, input, count);

    if (result == INVALID_CASE) {
        return result;
    }
    if (frame->slot[CANARY_SLOT] != CANARY_VALUE) {
        return CANARY_DETECTED;
    }
    return result;
}

/*
 * Map this routine to a small framebuffer/MMIO status display in the Playground.
 * For example, assign one color/status code for each Outcome.
 */
static uint32_t status_code(Outcome outcome) {
    switch (outcome) {
        case NORMAL_RETURN:      return 1;
        case BENIGN_WIN_REACHED: return 2;
        case INPUT_REJECTED:     return 3;
        case CANARY_DETECTED:    return 4;
        default:                 return 5;
    }
}

/* Fixed, deterministic test data for the demo. */
static const uint32_t normal_input[BUFFER_WORDS] = {
    10, 20, 30, 40, 50, 60, 70, 80
};

/* Slot 8 changes the canary; slot 9 models a changed saved return target. */
static const uint32_t oversized_lab_input[FRAME_WORDS] = {
    10, 20, 30, 40, 50, 60, 70, 80,
    0, RETURN_TO_WIN
};

/* Suggested test sequence for the hand-written RISC-V program:
   1. init_frame + copy_vulnerable_model(normal_input, 8) -> NORMAL_RETURN
   2. init_frame + copy_vulnerable_model(oversized_lab_input, 10)
      -> BENIGN_WIN_REACHED
   3. init_frame + copy_with_bound(oversized_lab_input, 10)
      -> INPUT_REJECTED
   4. Stretch: init_frame + copy_with_canary(oversized_lab_input, 10)
      -> CANARY_DETECTED
 */

int main(void) {
    FrameModel frame;
    Outcome outcome;

    init_frame(&frame);
    outcome = copy_vulnerable_model(&frame, normal_input, BUFFER_WORDS);

    /* In the assembly version, pass status_code(outcome) to show_status(). */
    return (int)status_code(outcome);
}
