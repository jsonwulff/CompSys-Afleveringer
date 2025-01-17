#!/usr/bin/env bash

# Exit immediately if any command below fails.
set -e

make


echo "Generating a test_files directory.."
mkdir -p test_files
rm -f test_files/*


echo "Generating test files.."

### Empty file type test ###
printf "" > test_files/empty.input

### ASCII ###
# Alle tegn
printf "\x07 \x08 \x09 \x0A \x0B \x0C \x0D \x1B \x20 \x21 \x22 \x23 \x24 \x25 \x26 \x27 \x28 \x29 \x2A \x2B \x2C \x2D \x2E \x2F \x30 \x31 \x32 \x33 \x34 \x35 \x36 \x37 \x38 \x39 \x3A \x3B \x3C \x3D \x3E \x3F \x40 \x41 \x42 \x43 \x44 \x45 \x46 \x47 \x48 \x49 \x4A \x4B \x4C \x4D \x4E \x4F \x50 \x51 \x52 \x53 \x54 \x55 \x56 \x57 \x58 \x59 \x5A \x5B \x5C \x5D \x5E \x5F \x60 \x61 \x62 \x63 \x64 \x65 \x66 \x67 \x68 \x69 \x6A \x6B \x6C \x6D \x6E \x6F \x70 \x71 \x72 \x73 \x74 \x75 \x76 \x77 \x78 \x79 \x7A \x7B \x7C \x7D \x7E " > test_files/ascii_allchars1.input
printf "\x07 \x08 \x09 \x0A \x0B \x0C \x0D \x1B \x20 \x21 \x22 \x23 \x24 \x25 \x26 \x27 \x28 \x29 \x2A \x2B \x2C \x2D \x2E \x2F \x30 \x31 \x32 \x33 \x34 \x35 \x36 \x37 \x38 \x39 \x3A \x3B \x3C \x3D \x3E \x3F \x40 \x41 \x42 \x43 \x44 \x45 \x46 \x47 \x48 \x49 \x4A \x4B \x4C \x4D \x4E \x4F \x50 \x51 \x52 \x53 \x54 \x55 \x56 \x57 \x58 \x59 \x5A \x5B \x5C \x5D \x5E \x5F \x60 \x61 \x62 \x63 \x64 \x65 \x66 \x67 \x68 \x69 \x6A \x6B \x6C \x6D \x6E \x6F \x70 \x71 \x72 \x73 \x74 \x75 \x76 \x77 \x78 \x79 \x7A \x7B \x7C \x7D \x7E \n" > test_files/ascii_allchars2.input
# Stor fil
printf "Hello, WorldAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA!" > test_files/asciiLarge1.input
printf "Hello, WorldAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA!" > test_files/asciiLarge2.input
# Lille fil
printf "Hello, World!" > test_files/ascii.input
printf "Hello, World!\n" > test_files/ascii2.input
# Fil med masse tal / hex
printf "0x129480 0x1303876 0x230876 0x132984760x129480 0x1303876 0x230876 0x132984760x129480 0x1303876 0x230876 0x132984760x129480 0x1303876 0x230876 0x132984760x129480 0x1303876 0x230876 0x132984760x129480 0x1303876 0x230876 0x13298476" > test_files/asciihex1.input
printf "0x129480 0x1303876 0x230876 0x132984760x129480 0x1303876 0x230876 0x132984760x129480 0x1303876 0x230876 0x132984760x129480 0x1303876 0x230876 0x132984760x129480 0x1303876 0x230876 0x132984760x129480 0x1303876 0x230876 0x13298476 \n" > test_files/asciihex2.input
# Et enkelt char
# printf "H" > test_files/ascii1.input  // commented out cause of "very short file (no magic)"
printf "H \n" > test_files/ascii2.input

### DATA ###
#Blandet ASCII, data
printf "\x001 \x002 \x003 \x004 \x000006 \x0007 \x0002 \x00007 \x006 AAAAAAAAAAAAAAAAAA \x0054 \x0234 \x0A \x01 \x03 \x088 \x0888 \x0999 \x02222 \x0344 \x05543 \x03232 \x0234 \x0654 \x0235 \x027 \x0257" > test_files/datamanychar1.input
printf "\x001 \x002 \x003 \x004 \x000006 \x0007 \x0002 \x00007 \x006 AAAAAAAAAAAAAAAAAA \x0054 \x0234 \x0A \x01 \x03 \x088 \x0888 \x0999 \x02222 \x0344 \x05543 \x03232 \x0234 \x0654 \x0235 \x027 \x0257 \n" > test_files/datamanychar2.input
# Stor fil
printf "\x001 \x002 \x003 \x004 \x000006 \x0007 \x0002 \x00007 \x006 AAAAAAAAAAAAAAAAAA \x0054 \x0234 \x0A \x01 \x03 \x088 \x0888 \x0999 \x02222 \x0344 \x05543 \x03232 \x0234 \x0654 \x0235 \x027 \x0257\x001 \x002 \x003 \x004 \x000006 \x0007 \x0002 \x00007 \x006 AAAAAAAAAAAAAAAAAA \x0054 \x0234 \x0A \x01 \x03 \x088 \x0888 \x0999 \x02222 \x0344 \x05543 \x03232 \x0234 \x0654 \x0235 \x027 \x0257\x001 \x002 \x003 \x004 \x000006 \x0007 \x0002 \x00007 \x006 AAAAAAAAAAAAAAAAAA \x0054 \x0234 \x0A \x01 \x03 \x088 \x0888 \x0999 \x02222 \x0344 \x05543 \x03232 \x0234 \x0654 \x0235 \x027 \x0257\x001 \x002 \x003 \x004 \x000006 \x0007 \x0002 \x00007 \x006 AAAAAAAAAAAAAAAAAA \x0054 \x0234 \x0A \x01 \x03 \x088 \x0888 \x0999 \x02222 \x0344 \x05543 \x03232 \x0234 \x0654 \x0235 \x027 \x0257" > test_files/datalarge1.input
printf "\x001 \x002 \x003 \x004 \x000006 \x0007 \x0002 \x00007 \x006 AAAAAAAAAAAAAAAAAA \x0054 \x0234 \x0A \x01 \x03 \x088 \x0888 \x0999 \x02222 \x0344 \x05543 \x03232 \x0234 \x0654 \x0235 \x027 \x0257\x001 \x002 \x003 \x004 \x000006 \x0007 \x0002 \x00007 \x006 AAAAAAAAAAAAAAAAAA \x0054 \x0234 \x0A \x01 \x03 \x088 \x0888 \x0999 \x02222 \x0344 \x05543 \x03232 \x0234 \x0654 \x0235 \x027 \x0257\x001 \x002 \x003 \x004 \x000006 \x0007 \x0002 \x00007 \x006 AAAAAAAAAAAAAAAAAA \x0054 \x0234 \x0A \x01 \x03 \x088 \x0888 \x0999 \x02222 \x0344 \x05543 \x03232 \x0234 \x0654 \x0235 \x027 \x0257\x001 \x002 \x003 \x004 \x000006 \x0007 \x0002 \x00007 \x006 AAAAAAAAAAAAAAAAAA \x0054 \x0234 \x0A \x01 \x03 \x088 \x0888 \x0999 \x02222 \x0344 \x05543 \x03232 \x0234 \x0654 \x0235 \x027 \x0257" > test_files/datalarge2.input
# Lille fil
printf "\x0 \x02 \x03 \x0 \x0 \x0A5 \x05B \x0533 \x12 \x583A \x1A" > test_files/datasmall1.input
printf "\x0 \x02 \x03 \x0 \x0 \x0A5 \x05B \x0533 \x12 \x583A \x1A" > test_files/datasmall2.input
# Lille fil mange ASCII
printf "Hello,\x00WorldAASFOIAJSFOAISHFOASFHASIDUHASD!" > test_files/data1.input
printf "Hello,\x00WorldAASFOIAJSFOAISHFOASFHASIDUHASD!\n" > test_files/data2.input
# NULL Bytes
printf "\x0 \x0 \x0 \x0 \x0 \x0 \x0 \x0 \x0 \x0 \x0" > test_files/null1.input
printf "\x0 \x0 \x0 \x0 \x0 \x0 \x0 \x0 \x0 \x0 \x0 \n" > test_files/null2.input
# Et enkelt data tegn
# printf "\xA1" > test_files/dataone1.input // commented out cause of "very short file (no magic)"
printf "\xA1 \n" > test_files/dataone2.input

### ISO-8859 ##
printf "\xe5\xe6\xf8\n" > test_files/ISOchar1.input
printf "\xe5\xe6\xf8\n" > test_files/ISOchar2.input

### UTF ##
printf "æøå\n" > test_files/UTF_8DKchar.input
# 1byte edges 00000111 & 01111111
printf "\x7\n" > test_files/UTF_8_1B_sm.input
printf "\x7E\n" > test_files/UTF_8_1B_lg.input
# 2byte edges 11000000 10000000 & 11000000 10111111
printf "\xC2\x80\n" > test_files/UTF_8_2B_sm_sm.input
printf "\xC2\xBF\n" > test_files/UTF_8_2B_sm_lg.input
printf "\xC2a\xBF\n" > test_files/UTF_8_2B_sm_err.input

printf "\xDF\x80\n" > test_files/UTF_8_2B_lg_sm.input
printf "\xDF\xBF\n" > test_files/UTF_8_2B_lg_lg.input
printf "\xDFa\xBF\n" > test_files/UTF_8_2B_lg_err.input
# 3byte edges 11101111 10000000 10000000 & 11011111 10111111
printf "\xe0\x80\x80\n" > test_files/UTF_8_3B_sm_sm.input
printf "\xe0\xBF\xBF\n" > test_files/UTF_8_3B_sm_lg.input
printf "\xe0\xBFa\xBF\n" > test_files/UTF_8_3B_sm_err.input

printf "\xef\x80\x80\n" > test_files/UTF_8_3B_lg_sm.input
printf "\xef\xBF\xBF\n" > test_files/UTF_8_3B_sm_lg.input
printf "\xef\xBFa\xBF\n" > test_files/UTF_8_3B_sm_err.input
#4byte edges 11110111 & 11110000
printf "\xf0\x80\x80\x80\n" > test_files/UTF_8_4B_sg_sm.input
printf "\xf0\xBF\xBF\xBF\n" > test_files/UTF_8_4B_sm_lg.input
printf "\xf0\xBF\xBFa\xBF\n" > test_files/UTF_8_4B_sm_err.input

printf "\xf7\x80\x80\x80\n" > test_files/UTF_8_4B_lg_sm.input
printf "\xf7\xBF\xBF\xBF\n" > test_files/UTF_8_4B_lg_lg.input
printf "\xf7\xBF\xBFa\xBF\n" > test_files/UTF_8_4B_lg_err.input


### Little-endian UTF-16 Unicode text ###
printf "\xFF\xFE\x0a\x00\x61\x00" > test_files/UTF_16_LE1.input
printf "\xFF\xFE\x0a\x00\xbb\xef\x68\xbf\x6c\x61\x6f\x6c\xc3\x20\xc3\xa6\x20\xb8\xa5\xc3\x77\x20\x72\x6f\x64\x6c" > test_files/UTF_16_LE2.input

### Big-endian UTF-16 Unicode text ###
printf "\xfe\xff\x68\x0a\x00\x0a" > test_files/UTF_16_BE1.input
printf "\xfe\xff\x68\x0a\x6c\x61\x6f\x6c\x77\x20\x72\x6f\x64\x6c\x00\x0a" > test_files/UTF_16_BE2.input


echo "Running the tests.."
exitcode=0
f=test_files/*.input
echo ">>> Testing ${f}.."
file    volapyk.input ${f} | sed 's/ASCII text.*/ASCII text/' > test_files/expected
./file  volapyk.input ${f} > test_files/actual


if ! diff -u test_files/expected test_files/actual
then
  echo ">>> Failed :-("
  exitcode=1
else
  echo ">>> Success :-)"
fi

exit $exitcode
