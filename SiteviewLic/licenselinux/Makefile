.SUFFIXES: .o .c 
CC=gcc




TARGET_LIB = license.so sysid lictest

ALL: $(TARGET_LIB)

.c.o:
	$(CC) -c $<


license.so:  mac.o base64.o commons.o liclib.o sysinfo.o des.o cpuid.o md5.o
	$(CC) -o $@ $^  -fpic -shared  -lcrypto	
	
sysid:  sysid.o  license.so
	$(CC) -o $@ $^ 
	
lictest:  lictest.o  license.so
	$(CC) -o $@ $^ 
	rm -f *.o 	
clean:
	rm -f *.o *.so
