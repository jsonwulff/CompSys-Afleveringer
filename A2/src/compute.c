#include "compute.h"
#include "arithmetic.h"

bool bool_xor(bool a, bool b) { return a^b; }
bool bool_not(bool a) { return !a; }

val shift_left(val a, val v) { return from_int(a.val << v.val); }
val shift_right_unsigned(val a, val v) { return from_int(a.val >> v.val); }
val shift_right_signed(val a, val v) { return from_int(((unsigned)a.val) >> v.val); }

val mul(val a, val b) { return from_int(a.val * b.val); }
val imul(val a, val b) { return from_int((unsigned)((signed)a.val * (signed)b.val)); }

bool comparator(val comparison, val op_a, val op_b) {
    val neg_b = neg(64, op_b);
    generic_adder_result adder_result = generic_adder(op_a, neg_b, true); // subtract
    val res = adder_result.result;

    // FIX THIS
    bool a_sign = pick_one(63, op_a);
    bool b_sign = pick_one(63, op_b);
    bool r_sign = pick_one(63, res);
    bool of = (a_sign && !b_sign && !r_sign)
      || (!a_sign && b_sign && r_sign);
      
      /* (!(a_sign || b_sign) &&   r_sign) ||
	 (  a_sign && b_sign  && (!r_sign)); */
    bool sf = r_sign;
    bool zf = !reduce_or(res);
    bool cf = adder_result.cf;
    
    bool res_e  = is(E, comparison)  & zf;
    bool res_ne = is(NE, comparison) & bool_not(zf);
    bool res_g  = is(G, comparison)  & bool_xor(sf, of);
    bool res_ge = is(GE, comparison) & (bool_xor(sf, of) || zf);
    bool res_l  = is(L, comparison)  & bool_not(bool_xor(sf, of)) & bool_not(zf);
    bool res_le = is(LE, comparison) & bool_not(bool_xor(sf, of));
    bool res_b  = is(B, comparison)  & bool_not(cf || zf);
    bool res_be = is(BE, comparison) & bool_not(cf);
    bool res_a  = is(A, comparison)  & cf;
    bool res_ae = is(AE, comparison) & (cf || zf);
    return res_e || res_ne || res_l || res_le || res_g || res_ge
        || res_a || res_ae || res_b || res_be;
}

val shifter(bool is_left, bool is_signed, val op_a, val op_b) {
    val res = or(use_if(is_left, shift_left(op_a, op_b)),
		 use_if(!is_left,
			or(use_if(is_signed, shift_right_signed(op_a,op_b)),
			   use_if(!is_signed, shift_right_unsigned(op_a, op_b)))));
    return res;
}

val multiplier(bool is_signed, val op_a, val op_b) {
    val res = or(use_if(is_signed, imul(op_a, op_b)),
		 use_if(!is_signed, mul(op_a, op_b)));
    return res;
}

val alu_execute(val op, val op_a, val op_b) {
    bool is_sub = is(SUB, op);
    bool is_add = is(ADD, op);
    val val_b = or( use_if(!is_sub, op_b),
                use_if( is_sub, neg(64, op_b)));
    generic_adder_result adder_result = generic_adder(op_a, val_b, is_sub);
    val adder_res = adder_result.result;
    val res = or(use_if(is_add, adder_res),
              or(use_if(is_sub, adder_res),
              or(use_if(is(AND, op), and(op_a, op_b)),
              or(use_if(is(OR, op),  or(op_a, op_b)),
		 use_if(is(XOR, op), xor(op_a, op_b))))));
    return res;
}

val address_generate(val op_z_or_d, val op_s, val imm, val shift_amount,
		     bool sel_z_or_d, bool sel_s, bool sel_imm) {
    val val_a = use_if(sel_z_or_d, op_z_or_d);
    val val_b = use_if(sel_s, op_s);
    val val_imm = use_if(sel_imm, imm);

    val shifted_a = shift_left(val_a, shift_amount);

    val effective_addr = add(add(shifted_a, val_b), val_imm);
    return effective_addr;
}

/*
compute_execute_result compute_execute(val op_z_or_d, val op_s, val imm,
                                       bool sel_z_or_d, bool sel_s, bool sel_imm,
                                       val shift_amount, bool use_agen,
                                       val alu_op, val condition) {
    val alu_input_b = or(val_b, val_imm);
    val alu_result = alu_execute(alu_op, op_z_or_d, alu_input_b);
    bool cond_met = comparator(condition, op_z_or_d, alu_input_b);

    val result = or(use_if(!use_agen, alu_result),
                    use_if( use_agen, effective_addr));

    compute_execute_result retval;
    retval.result = result;
    retval.cond_met = cond_met;
    return retval;
}
*/
