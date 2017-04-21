CC=$(EC535)/gumstix/oe/cross/bin/arm-linux-gcc

gumstix_master: gumstix_master.o
	$(CC) -o gumstix_master gumstix_master.o -lbluetooth

gumstix_master.o: gumstix_master.c
	$(CC) -c gumstix_master.c 

clean:
	rm gumstix_master gumstix_master.o

