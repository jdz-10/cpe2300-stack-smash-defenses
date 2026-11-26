.data
frame:
    .space 40

normal_input:
    .word 10, 20, 30, 40, 50, 60, 70, 80

oversized_lab_input: 
    .word 10, 20, 30, 40, 50, 60, 70, 80, 0, 0x22222222

.text
main: 
#Make the frame
    la a0, frame
    jal ra, init_frame

#Test case: vulnerable_model(%frame, normal_input, 8) 
   
    la a0, frame
    la a1, normal_input
    li a2, 8
    jal ra, vulnerable_model

    
 # Make a0 to status_code(outcome)
    jal ra, status_code

halt:
    li a7, 10
    ecall

#starting values for loop
init_frame:
    li t0, 0
    li t1, 10

#loop for wiping slots and return 0x11111111
init_loop:
    bge t0, t1, init_done
    slli t2, t0, 2
    add t3, a0, t2
    sw zero, 0(t3)
    addi t0, t0, 1
    j init_loop

#checks for any write past 8 word bufferti cross offset 32
init_done:
    li t0, 0xC0DEF00D
    sw t0, 32(a0)
    li t0, 0x11111111
    sw t0, 36(a0)
    ret

vulnerable_model:
    li t0, 10 #Frame_words = 10
    blt t0, a2, vuln_invalid #if for count of 10 gt returns Invalid Case
    
    li t0, 0 

vuln_loop:
    bge t0, a2, vuln_check_ret
    slli t1, t0, 2 
    add t2, a1, t1
    lw t3, 0(t2)
    add t4, a0, t1
    sw t3, 0(t4)
    addi t0,t0, 1 
    j vuln_loop

#checks ret and checks slot[9] for 0x22222222 and returns the vuln_win part
vuln_check_ret:
    lw t0, 36(a0)
    li t1, 0x22222222
    beq t0, t1, vuln_win

    li a0, 0 #return Normal_return (0)
    ret

vuln_win:
    li a0, 1 #return benign_win_reached (1)
    ret

vuln_invalid:
    li a0, 4 #return invalid_case (4)
    ret

with_bound:
    li t0, 8 #Buffer_words = 8
    bltu t0, a2, bound_reject #if count is greater than 8 return Input_rejected

    li t0, 0 

bound_loop:
    bgeu t0, a2, bound_normal
    slli t1, t0, 2 
    add t2, a1, t1
    lw t3, 0(t2)
    add t4, a0, t1
    sw t3, 0(t4)
    addi t0, t0, 1
    j bound_loop

bound_normal:
    li a0,0 #return Normal_return (0)
    ret


bound_reject:
    li a0, 2 #return Input_rejected (2)
    ret

with_canary:
    addi sp, sp, -8 #reserves 8 bytes on stack
    sw ra, 4(sp)    #save address to main
    sw a0, 0(sp)    #save frame pointer

    jal ra, vulnerable_model #call vulnerable_model (frame, input, count)

    lw t0, 0(sp)
    lw ra, 4(sp)
    addi sp, sp, 8

    li t1, 4
    beq a0, t1, canary_done

    lw t1, 32(t0)
    li t2, 0xC0DEF00D
    bne t1, t2, canary_detected

canary_done:
    ret  #return result already in a0

canary_detected:
    li a0, 3  #return canary_detected (3)
    ret
    
status_code: 
    li t0, 0
    beq a0, t0, status_1
    li t0, 1
    beq a0, t0, status_2
    li t0, 2
    beq a0, t0, status_3
    li t0, 3 
    beq a0, t0, status_4

    li a0, 5
    ret

status_1:
    li a0, 1
    ret
status_2:
    li a0, 2
    ret
status_3:
    li a0, 3
    ret
status_4:
    li a0, 4
    ret
