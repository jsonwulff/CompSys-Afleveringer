long p(long a1) {
  temp 1 = a1 >> 63;
  temp 2 = temp1 ^ a1;
  return temp 2 - temp1;
}