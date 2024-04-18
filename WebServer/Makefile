SRC = $(wildcard src/*.cpp)
SERVER_SRC = $(SRC) test/server.cpp ../gitlite/src/Utils.cpp
CLIENT_SRC = $(SRC) test/client.cpp
TEST_SRC = $(SRC) test/test.cpp
SERVER_OBJ = $(patsubst %.cpp, %.o, $(SERVER_SRC))
CLIENT_OBJ = $(patsubst %.cpp, %.o, $(CLIENT_SRC))
INCLUDE = -I./include -I../gitlite/include
BIN = bin


all: server client

server: $(SERVER_OBJ)
	@mkdir -p $(BIN)
	g++ $(SERVER_OBJ) -o $(BIN)/server

client: $(CLIENT_OBJ)
	g++ $(CLIENT_OBJ) -o $(BIN)/client

%.o: %.cpp
	g++ $(INCLUDE) -c $< -o $@


test: $(TEST_SRC)
	g++ $(TEST_SRC) $(INCLUDE) -o $(BIN)test


.PHONY: clean

clean:
	rm -f $(BIN)/* $(SERVER_OBJ) $(CLIENT_OBJ)
