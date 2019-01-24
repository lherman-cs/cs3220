CC=gcc

all: leakcount memory_shim.so

leakcount: leakcount.c
	$(CC) -o $@ -g -Wall $?

memory_shim.so: memory_shim.c
	$(CC) -Wall -fPIC -shared -o $@ $? -ldl

clean:
	rm leakcount memory_shim.so

memory_shim.dylib: memory_shim.c
	$(CC) -Wall -fPIC -shared -o $@ $? -ldl

memory_shim_test:
	DYLD_INSERT_LIBRARIES=$(PWD)/memory_shim.dylib DYLD_FORCE_FLAT_NAMESPACE=1 ./leakcount