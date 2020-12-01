addi s6, zero, 16
add s7, zero, zero
add sp, zero, zero
add s8, zero, zero
addi ra, zero, _$End
sw ra, 0(sp)
addi sp, sp, 4
sw s6, 0(sp)
addi sp, sp, 4
sw s7, 0(sp)
addi sp, sp, 4
sw s8, 0(sp)
addi sp, sp, 4

jal zero, _L4

_L1:

sw zero, 0(sp)
addi sp, sp, 4
sw zero, 0(sp)
addi sp, sp, 4
add s8, sp, zero
sw zero, 0(sp)
addi sp, sp, 4
sw zero, 0(sp)
addi sp, sp, 4

lw s3, 0(s6)
sw s3, 0(sp)
addi sp, sp, 4
addi s3, zero, 2
sw s3, 0(sp)
addi sp, sp, 4
addi sp, sp, -4
lw s5, 0(sp)
addi sp, sp, -4
lw s4, 0(sp)
add s3, s4, s5
sw s3, 0(s8)

lw s3, 0(s8)
sw s3, 0(sp)
addi sp, sp, 4
addi sp, sp, -4
sw s3, 4(s6)

lw s3, 0(s7)
sw s3, 0(sp)
addi sp, sp, 4
lw s3, 4(s7)
sw s3, 0(sp)
addi sp, sp, 4
addi sp, sp, -4
lw s5, 0(sp)
addi sp, sp, -4
lw s4, 0(sp)
add s3, s4, s5
sw s3, 4(s8)

lw s3, 4(s8)
sw s3, 0(sp)
addi sp, sp, 4
add fp, s6, zero
addi s4, fp, -16
lw ra, 0(s4)
addi sp, sp, -4
lw s4, 0(sp)
sw s4, 0(s7)
addi sp, s7, 4
addi s4, fp, -4
lw s8, 0(s4)
addi s4, fp, -8
lw s7, 0(s4)
addi s4, fp, -12
lw s6, 0(s4)
jal ra, 0

_L4:

sw zero, 0(sp)
addi sp, sp, 4
sw zero, 0(sp)
addi sp, sp, 4
sw zero, 0(sp)
addi sp, sp, 4
add s8, sp, zero
sw zero, 0(sp)
addi sp, sp, 4
sw zero, 0(sp)
addi sp, sp, 4

lw s3, 0(s6)
sw s3, 0(sp)
addi sp, sp, 4
lw s3, 4(s6)
sw s3, 0(sp)
addi sp, sp, 4
addi sp, sp, -4
lw s5, 0(sp)
addi sp, sp, -4
lw s4, 0(sp)
sub s3, s4, s5
sltiu s3, s3, 1
sw s3, 0(s8)

lw s3, 0(s8)
bne s3, zero, _L2

lw s3, 0(s6)
sw s3, 0(sp)
addi sp, sp, 4

lw s3, 4(s6)
sw s3, 0(sp)
addi sp, sp, 4
addi s3, zero, 3
sw s3, 0(sp)
addi sp, sp, 4
addi sp, sp, -4
lw s5, 0(sp)
addi sp, sp, -4
lw s4, 0(sp)
add s3, s4, s5
sw s3, 0(s8)

lw s3, 0(s8)
sw s3, 0(sp)
addi sp, sp, 4

addi ra, zero, _$Ret1
sw ra, 0(sp)
addi sp, sp, 4
sw s6, 0(sp)
addi sp, sp, 4
sw s7, 0(sp)
addi sp, sp, 4
sw s8, 0(sp)
addi sp, sp, 4
addi s7, sp, -24
add s6, sp, zero
jal zero, _L1
_$Ret1:

addi sp, sp, -4
lw s3, 0(sp)
sw s3, 0(s8)

lw s3, 0(s8)
sw s3, 0(sp)
addi sp, sp, 4
addi sp, sp, -4
sw s3, 0(s6)

jal zero, _L3

_L2:

lw s3, 0(s6)
sw s3, 0(sp)
addi sp, sp, 4
lw s3, 4(s6)
sw s3, 0(sp)
addi sp, sp, 4
addi sp, sp, -4
lw s5, 0(sp)
addi sp, sp, -4
lw s4, 0(sp)
add s3, s4, s5
sw s3, 0(s8)

lw s3, 0(s8)
sw s3, 0(sp)
addi sp, sp, 4
addi sp, sp, -4
sw s3, 0(s6)

lw s3, 4(s6)
sw s3, 0(sp)
addi sp, sp, 4
lw s3, 8(s6)
sw s3, 0(sp)
addi sp, sp, 4
addi sp, sp, -4
lw s5, 0(sp)
addi sp, sp, -4
lw s4, 0(sp)
add s3, s4, s5
sw s3, 4(s8)

lw s3, 4(s8)
sw s3, 0(sp)
addi sp, sp, 4
addi sp, sp, -4
sw s3, 4(s6)

_L3:

addi s3, zero, 0
sw s3, 0(sp)
addi sp, sp, 4
add fp, s6, zero
addi s4, fp, -16
lw ra, 0(s4)
addi sp, sp, -4
lw s4, 0(sp)
sw s4, 0(s7)
addi sp, s7, 4
addi s4, fp, -4
lw s8, 0(s4)
addi s4, fp, -8
lw s7, 0(s4)
addi s4, fp, -12
lw s6, 0(s4)
jal ra, 0

_$End: