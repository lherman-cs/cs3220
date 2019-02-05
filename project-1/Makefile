CC=gcc

all: leakcount memory_shim.so sctracer

leakcount: leakcount.c
	$(CC) -o $@ -Wall $?

memory_shim.so: memory_shim.c
	$(CC) -Wall -fPIC -shared -o $@ $? -ldl

sctracer: sctracer.c
	$(CC) -Wall -o $@ $?

clean:
	rm leakcount memory_shim.so sctracer

archive: memory_shim.c leakcount.c sctracer.c Makefile README
	tar -zcf project1.tgz $?