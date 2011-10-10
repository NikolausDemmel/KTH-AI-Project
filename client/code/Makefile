
SERVER=130.237.218.85
PORT=7777
# 7778 trivial boards
# 7781 and 7782 have similar difficulty to the ones used in evaluation

BOARD=1

all: build

build: client

client: *.cc *.h
	g++ *.cc $(CXXFLAGS) -o client

run: client
	./client $(SERVER) $(PORT) $(BOARD)

build-opt: client-opt

client-opt: *.cc *.h
	g++ *.cc $(CXXFLAGS) -O3 -DNDEBUG -o client-opt

run-opt: client-opt
	./client-opt $(SERVER) $(PORT) $(BOARD)

clean:
	rm client client-opt