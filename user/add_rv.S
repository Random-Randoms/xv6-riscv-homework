.data                                      # Data section
buffer:                                    # bf
    .skip 24

msg_fmt: .asciz "format error: two space-separated numbers expected\n"  # Format error message
out_fmt: .asciz "%d\n"                                                  # Output format
msg_div: .asciz "|"                                                     # Message divisor
msg_nl:  .asciz "\n"                                                    # Newline

.text                                      # Text (code) section
.global main                               # Exportable object (label)



# regs
# t0 | i            | int
# t1 | cur          | char*
# t2 | bf           | char*
# t3 | flipl, flipr | int
# t4 | snd          | int 
# t5 | reading      | int
# t6 | status       | int

# t6 | l or r       | int
# a0 | fsr          |
main:
    # initials
    li   t6, 0                  # status = 0
    li   t0, 0                  # i = 0
    li   t5, 1                  # reading = 1
    li   t3, 0                  # flipl = false, flipr = false
    la   t2, buffer             # buffer

    # read input
    addi a0, t2, 0
    addi a1, zero, 24
    call gets  

    # initial fst
    addi a0, t2, 0               # fst = bf
loop:            
    addi t0, t0, -24
    beqz t0, loop_end            # if i == 24 finish loop
    addi t0, t0, 25              # increment i otherwise

    add  t2, t2, t0        
    lb   t1, -1(t2)              # load bf[i]  
    sub  t2, t2, t0

    beqz t1, loop_end           # if term zero found finish loop

    addi t1, t1, -45
    beqz t1, found_minus        # minus found

    mul  t5, zero, t5           # otherwise reading = 0

    addi t1, t1, 35             # \n found
    beqz t1, found_nl

    addi t1, t1, -3
    beqz t1, found_nl           # \r found

    addi t1, t1, -19
    beqz t1, found_space        # space found

    addi t1, t1, -16
    bltz t1, format_fail        # if symbol < '0' format fail

    addi t1, t1, -9
    bgtz t1, format_fail        # if symbol > '9' format fail

    j loop                      # continue loop

found_minus:
    beqz t5, format_fail        # if reading == 0 format fail
    addi t5, t5, -2

    beqz t5, found_minus_r_2    # if reading == 2 proceed
    addi t5, t5, 1              # otherwise reading == 1, make it 0
    addi a0, a0, 1              # ++fst
    addi t3, t3, -2             # flipl = true
    j loop                      # continue loop
    
found_minus_r_2:
    addi t3, t3, 1              # flipr = true
    addi t4, t4, 1              # ++snd
    j loop                      # continue loop

found_space:
    bnez t6, format_fail        # if status != 0 format fail

    addi t5, t5, 2              # set reading = 2
    addi t6, t6, 1              # set status = 1 otherwise
    add  t4, t2, t0             # set snd = bf + i + 1
    j loop                      # continue loop

found_nl:
    add t2, t2, t0
    sb  zero, -1(t2)
    sub t2, t2, t0              # change nl to term zero
    j loop_end                  # finish loop

format_fail:
    addi t6, zero, -1           # set status = -1
    j loop                      # continue loop

loop_end:
    addi t6, t6, -1             
    bnez t6, out_fmt_error      # if status not 1 output error message

compute_sum:
    # atoi for first number
    call atoi                   # read from fst, already in a0
    addi t6, a0, 0              # store in t6

    # check if need to flip
    bgez t3, skip_fst
    addi t3, t3, 2              # rollback flipl = true
    neg  t6, t6                 # flip t6       

skip_fst:
    # atoi for the second number
    addi a0, t4, 0              # read from snd
    call atoi

    # check if need to flip
    beqz t3, skip_snd
    neg a0, a0

skip_snd:
    # perform addition
    add  t6, t6, a0

    # output number
    la   a0, out_fmt
    addi a1, t6, 0
    call printf

    # output buffered string
    j out_buf_str

out_fmt_error:
    # output error message to stderr
    addi a0, zero, 2
    la   a1, msg_fmt            
    call fprintf

    # exit
    li   a7, 2                  # Exit system call
    li   a0, 2                  # Exit status (format fail)
    ecall

out_buf_str:
    # print |
    la   a0, msg_div
    call printf

    # print string
    addi a0, t2, 0              # set first arg to bf
    call printf

    # print |
    la   a0, msg_div
    call printf

    # print \n
    la   a0, msg_nl
    call printf

    # exit
    li   a7, 2                  # Exit system call
    li   a0, 0                  # Exit status (success)
    ecall
