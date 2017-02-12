TARGET=iotop
LIB=libiotop.so
OBJS=main.o ioprio.o utils.o views.o xxxid_info.o
CFLAGS=-Wall -O2 --pedantic --std=c99 -fPIC
LDFLAGS=-lncurses

PREFIX=/usr

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)
	$(CC) -shared -o $(LIB) $^

%.o: src/%.c
	$(CC) -c $(CFLAGS) -o $@ $<


.PHONY: clean install

clean:
	rm -f $(OBJS) $(TARGET) $(LIB)

install:
	cp $(TARGET) $(PREFIX)/bin/$(TARGET)

uninstall:
	rm $(PREFIX)/bin/$(TARGET)

