TAR = gitlite
SRC = main.cpp src/*.cpp
WEBSERVER_SRC = ../WebServer/src/*.cpp

$(TAR) : $(SRC)
	g++ $(SRC) $(WEBSERVER_SRC) -Iinclude -I../WebServer/include -o $(TAR) -std=c++17

clean:
	rm -rf .gitlite main
