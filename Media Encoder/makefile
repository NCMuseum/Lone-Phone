OBJS	= PNGEncoder.o upng.o
SOURCE	= PNGEncoder.cpp upng.cpp
HEADER	= upng.h osc.h
OUT	= PNGEncoder
FLAGS	= -fpermissive
CC	= g++

PNGEncoder: $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS)

%.o: %.c $(HEADER)
	$(CC) $(FLAGS) -c -o $@ $< $(LFLAGS)

clean:
	rm -f $(OBJS) $(OUT)