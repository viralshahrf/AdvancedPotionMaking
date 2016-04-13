CC=g++
#CFLAGS+=-g
CFLAGS+=`pkg-config --cflags opencv`
LDFLAGS+=`pkg-config --libs opencv`

.PHONY: make 
make :
	g++ -std=c++11 `pkg-config --cflags opencv` -c roi.cpp `pkg-config --libs opencv`
	g++ -std=c++11 `pkg-config --cflags opencv` -c features.cpp `pkg-config --libs opencv`
	g++ -std=c++11 `pkg-config --cflags opencv` -c utilities.cpp `pkg-config --libs opencv`
	g++ -std=c++11 `pkg-config --cflags opencv` -c run.cpp `pkg-config --libs opencv`
	g++ -std=c++11 `pkg-config --cflags opencv` roi.o features.o utilities.o run.o -o run `pkg-config --libs opencv`
	gcc -c -fPIC middleware.cpp
	gcc -shared -o libmiddleware.so middleware.o

.PHONY: clean
clean :
	rm -f *.o
	rm -rf *.so
	rm -rf run


