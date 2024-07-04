OBJS	= lcdScreenApp.o KS0108X3_RPI_Driver.o osc.o apa102LEDStrip.o colourObject.o VBitmap.o
SOURCE	= lcdScreenApp.cpp KS0108X3_RPI_Driver.cpp osc.cpp apa102LEDStrip.cpp colourObject.cpp VBitmap.cpp
HEADER	= KS0108X3_RPI_Driver.h osc.h apa102LEDStrip.h colourObject.h VBitmap.h
OUT	= lcdScreenApp
FLAGS	= -fpermissive -Wwrite-strings -Wint-to-pointer-cast
LFLAGS	= -pthread -lgpiod
CC	= g++

lcdScreenApp: $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS)

%.o: %.c $(HEADER)
	$(CC) $(FLAGS) -c -o $@ $< $(LFLAGS)

clean:
	rm -f $(OBJS) $(OUT)