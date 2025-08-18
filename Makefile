# Out
OUT = -o ./out/x

# Flags
# -lsqlite3 for lsqlite3
# -lssl -lcrypto for openssl/sha256
# -Wall = Warn ALl
FLAGS = -Wall

# C++ standard version
CPPSTDV = -std=c++2a

# File(s)
FILES = ./source/main.cpp

# Default / Main
build_and_run_main: clear build_main run_main

# Builds executable
build_main:
	g++ $(FILES) $(OUT) $(CPPSTDV) $(FLAGS)

# Runs the outputed file
run_main: clear
	./out/x

# Build And Run Client
client: clear
	g++ ./source/client.cpp -o ./out/client $(CPPSTDV) $(FLAGS)
	./out/client

# Clears The Terminal
clear:
	clear

# Cleans Outs
clean:
	rm ./out/*
