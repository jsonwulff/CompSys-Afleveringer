/*** DECODE ***/
val reg_z = pick_bits(4, 4, inst_bytes[2]);

// Decode memory load and store
bool is_load = (is(COPY, minor_op) || is(IMM_ADD_REG, minor_op)) &&
               (is_reg_movq_mem || is_imm_movq_mem);

// Decode address generation operations
bool use_z = is_leaq3 || is_leaq7;

// determine instruction size
bool size2 = is_return || is_reg_arithmetic || is_reg_movq || is_reg_movq_mem ||
             is_leaq2;

val ins_size =
    or (or (use_if(size2, from_int(2)), use_if(size3, from_int(3))),
        or (use_if(size6, from_int(6)),
            or (use_if(size7, from_int(7)), use_if(size10, from_int(10)))));

bool use_imm = is_imm_arithmetic || is_imm_movq || is_imm_movq_mem ||
               is_leaq6 || is_leaq7 || is_imm_cbranch;

bool reg_wr_enable = is_reg_movq || is_reg_arithmetic || is_imm_arithmetic ||
                     is_imm_movq || is_load || is_leaq2 || is_leaq3 ||
                     is_leaq6 || is_leaq7;