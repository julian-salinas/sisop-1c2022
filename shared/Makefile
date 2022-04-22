PROCESS_NAME=shared
IDIR =./include
CC=gcc
CFLAGS=-I$(IDIR) -g -Wall

ODIR=./obj
SRCDIR =./src
LIBS=-lcommons -lpthread -lreadline -lrt

DEPS = $(wildcard $(IDIR)/*.h)

SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(patsubst $(SRCDIR)/%.c,$(ODIR)/%.o,$(SRC))

$(ODIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(PROCESS_NAME): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ $(PROCESS_NAME) 