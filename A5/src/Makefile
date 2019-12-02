CC=gcc
CFLAGS=-g -Wall -Wextra -pedantic -std=gnu99 -pthread
EXAMPLES=fibs fauxgrep fauxgrep-mt fhistogram fhistogram-mt

.PHONY: all test clean ../src.zip

all: $(TESTS) $(EXAMPLES)

job_queue.o: job_queue.c job_queue.h
	$(CC) -c job_queue.c $(CFLAGS)

%: %.c job_queue.o
	$(CC) -o $@ $^ $(CFLAGS)

testfiles:
	rm -rf testfiles
	mkdir testfiles
	dd bs=1024 count=1024 </dev/urandom >./testfiles/myfile1
	dd bs=1024 count=1024 </dev/urandom >./testfiles/myfile2
	dd bs=1024 count=4096 </dev/urandom >./testfiles/myfile3
	dd bs=1024 count=4096 </dev/urandom >./testfiles/myfile4
	dd bs=1024 count=8192 </dev/urandom >./testfiles/myfile5
	dd bs=1024 count=8192 </dev/urandom >./testfiles/myfile6
	dd bs=1024 count=16384 </dev/urandom >./testfiles/myfile7
	dd bs=1024 count=16384 </dev/urandom >./testfiles/myfile8
	dd bs=1024 count=32768 </dev/urandom >./testfiles/myfile9
	dd bs=1024 count=32768 </dev/urandom >./testfiles/myfile10
	dd bs=1024 count=32768 </dev/urandom >./testfiles/myfile11

test: $(TESTS)
	@set e; for test in $(TESTS); do echo ./$$test; ./$$test; done

clean:
	rm -rf $(TESTS) $(EXAMPLES) *.o core testfiles *.dSYM

../src.zip:
	make clean
	cd .. && zip src.zip -r src
