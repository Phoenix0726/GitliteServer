SRC = src/*.cpp
SERVER_SRC = $(SRC) test/server.cpp
CLIENT_SRC = $(SRC) test/client.cpp
TEST_SRC = $(SRC) test/test.cpp
SERVER_OBJ = $(patsubst %.cpp, %.o, $(SERVER_SRC))
CLIENT_OBJ = $(patsubst %.cpp, %.o, $(CLIENT_SRC))
INCLUDE = -I./include
BIN = bin

all: server client

server: $(SERVER_OBJ)
	@mkdir -p $(BIN)
	g++ $(SERVER_OBJ) -o server

client: $(CLIENT_OBJ)
	g++ $(CLIENT_OBJ) -o client

$(SERVER_OBJ): $(SERVER_SRC)
	g++ -c $(SERVER_SRC) $(INCLUDE)

$(CLIENT_OBJ): $(CLIENT_SRC)
	g++ -c $(CLIENT_SRC) $(INCLUDE)

test: $(TEST_SRC)
	g++ $(TEST_SRC) $(INCLUDE) -o $(BIN)test


.PHONY: clean

clean:
	rm -f $(BIN)/*
