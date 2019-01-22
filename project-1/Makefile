CC=gcc

all: leakcount memory_shim.so

leakcount: leakcount.c
	$(CC) -o $@ -Wall $?

memory_shim.so: memory_shim.c
	$(CC) -Wall -fPIC -shared -o $@ $? -ldl

clean:
	rm leakcount memory_shim.so