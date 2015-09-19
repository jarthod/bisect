all: bisect

bisect: bisect.c
	clang bisect.c -o bisect

clean:
	rm -f bisect

re: clean all

test: all
	test/run.sh