
SERVER=130.237.218.85
PORT=7778
# 7777 many boards
# 7778 trivial boards
# 7781 and 7782 have similar difficulty to the ones used in evaluation

BOARD=1
LASTBOARD=100
OPTIONS=
#OPTIONS=playsol 10
TIMEOUT=10 # timeout for testing



UNAME := $(shell uname -s)

CPP=g++-4.5
ifeq ($(UNAME),Darwin)
	CPP=g++-mp-4.5
endif

all: build

build: client

client: *.cc *.h
	$(CPP) --std=gnu++0x *.cc $(CXXFLAGS) -g -Wall -o client

run: client
	./client $(SERVER) $(PORT) $(BOARD) $(OPTIONS)

build-opt: client-opt

client-opt: *.cc *.h
	$(CPP) --std=gnu++0x *.cc $(CXXFLAGS) -O3 -DNDEBUG -o client-opt

run-opt: client-opt
	./client-opt $(SERVER) $(PORT) $(BOARD) $(OPTIONS)

test: client-opt
	rm -f tmp_pipe1___
	mkfifo tmp_pipe1___
	(for i in `seq $(BOARD) $(LASTBOARD)` ; do echo ">>> $$i" 1>&2 && $(MAKE) \
			run-opt SERVER=$(SERVER) PORT=$(PORT) BOARD=$$i OPTIONS=$(TIMEOUT) \
			| tee -a test-boards.log | grep "CORRECT\|timeout" ; done && \
			echo "Number of boards solved in under $$TIMEOUT seconds:" ) 2>&1 \
			| tee tmp_pipe1___ | grep "CORRECT" | wc -l & \
	cat < tmp_pipe1___
	rm -f tmp_pipe1___

clean:
	rm -f client client-opt tmp_pipe1___
