all: client server

client: bin/client
server: bin/server

bin/client: client/main.cpp client/src/*.cpp client/lib/*.h | bin
	g++ client/*.cpp client/src/*.cpp -I client/lib -o bin/client

bin/server: server/main.cpp | bin
	g++ server/*.cpp -o bin/server

bin:
	mkdir bin

clean:
	rm -rf bin
