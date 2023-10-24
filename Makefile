CFLAGS = -I ./include
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm #-lXrandr

all: pixeljump

pixeljump: pixeljump.cpp log.cpp dlopez2.cpp dlopez2.h nchiang.cpp nchiang.h aamistoso.cpp aamistoso.h usingh.cpp usingh.h
	g++ $(CFLAGS) pixeljump.cpp log.cpp dlopez2.cpp aamistoso.cpp usingh.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -opixeljump

clean:
	rm -f pixeljump
	rm -f *.o

