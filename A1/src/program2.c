... p2(... a1, ... a2) {
  (% rcx->a1, % rdi-- > a2, % rax-- > temp1) temp = 0;
  if (a1 == 0;) {
    DO L3
  } else if (a2 == a1) {
    temp++;
    continue
  } else {
    add 1 ‰rdx;
    add 1 ‰rax;
    continue
  }
  return % rax - % rdx  // %caller saved
}
