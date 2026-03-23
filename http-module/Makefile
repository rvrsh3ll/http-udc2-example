CC=i686-w64-mingw32-gcc
CC_64=x86_64-w64-mingw32-gcc
CFLAGS=-O1 -fno-jump-tables -shared -Wall -Wno-pointer-arith

.PHONY: all x86 x64 clean

all: x86 x64

bin:
	mkdir -p bin

x86: bin
	$(CC) -DWIN_X86 $(CFLAGS) -c src/http.c -o bin/http.x86.o
	$(CC) -DWIN_X86 $(CFLAGS) -c src/udc2.c -o bin/udc2.x86.o
	zip -q -j http-udc2.x86.zip bin/*.x86.o

x64: bin
	$(CC_64) -DWIN_X64 $(CFLAGS) -c src/http.c -o bin/http.x64.o
	$(CC_64) -DWIN_X64 $(CFLAGS) -c src/udc2.c -o bin/udc2.x64.o
	zip -q -j http-udc2.x64.zip bin/*.x64.o

clean:
	rm -rf bin