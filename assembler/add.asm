lw $t1, 12($zero)
lw $t2, 13($zero)
add $t0, $t1, $t2
sw $t0, 14($zero)
lw $t3, 14($zero)
beq $t0, $t3, 15