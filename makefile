CC=gcc

default: run

main:
	$(CC) -w -g -no-pie -o run giaodien.c `pkg-config --libs gtk+-3.0` `pkg-config --cflags gtk+-3.0` libbt.a

run: main
	./run

clean:
	rm run