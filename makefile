CC=/usr/bin/g++
#CC=/usr/local/bin/g++-4.9

RAPIDJSON=-Isrc/rapidjson

SERVER_OUTNAME=server
SERVER_FLAGS=-std=c++11 -Wall -o $(SERVER_OUTNAME)
SERVER_PATH=src/server/*.cpp

all:
	$(CC) $(SERVER_FLAGS) $(SERVER_PATH) $(RAPIDJSON) -lpthread
clean:
	rm $(SERVER_OUTNAME) $(CLIENT_OUTNAME)
