CC = clang
CFLAGS = -Wall -Werror -Wpedantic -Wextra
LFLAGS = -lm

all: banhammer

banhammer: banhammer.o bf.o bv.o speck.o parser.o node.o ht.o bst.o
	$(CC) -o banhammer banhammer.o bf.o bv.o speck.o parser.o node.o ht.o bst.o $(LFLAGS)

banhammer.o: banhammer.c
	$(CC) $(CFLAGS) -c banhammer.c

bf.o: bf.c
	$(CC) $(CFLAGS) -c bf.c

node.o: node.c
	$(CC) $(CFLAGS) -c node.c

bv.o: bv.c
	$(CC) $(CFLAGS) -c bv.c

speck.o: speck.c
	$(CC) $(CFLAGS) -c speck.c

parser.o: parser.c
	$(CC) $(CFLAGS) -c parser.c

ht.o: ht.c
	$(CC) $(CFLAGS) -c ht.c

bst.o: bst.c
	$(CC) $(CFLAGS) -c bst.c

format:
	clang-format -i -style=file *.c *.h
clean:
	rm -f banhammer *.o

