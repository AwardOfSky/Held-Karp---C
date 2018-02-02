CC = gcc
CFLAGS = -Wall -pthread -Ofast -Wno-unused-result

OBJECTS = heldkarp.o helper.o main.o

heldkarp: $(OBJECTS)
	$(CC) $(CFLAGS) -o heldkarp $(OBJECTS)

helper.o: helper.h helper.c
heldkarp.o: heldkarp.h heldkarp.c helper.h
main.o: main.c helper.h heldkarp.h

clean:
	rm -f $(OBJECTS) heldkarp
