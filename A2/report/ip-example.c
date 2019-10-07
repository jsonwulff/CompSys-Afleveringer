// address of succeeding instruction in memory
val pc_incremented = add(pc, ins_size);

// Perform CFLOW comparision
bool cb = comparator(minor_op, reg_out_a, op_b);

// IP select
val pc_next =
    or (or (use_if(is_jmp, address_p),
            use_if((is_cflow || is_imm_cbranch) && cb, address_p)),
        or (use_if(is_return, reg_out_b),
            use_if(is_reg_arithmetic || is_reg_movq || is_reg_movq_mem ||
                       is_imm_arithmetic || is_imm_movq || is_imm_movq_mem ||
                       is_leaq2 || is_leaq3 || is_leaq6 || is_leaq7,
                   pc_incremented)));