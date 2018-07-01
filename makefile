CC=g++
#LEDAID='/usr/include/LEDA'
LEDAINCL='/usr/include/LEDA'
#-L$(LEDAID)
CFLAGS= -I$(LEDAINCL) -O3 -lleda -lX11 -std=c++0x
TARGET=bdbfs


all:
	$(CC) main.cpp -o $(TARGET) $(CFLAGS) 
	
info:
	$(CC) main.cpp -o $(TARGET) $(CFLAGS) -D INFO=1

clean:
	rm bin/bdbfs

