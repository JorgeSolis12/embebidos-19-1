PROYECTO=muestreo2
DESTDIR=/usr/local/bin
CFLAGS=-Wall
LDFLAGS=-lm
CC=gcc

all: $(PROYECTO) 

OBJ = $(wildcard *.o)

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
