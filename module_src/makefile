CC=g++
CFLAGS=-c -Wall -D_LINUX -w -ISDK/ -I. -I../../../Shared -I../../../Server/Core -I../../../Vendor -I../../../Vendor/Squirrel -I/usr/include/mysql -lmysqlclient -lboost_system -lboost_date_time -lboost_chrono -lboost_thread
SOURCES=$(wildcard *.cpp)
SOURCES+=$(wildcard SDK/*.cpp) $(wildcard ../../../Vendor/Squirrel/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=ivmp-mysql.so

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	g++ $(OBJECTS) -lpthread -ldl -shared -o $@ 

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -Rf *.o $(EXECUTABLE)
