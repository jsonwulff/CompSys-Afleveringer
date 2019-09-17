int i = 0;
while (1) {
char b;
int read = fread(&b, 1, 1, f);

if (read == 0 && i == 0) {
    cur_type = EMPTY;
    break;
} else if (read == 0) {
    break;
} else if ((b >= 0x07 && b <= 0x0D) || b == 0x1B || (b >= 0x20 && b <= 0x7E)) {
    cur_type = ASCII;
} else {
    cur_type = DATA;
    break;
}
i++;
}