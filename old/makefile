
all: main 
build: NWC_math.o BitOperate.o NWC.o

CC = g++
INSTDIR = /usr/local/bin
INCLUDE = .
CFLAGS = -g -std=c++11 -Wall -ansi
LIBS += -framework CoreFoundation

main: main_32_2_16.o main_512_2_16_16.o main_8192_2_16_16_16.o build		
	
	$(CC) -o main_32_2_16.exe main_32_2_16.o NWC_math.o BitOperate.o NWC.o -lntl -lgmp -lm
	$(CC) -o main_512_2_16_16.exe main_512_2_16_16.o NWC_math.o BitOperate.o NWC.o -lntl -lgmp -lm
	$(CC) -o main_8192_2_16_16_16.exe main_8192_2_16_16_16.o NWC_math.o BitOperate.o NWC.o -lntl -lgmp -lm



main_32_2_16.o: main_32_2_16.cpp
main_512_2_16_16.o: main_512_2_16_16.cpp
main_8192_2_16_16_16.o: main_8192_2_16_16_16.cpp


NWC_math.o: NWC_math.cpp NWC_math.h
BitOperate.o: BitOperate.cpp BitOperate.h
NWC.o: NWC.cpp NWC.h

clean:
	rm *.exe
	rm *.o
	rm *.txt

