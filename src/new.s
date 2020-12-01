lw s3 0(s1)
sw s3 0(s2)
addi s2 s2 4
lw s3 4(s1)
sw s3 0(s2)
lw s5 0(s2)
addi s2 s2 -4
lw s4 0(s2)
sub s3 s4 s5
sw s3 8(s1)

lw s3 0(s1)
sw s3 0(s2)
addi s2 s2 4
lw s3 4(s1)
sw s3 0(s2)
lw s5 0(s2)
addi s2 s2 -4
lw s4 0(s2)
sub s3 s4 s5
sw s3 12(s1)

lw s3 8(s1)
sw s3 0(s2)
addi s2 s2 4
lw s3 12(s1)
sw s3 0(s2)
lw s5 0(s2)
addi s2 s2 -4
lw s4 0(s2)
mul s3 s4 s5
sw s3 16(s1)

lw s3 20(s1)
sw s3 0(s2)
addi s2 s2 4
lw s3 24(s1)
sw s3 0(s2)
lw s5 0(s2)
addi s2 s2 -4
lw s4 0(s2)
sub s3 s4 s5
sw s3 28(s1)

lw s3 20(s1)
sw s3 0(s2)
addi s2 s2 4
lw s3 24(s1)
sw s3 0(s2)
lw s5 0(s2)
addi s2 s2 -4
lw s4 0(s2)
sub s3 s4 s5
sw s3 32(s1)

lw s3 28(s1)
sw s3 0(s2)
addi s2 s2 4
lw s3 32(s1)
sw s3 0(s2)
lw s5 0(s2)
addi s2 s2 -4
lw s4 0(s2)
mul s3 s4 s5
sw s3 36(s1)

lw s3 16(s1)
sw s3 0(s2)
addi s2 s2 4
lw s3 36(s1)
sw s3 0(s2)
lw s5 0(s2)
addi s2 s2 -4
lw s4 0(s2)
add s3 s4 s5
sw s3 40(s1)

sw s3 44(s1)

addi s3 zero 2
sw s3 48(s1)

addi s3 zero 1
sw s3 0(s2)
addi s2 s2 4
addi s3 zero 7
sw s3 0(s2)
lw s5 0(s2)
addi s2 s2 -4
lw s4 0(s2)
add s3 s4 s5
sw s3 52(s1)

sw s3 48(s1)

