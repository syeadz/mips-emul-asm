lw $t1, 48($zero)
lw $t2, 52($zero)
add $t0, $t1, $t2
sw $t0, 56($zero)
lw $t3, 56($zero)
beq $t0, $t3, 9
