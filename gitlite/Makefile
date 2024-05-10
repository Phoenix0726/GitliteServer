TAR = gitlite
GITLITE_SRC = main.cpp $(wildcard src/*.cpp)
WEBSERVER_SRC =  $(wildcard ../Server/src/*.cpp)
GITLITE_OBJ = $(patsubst %.cpp, %.o, $(GITLITE_SRC))
WEBSERVER_OBJ = $(patsubst %.cpp, %.o, $(WEBSERVER_SRC))
SRC = $(GITLITE_SRC) $(WEBSERVER_SRC)
OBJ = $(GITLITE_OBJ) $(WEBSERVER_OBJ)
INCLUDE = -I./include -I../Server/include
CFLAGS = -std=c++17

$(TAR): $(OBJ)
	g++ $(OBJ) -o $(TAR)

%.o: %.cpp
	g++ $(CFLAGS) $(INCLUDE) -c $< -o $@


.PHONY: clean

clean:
	rm -rf .gitlite gitlite $(GITLITE_OBJ)
