
all: main 
build: NWC_math.o BitOperate.o NWC.o

CC = g++
INSTDIR = /usr/local/bin
INCLUDE = .
CFLAGS = -g -std=c++11 -Wall -ansi
LIBS += -framework CoreFoundation

main: main.o main_AE.o main_R16_BU.o main_barrett.o \
		test_file.o test_file_in_place.o check_in_place_NWC.o build		
	$(CC) -o main_R16_BU.exe main_R16_BU.o NWC_math.o BitOperate.o NWC.o -lntl -lgmp -lm
	$(CC) -o main_barrett.exe main_barrett.o NWC_math.o BitOperate.o NWC.o -lntl -lgmp -lm
	$(CC) -o test_file_in_place.exe test_file_in_place.o NWC_math.o BitOperate.o NWC.o -lntl -lgmp -lm

AE: main_AE.o
	$(CC) -o main_AE.exe main_AE.o NWC_math.o BitOperate.o NWC.o -lntl -lgmp -lm
	./main_AE.exe

test_in_place: test_file_in_place.o
	$(CC) -o test_file_in_place.exe test_file_in_place.o NWC_math.o BitOperate.o NWC.o -lntl -lgmp -lm
	./test_file_in_place.exe

barrett: main_barrett.o
	$(CC) -o main_barrett.exe main_barrett.o NWC_math.o BitOperate.o NWC.o -lntl -lgmp -lm
	./main_barrett.exe

R16_tb: main_R16_BU.o
	$(CC) -o main_R16_BU.exe main_R16_BU.o NWC_math.o BitOperate.o NWC.o -lntl -lgmp -lm
	./main_R16_BU.exe

AGU_tb: main_test_AGU.o
	$(CC) -o main_test_AGU.exe main_test_AGU.o NWC_math.o BitOperate.o NWC.o -lntl -lgmp -lm
	./main_test_AGU.exe

mem_tb: main_test_mem.o
	$(CC) -o main_test_mem.exe main_test_mem.o NWC_math.o BitOperate.o NWC.o -lntl -lgmp -lm
	./main_test_mem.exe

TF_tb: main_TF_gen_tb.o
	$(CC) -o main_TF_gen_tb.exe main_TF_gen_tb.o NWC_math.o BitOperate.o NWC.o -lntl -lgmp -lm
	./main_TF_gen_tb.exe

top_tb: main_top_tb.o
	$(CC) -o main_top_tb.exe main_top_tb.o NWC_math.o BitOperate.o NWC.o -lntl -lgmp -lm
	./main_top_tb.exe

check_mem_out: check_in_place_NWC.o
	$(CC) -o check_in_place_NWC.exe check_in_place_NWC.o NWC_math.o BitOperate.o NWC.o -lntl -lgmp -lm
	./check_in_place_NWC.exe

main.o: main.cpp
main_AE.o: main_AE.cpp
main_BU.o: main_BU.cpp
main_R16_BU.o: main_R16_BU.cpp
main_barrett.o: main_barrett.cpp
test_file.o: test_file.cpp
test_file_mem_AE.o: test_file_mem_AE.cpp
check_AE_NWC.o: check_AE_NWC.cpp
check_in_place_NWC.o: check_in_place_NWC.cpp
test_file_in_place.o: test_file_in_place.cpp
main_test_AGU.o: main_test_AGU.cpp
main_test_mem.o: main_test_mem.cpp
main_TF_gen_tb.o: main_TF_gen_tb.cpp
main_top_tb.o: main_top_tb.cpp

NWC_math.o: NWC_math.cpp NWC_math.h
BitOperate.o: BitOperate.cpp BitOperate.h
NWC.o: NWC.cpp NWC.h

clean:
	rm *.exe
	rm *.o
	rm *.txt

