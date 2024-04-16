TAR = gitlite
SRC = main.cpp src/*.cpp

$(TAR) : $(SRC)
	g++ $(SRC) -Isrc -o $(TAR) -std=c++17

clean:
	rm -rf .gitlite main
