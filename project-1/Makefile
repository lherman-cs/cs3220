CC=gcc

all: leakcount memory_shim.so

leakcount: leakcount.c
	$(CC) -o $@ -Wall $?

memory_shim.so: memory_shim.c
	$(CC) -Wall -fPIC -shared -o $@ $? -ldl

clean:
	rm leakcount memory_shim.so

archive: memory_shim.c leakcount.c Makefile README
	tar -zcf project1.tgz $?