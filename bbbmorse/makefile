SRC:= bbbmorse.c 

OBJ:= bbbmorse.o

INC:= ../include

GCC:= arm-linux-gnueabi-gcc

all:
	@echo 'Buiding bbbmorse'
	$(GCC) $(SRC) -I$(INC) -o bbbmorse

clean:	
	@echo 'Cleaning bbbmorse'
	rm -rf *.o
	rm bbbmorse
