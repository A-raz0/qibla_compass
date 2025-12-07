INCLUDE := -I ~/include
LDFLAGS := -L ~/lib -lsense -lm

all: qibla_compass

qibla_compass: main.o input.o output.o
	cc -o qibla_compass main.o input.o output.o $(LDFLAGS)

main.o: main.c project.h
	cc -c main.c $(INCLUDE)

input.o: input.c project.h
	cc -c input.c $(INCLUDE)

output.o: output.c project.h
	cc -c output.c $(INCLUDE)

clean:
	rm -f *.o qibla_compass

run: qibla_compass
	./qibla_compass
