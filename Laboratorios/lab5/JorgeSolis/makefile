PROYECTO= hilo
DESTDIR=/usr/local/bin
CFLAGS=-Wall
LDFLAGS=-lm -lpthread
CC=gcc
OBJ = $(wildcard *.o)

all: $(PROYECTO) 
%.o: %.c
	$(CC) -c $< $(CFLAGS)

$(PROYECTO): $(OBJ) 
	$(CC) $^ -o $@ $(PROYECTO) $(LDFLAGS)

.PHONY: clean

install: all 
	if [! -d $(DESTDIR)]; then \
		sudo mkdir /usr/local/bin; \
	fi;\
	sudo cp $(DESTDIR)/$(PROYECTO)

unistall:
	sudo rm $(DESTDIR)/$(PROYECTO)

clean:
	rm -f *.o $(PROYECTO)