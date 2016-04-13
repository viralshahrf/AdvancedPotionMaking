CC=g++
#CFLAGS+=-g
CFLAGS+=`pkg-config --cflags opencv`
LDFLAGS+=`pkg-config --libs opencv`

.PHONY: make 
make :
	g++ -std=c++11 `pkg-config --cflags opencv` -c -fPIC roi.cpp `pkg-config --libs opencv`
	g++ -std=c++11 `pkg-config --cflags opencv` -c -fPIC lbp.cpp `pkg-config --libs opencv`
	g++ -std=c++11 `pkg-config --cflags opencv` -c -fPIC features.cpp `pkg-config --libs opencv`
	g++ -std=c++11 `pkg-config --cflags opencv` -c -fPIC utilities.cpp `pkg-config --libs opencv`
	g++ -std=c++11 `pkg-config --cflags opencv` -c run.cpp `pkg-config --libs opencv`
	g++ -std=c++11 `pkg-config --cflags opencv` roi.o lbp.o features.o utilities.o run.o -o run `pkg-config --libs opencv`
	g++ -std=c++11 `pkg-config --cflags opencv` roi.o lbp.o features.o utilities.o -c -fPIC middleware.cpp `pkg-config --libs opencv`
	g++ -shared -o libmiddleware.so middleware.o

.PHONY: clean
clean :
	rm -f *.o
	rm -rf *.so
	rm -rf run


