#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "arithmetic.h"
#include "compute.h"
#include "ip_reg.h"
#include "memory.h"
#include "registers.h"
#include "support.h"
#include "wires.h"

// major opcodes
#define RETURN 0x0
#define REG_ARITHMETIC 0x1
#define REG_MOVQ 0x2
#define REG_MOVQ_MEM 0x3
#define CFLOW 0x4
#define IMM_ARITHMETIC 0x5
#define IMM_MOVQ 0x6
#define IMM_MOVQ_MEM 0x7
#define LEAQ2 0x8
#define LEAQ3 0x9
#define LEAQ6 0xA
#define LEAQ7 0xB
#define IMM_CBRANCH 0xF

#define JMP 0xF
#define CALL 0xE

// minor op codes
#define LOAD 0x1        // movq (s),d
#define STORE 0x9       // movq d,(s)
#define LOAD_IMM 0x5    // movq i(s),d
#define STORE_IMM 0xD  // movq movq d,i(s)

int main(int argc, char* argv[]) {
  // Check command line parameters.
  if (argc < 2) error("missing name of programfile to simulate");

  if (argc < 3) error("Missing starting address (in hex notation)");

  /*** SETUP ***/
  // We set up global state through variables that are preserved between
  // cycles.

  // Program counter / Instruction Pointer
  ip_reg_p ip = ip_reg_create();

  // Register file:
  reg_p regs = regs_create();

  // Memory:
  // Shared memory for both instructions and data.
  mem_p mem = memory_create();
  memory_read_from_file(mem, argv[1]);

  int start;
  int scan_res = sscanf(argv[2], "%x", &start);
  if (scan_res != 1) error("Unable to interpret starting address");

  if (argc == 4) {  // tracefile specified, hook memories to it
    memory_tracefile(mem, argv[3]);
    regs_tracefile(regs, argv[3]);
    ip_reg_tracefile(ip, argv[3]);
  }
  ip_write(ip, from_int(start), true);

  // a stop signal for stopping the simulation.
  bool stop = false;

  // We need the instruction number to show how far we get
  int instruction_number = 0;

  while (!stop) {  // for each cycle:
    val pc = ip_read(ip);
    ++instruction_number;
    printf("%d %lx\n", instruction_number, pc.val);

    /*** FETCH ***/
    // We're fetching 10 bytes in the form of 10 vals with one byte each
    val inst_bytes[10];
    memory_read_into_buffer(mem, pc, inst_bytes, true);

    /*** DECODE ***/
    // read 4 bit values
    val major_op = pick_bits(4, 4, inst_bytes[0]);
    val minor_op = pick_bits(0, 4, inst_bytes[0]);

    val reg_d = pick_bits(4, 4, inst_bytes[1]);
    val reg_s = pick_bits(0, 4, inst_bytes[1]);

    val leaq_z = pick_bits(4, 4, inst_bytes[2]);
    val leaq_v = pick_bits(4, 4, inst_bytes[2]);

    // decode instruction type
    // read major operation code
    bool is_return = is(RETURN, major_op);  // 0000 - IMMPLEMENTED
    bool is_reg_arithmetic = is(REG_ARITHMETIC, major_op);  // 0001 -
    bool is_reg_movq = is(REG_MOVQ, major_op);          // 0010 - IMPLEMENTED
    bool is_reg_movq_mem = is(REG_MOVQ_MEM, major_op);  // 0011 - IMPLEMENTED
    bool is_cflow = is(CFLOW, major_op);                // 0100
    bool is_imm_arithmetic = is(IMM_ARITHMETIC, major_op);  // 0101
    bool is_imm_movq = is(IMM_MOVQ, major_op);          // 0110 - IMPLEMENTED
    bool is_imm_movq_mem = is(IMM_MOVQ_MEM, major_op);  // 0111
    bool is_leaq2 = is(LEAQ2, major_op);                // 1000 - IMPLEMENTED
    bool is_leaq3 = is(LEAQ3, major_op);                // 1001
    bool is_leaq6 = is(LEAQ6, major_op);                // 1010
    bool is_leaq7 = is(LEAQ7, major_op);                // 1011
    bool is_imm_cbranch = is(IMM_CBRANCH, major_op);    // 1111

    bool is_call = is(CALL, minor_op) && is_cflow;
    bool is_jmp = is(JMP, minor_op) && is_cflow;

    bool is_load = (is(LOAD, minor_op) || is(LOAD_IMM, minor_op)) &&
                   (is_reg_movq_mem || is_imm_movq_mem);  // 0001
    bool is_store = (is(STORE, minor_op) || is(STORE_IMM, minor_op)) &&
                    (is_reg_movq_mem || is_imm_movq_mem);  // 0001

    // printf("is_store: %d \n", is_store);
    // determine instruction size
    bool size2 = is_return || is_reg_arithmetic || is_reg_movq ||
                 is_reg_movq_mem || is_leaq2;
    bool size3 = is_leaq3;
    bool size6 = is_cflow || is_imm_arithmetic || is_imm_movq ||
                 is_imm_movq_mem || is_leaq6;
    bool size7 = is_leaq7;
    bool size10 = is_imm_cbranch;

    val ins_size =
        or (or (use_if(size2, from_int(2)), use_if(size3, from_int(3))),
            or (use_if(size6, from_int(6)),
                or (use_if(size7, from_int(7)), use_if(size10, from_int(10)))));

    // setting up operand fetch and register read and write for the datapath:
    bool use_imm = is_imm_arithmetic || is_imm_movq || is_imm_movq_mem ||
                   is_leaq6 || is_leaq7 || is_imm_cbranch;
    val reg_read_dz = reg_d;
    // - other read port is always reg_s
    // - write is always to reg_d
    bool reg_wr_enable = is_reg_movq || is_reg_arithmetic ||
                         is_imm_arithmetic || is_imm_movq || is_load ||
                         is_leaq2 || is_leaq3;

    // Datapath:
    val offset_2 =
        or
        (or (put_bits(0, 8, inst_bytes[2]), put_bits(8, 8, inst_bytes[3])),
         or (put_bits(16, 8, inst_bytes[4]), put_bits(24, 8, inst_bytes[5])));
    val offset_3 =
        or
        (or (put_bits(0, 8, inst_bytes[3]), put_bits(8, 8, inst_bytes[4])),
         or (put_bits(16, 8, inst_bytes[5]), put_bits(24, 8, inst_bytes[6])));
    val offset_6 =
        or
        (or (put_bits(0, 8, inst_bytes[6]), put_bits(8, 8, inst_bytes[7])),
         or (put_bits(16, 8, inst_bytes[8]), put_bits(24, 8, inst_bytes[9])));

    val imm_i = or (use_if((is_imm_movq || is_imm_movq_mem || is_leaq6 ||
                            is_imm_cbranch || is_imm_arithmetic),
                           offset_2),
                    (use_if((is_leaq7), offset_3)));

    val address_p =
        or (use_if(is_cflow, offset_2), use_if(!is_cflow, offset_6));
    val sext_imm_i = sign_extend(31, imm_i);

    /*** EXECUTE ***/
    // read registers
    val reg_out_a = reg_read(
        regs,
        reg_read_dz);  // <-- will generate warning, as we don't use it yet
    val reg_out_b = reg_read(regs, reg_s);
    val op_b = or (use_if(use_imm, sext_imm_i), use_if(!use_imm, reg_out_b));

    // perform calculations
    // not really any calculations yet!
    val arithmetic_result = alu_execute(minor_op, reg_out_a, op_b);

    // Address generator
    // generate address for memory access
    val agen = add(reg_out_b, sext_imm_i);
    val agen_result =
        or (use_if(!is_imm_movq_mem, reg_out_b), use_if(is_imm_movq_mem, agen));

    // address of succeeding instruction in memory
    val pc_incremented = add(pc, ins_size);

    // determine the next position of the program counter
    val pc_next =
        or (use_if(is_return, reg_out_b), use_if(!is_return, pc_incremented));

    /*** MEMORY ***/
    // read from memory if needed
    // Not implemented yet!
    val mem_out = memory_read(mem, agen_result, is_load);

    /*** WRITE ***/
    /*** RESULT SELECT ***/
    // choose result to write back to register
    val datapath_result =
    or(
            use_if(is_reg_movq || is_imm_movq, op_b),
            or(
                use_if(is_load, mem_out),
                use_if(is_imm_arithmetic || is_reg_arithmetic, arithmetic_result))
        );


    // write to register if needed
    reg_write(regs, reg_d, datapath_result, reg_wr_enable);

    // write to memory if needed
    // Not implemented yet!
    memory_write(mem, agen_result, reg_out_a, is_store);

    // update program counter
    ip_write(ip, pc_next, true);

    // terminate when returning to zero
    if (pc_next.val == 0 && is_return) stop = true;
  }
  memory_destroy(mem);
  regs_destroy(regs);

  printf("Done\n");
}