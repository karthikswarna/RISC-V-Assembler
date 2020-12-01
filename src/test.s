# comment comment comment comment 
add t0, t0, t6
lw t2, 8096(t0)             
addi t1, t1, 4
loop: 
sw t1, 0(t0)        # comment
addi x3, x3, 8096
beq x3, x0, exit         # comment
jal ra, exit
beq x0, x3, loop
exit: 
sw x1, 32(x0)