

// 1 p1:
// 2     subq $8, %rsp
// 3     movq %r11, (%rsp)
// 4     movq %rdi, %rdx
// 5     sarq $63, %rdx
// 6     movq %rdi, %rax
// 7     xorq %rdx, %rax
// 8     subq %rdx, %rax
// 9     movq (%rsp), %r11
// 10    addq $8, %rsp
// 11    ret %r11

// long p (long a1) {
//
// {‰rdi -> a1, %rdx = temp1, %rax = temp2}
//
// temp 1 = a1 >> 64;
// temp 2 = temp1 ^ a1
// return temp 2 - temp1

// }
