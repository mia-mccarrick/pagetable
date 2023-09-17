CC := clang
CDFLAGs := -Wall
LDFLAGS := -lm
.PHONY: all clean

all: pagetable pagetable.o

pagetable: libmlpt.a pagetable.o pagetable.c mlpt.h config.h
	$(CC) $(CFLAGS) -o pagetable pagetable.o libmlpt.a $(LDFLAGS)

pagetable.o: pagetable.c mlpt.h config.h
	$(CC) $(CLFLAGS) -c pagetable.c 

libmlpt.a: pagetable.o pagetable.c mlpt.h config.h
	ar rcs libmlpt.a pagetable.o

clean:
	rm -f *.o pagetable libmlpt.a
