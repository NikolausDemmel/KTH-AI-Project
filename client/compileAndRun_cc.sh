# requires (boost system library, not yet) and includes for parts of "random"

# The program will allocate around 600MB of RAM for a hashtable.

cd code
make run-opt PORT=7778 BOARD=1
