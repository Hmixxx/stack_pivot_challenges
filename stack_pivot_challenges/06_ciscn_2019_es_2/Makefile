CC=gcc
CFLAGS=-m32 -fno-stack-protector -no-pie -z execstack
TARGET=ciscn_2019_es_2

all: $(TARGET)

$(TARGET): ciscn_2019_es_2.c
	$(CC) $(CFLAGS) -o $(TARGET) ciscn_2019_es_2.c

clean:
	rm -f $(TARGET)
