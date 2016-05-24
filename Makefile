CC=g++
CFLAGS+=`pkg-config --cflags opencv`
LDFLAGS+=`pkg-config --libs opencv`

.PHONY: make 
make :
	g++ -std=c++11 `pkg-config --cflags opencv` -c -fPIC roi.cpp `pkg-config --libs opencv`
	g++ -std=c++11 `pkg-config --cflags opencv` -c -fPIC lbp.cpp `pkg-config --libs opencv`
	g++ -std=c++11 `pkg-config --cflags opencv` -c -fPIC features.cpp `pkg-config --libs opencv`
	g++ -std=c++11 `pkg-config --cflags opencv` -c -fPIC utilities.cpp `pkg-config --libs opencv`
	g++ -std=c++11 `pkg-config --cflags opencv` -c -I/usr/include/mysql -fPIC middleware.cpp `pkg-config --libs opencv`
	g++ -std=c++11 `pkg-config --cflags opencv` -c -fPIC normalize.cpp `pkg-config --libs opencv`
	g++ -std=c++11 `pkg-config --cflags opencv` -c -I/usr/include/mysql populateDatabase.cpp `pkg-config --libs opencv`
	g++ --std=c++11 `pkg-config --cflags opencv` -I/usr/include/mysql lbp.o roi.o utilities.o features.o populateDatabase.o -o populateDatabase -L/usr/lib/mysql -lmysqlclient `pkg-config --libs opencv`
	g++ -std=c++11 `pkg-config --cflags opencv` roi.o lbp.o features.o utilities.o middleware.o -shared -o libmiddleware.so `pkg-config --libs opencv` -L/usr/lib/mysql -lmysqlclient 
	g++ -std=c++11 `pkg-config --cflags opencv` -o normalize utilities.o normalize.o `pkg-config --libs opencv`

.PHONY: clean
clean :
	rm -f *.o
	rm -rf *.so
	rm -rf run


