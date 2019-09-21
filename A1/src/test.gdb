start
#
p "Printing diffrent representations"
p /x 192
p /t 192
p /t 0x80
p /u 0x80
p /x 0b110
p /u 0b110
#
p "Testing UTF 16 BOM byte"
p BOM_FF(255) !=0
p BOM_FE(254) !=0
#
p "Some basic tests.."
p UTF8_CONT(128) != 0
p UTF8_2B(192) != 0
p UTF8_3B(224) != 0
p UTF8_4B(240) != 0
#
p "Testing UTF8_CONT.."
p UTF8_CONT(128 + 1) != 0
p UTF8_CONT(128 | 1) != 0
p UTF8_CONT(128 | 63) != 0
p UTF8_CONT(128 | 63) > 0
p UTF8_CONT(128 + 64) == 0
p UTF8_CONT(128 | 64) == 0
#
p "Testing UTF8_2B.."
p UTF8_2B(128 + 64) != 0
p UTF8_2B(128 | 64) != 0
p UTF8_2B(128 | 64 | 31) != 0
p UTF8_2B(128 | 64 | 31) > 0
p UTF8_2B(128 + 64 + 32) == 0
p UTF8_2B(128 | 64 | 32) == 0
#
p "Testing UTF8_3B.."
p UTF8_3B(128 + 64 + 32) != 0
p UTF8_3B(128 | 64 | 32) != 0
p UTF8_3B(128 | 64 | 32 | 15) != 0
p UTF8_3B(128 | 64 | 32 | 15) > 0
p UTF8_3B(128 + 64 + 32 + 16) == 0
p UTF8_3B(128 | 64 | 32 | 16) == 0
#
p "Testing UTF8_4B.."
p UTF8_4B(128 + 64 + 32 + 16) != 0
p UTF8_4B(128 | 64 | 32 | 16) != 0
p UTF8_4B(128 | 64 | 32 | 16 | 7) != 0
p UTF8_4B(128 | 64 | 32 | 16 | 7) > 0
p UTF8_4B(128 + 64 + 32 + 16 + 8) == 0
p UTF8_4B(128 | 64 | 32 | 16 | 8) == 0
#
q
