// perform arithmic calculations
val arithmetic_result = alu_execute(minor_op, reg_out_a, op_b);

// Address generator
val agen = address_generate(reg_out_z, reg_out_b, sext_imm_i, op_v, use_z,
                            use_s, use_imm);

/*** RESULT SELECT ***/
val datapath_result =
    or
    (use_if(is_reg_movq || is_imm_movq, op_b),
     or (use_if(is_load, mem_out),
         or (use_if(is_imm_arithmetic || is_reg_arithmetic, arithmetic_result),
             use_if(is_leaq2 || is_leaq3 || is_leaq6 || is_leaq7,
                    agen_result))));