.data                                      # Data section

msg_fmt: .asciz "format error: two space-separated numbers expected\n"  # Format error message
msg_alc: .asciz "memory allocation failed\n"                            # Malloc error message
out_fmt: .asciz "%u\n"                                                  # Output format
msg_div: .asciz "|"                                                     # Message divisor
msg_nl:  .asciz "\n"                                                    # Newline

.text                                      # Text (code) section
.global main                               # Exportable object (label)

# regs
# t0 | i            | int
# t1 | cur*         | char*
# t2 | bf           | char*
# t3 | bfsize = 20  | int
# t4 | snd          | int 
# t5 | cur          | int
# t6 | status       | int

# t6 | l or r       | int

main:
    # initials
    li   t6, 0                  # status = 0
    li   t3, 20                 # bfsize = 20
    li   t0, 0                  # i = 0

    # allocate 20 bytes for bf
    li   a0, 20                 
    call malloc
    addi t2, a0, 0
    beqz t2, out_alc_error

    # read input
    addi a0, t2, 0
    addi a1, t3, 0
    call gets  
loop:            
    beq  t0, t3, loop_end       # if i == bfsize finish loop
    addi t0, t0, 1              # increment i otherwise

    add  t2, t2, t0        
    lb   t1, -1(t2)              # load bf[i]  
    sub  t2, t2, t0

    beqz t1, loop_end           # if term zero finish loop

    addi t1, t1, -10            # \n found
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

found_space:
    bnez t6, format_fail        # if status != 0 format fail

    addi t6, t6, 1              # set status = 1 otherwise
    add  t4, t2, t0             # set snd = bf + i + 1
    j loop                      # continue loop

found_nl:
    add t2, t2, t0
    sb  zero, -1(t2)
    sub t2, t2, t0              # change nl to term zero

    j loop_end

format_fail:
    addi t6, zero, -1           # set status = -1
    j loop

loop_end:
    addi t6, t6, -1             
    bnez t6, out_fmt_error          # if status not 1 output error message

compute_sum:
    # atoi for first number
    addi a0, t2, 0            # read from bf
    call atoi
    addi t6, a0, 0            # store in t6

    # atoi for the second number
    addi a0, t4, 0            # read from snd
    call atoi
    add  t6, t6, a0           # add to first number

    # output number
    la   a0, out_fmt
    addi a1, t6, 0
    call printf

    # output buffered string
    j out_buf_str

out_alc_error:
    la   a0, msg_alc
    call printf

    j exit_

out_fmt_error:
    # output error message
    la   a0, msg_fmt            
    call printf    

out_buf_str:
    # print |
    la   a0, msg_div
    call printf

    # print string
    addi a0, t2, 0            # set first arg to bf
    call printf

    # print |
    la   a0, msg_div
    call printf

    # print \n
    la   a0, msg_nl
    call printf

exit_:
    # exit
    li   a7, 2                  # Exit system call
    li   a0, 0                  # Exit status (success)
    ecall
