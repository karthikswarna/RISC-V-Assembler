
main:
addi t0, t0, -10
sub t0, t0, t1
and t3, t3, t0
xor t3, t3, t4

loop: 
sw t1, 0(t0)        
addi x3, x3, -1
sw x3, 8(t4)
lw x4, 3(x5)
ori x3,x4, 9
jal ra, exit
beq x0, x3, while       # label is wrong, while is used instead of loop

exit: 
sw x5, 3(x4)
