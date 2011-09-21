
SERVER=130.237.218.85
PORT=7777
BOARD=1

all: build

build: client

client: *.cc *.h
	g++ *.cc $(CXXFLAGS) -o client

run: client
	./client $(SERVER) $(PORT) $(BOARD)
	
build-opt: client-opt

client-opt: *.cc *.h
	g++ *.cc $(CXXFLAGS) -O2 -o client-opt

run-opt: client-opt
	./client-opt $(SERVER) $(PORT) $(BOARD)
	
clean:
	rm client client-opt