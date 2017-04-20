CC=$(EC535)/gumstix/oe/cross/bin/arm-linux-gcc

i2c: i2c.o
	$(CC) -o i2c i2c.o

i2c.o: i2c.c
	$(CC) -c i2c.c
clean:
	rm i2c i2c.o
