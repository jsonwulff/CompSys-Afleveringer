1 p2 : 2 subq $8, %
                      rsp  // $8 = 8                   %rsp = stack pointer
                      3 movq %
                      r11,
    (% rsp)  // %r11 = caller saved      %rsp = stack pointer

        4 xorq
        % edx,
    %
        edx  // %edx = 3rd arg 32bit     %edx = 3rd arg 32bit
        5 xorq
        % eax,
    % eax  // %eax = Return 32 bit     %eax = Return val 32 bit
        6.L1 : 9 cbe $0,
    % rcx,
    .L3  //                          %rcx = 4th arg
        10 cbe
        % rdi,
    % rcx,
    .L2  // %rdi = 1st arg           %rcx = 4th arg
    11 leaq(% rsi, % rdx, 8),
    %
        r11  // %rsi = 2nc arg           %rdx = 3rd arg              %r11 =
             // Caller saved
        13 movq
        % rcx,
    (% r11)  // %rcx = 4th arg           %r11 = Caller saved
    14 addq $1,
    % rdx  //                          %rdx = 3rd arg              //Look like a
           //                          counter/iterator
        15.L2 : 16 addq $1,
    % rax  // %rax = Return value
        17 jmp.L1.L3 : 18 leaq(% rsi, % rdx, 8),
    % r11  // %rsi = 2nd arg           %rdx = 3rd arg
        19 movq $0,
    %
        r10  //                          %r10 = Caller saved
        20 movq
        % r10,
    (% r11)  // %r10 = Caller saved      %r11 = Caller saved
        21 subq
        % rdx,
    % rax  // %rdx = 3rd arg           %rax = Return value

        22 movq(% rsp),
    % r11  // %rsp = stack pointer     %r11 = caller saved
        23 addq $8,
    %
        rsp  //                          %rsp = stack pointer
        24 ret
        %
        r11  // %caller saved

        ... p2(... a1, ... a2) {
  (% rcx->a1, % rdi-- > a2, % rax-- > temp1) temp = 0;
  $ if (a1 == 0;) { DO L3 }
  else if (a2 == a1) {
    L2 temp++;
    continue
  }
  else {
    add 1 ‰rdx;
    add 1 ‰rax;
    continue
  }
  return % rax - % rdx

                         temp += 1;
  15.L2 : 16 addq $1, %
                          rax  // %rax = Return value
                          17 jmp.L1.L3 :

                          21 subq %
                          rdx,
      % rax       // %rdx = 3rd arg           %rax = Return value
          24 ret  // %caller saved
}
