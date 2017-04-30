build:
	gcc test.c -Wall -o test

test:
	./test

clean:
	rm -f test

.PHONY: test
