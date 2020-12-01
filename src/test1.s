
add t0, t0, t6
lw t2, 4(t0)             
addi t1, t1, 4


while: 
sw t1, 0(t0)        # comment
addi x3, x3, -1
ori x3,x4, 9
sll x4,x4,x3
jal ra, exitwhile
beq x0, x3, while


exitwhile: 
add x5,x5,x3

loop: 
lw t1, 0(t0)  
xori x3, x3, 2
ori x3,x4, 9
sll x4,x4, x3
beq x0, x3, exitloop

exitloop: 
sw x5, 3(x4)


