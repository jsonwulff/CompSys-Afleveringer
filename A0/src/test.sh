#!/usr/bin/env bash

# Exit immediately if any command below fails.
set -e

make


echo "Generating a test_files directory.."
mkdir -p test_files
rm -f test_files/*


echo "Generating test files.."
printf "Hello, World!\n" > test_files/ascii.input
printf "Hello, World!" > test_files/ascii2.input
printf "Hello,\x00World!\n" > test_files/data.input
printf "" > test_files/empty.input
### TODO: Generate more test files ###
printf "hemmelighed" > secret.input
chmod -r secret.input
# Create tests for number of arguments on program
# Create tests files that dosen't exist
# Create tests for "hemmelig_fil" / Permission denied

### ASCII
# Alle tegn
# Stor fil
# Lille fil
# Fil med masse tal / hex
# Et enkelt char


### DATA
# Mange tegn
# Stor fil
# Lille fil
# NULL Bytes
# Et enkelt data tegn


echo "Running the tests.."
exitcode=0
for f in test_files/*.input
do
  echo ">>> Testing ${f}.."
  file    "${f}" | sed 's/ASCII text.*/ASCII text/' > "${f}.expected"
  ./file  "${f}" > "${f}.actual"

  if ! diff -u "${f}.expected" "${f}.actual"
  then
    echo ">>> Failed :-("
    exitcode=1
  else
    echo ">>> Success :-)"
  fi
done

chmod +r secret.input
exit $exitcode
