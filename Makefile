CXX = clang++
CXXFLAGS = -std=c++20 -g -I. # -Weverything

SRCS = server.cpp client.cpp 
LIBS = message.cpp httpmessage.cpp simplesocket.cpp  serversocket.h clientsocket.h  httpmessage.h 

all: client server

client: client.cpp $(LIBS)
	$(CXX) $(CXXFLAGS) client.cpp message.cpp simplesocket.cpp -o client

server: server.cpp $(LIBS)
	$(CXX) $(CXXFLAGS) server.cpp message.cpp simplesocket.cpp -o server -lpthread

clean:
	rm -f server client


