test:
    subq $16,      %rsp
    movq %r11,     8(%rsp)
    movq %rbx,     (%rsp)
    movq %rdi,     %rbx
    movq %rsi,     %rax
    cbe  $1,       %rsi, .L2
    leaq -1(%rsi), %rsi
    call test,    %r11
.L2:
    imulq %rbx,    %rax
    movq (%rsp),   %rbx
    movq 8(%rsp),  %r11
    addq $16,      %rsp
    ret %r11
