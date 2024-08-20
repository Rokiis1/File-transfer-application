#!/bin/bash

if [ -d logs ]; then
    rm -rf logs
fi

if [ -d build ]; then
    rm -rf build
fi

mkdir -p logs
mkdir -p build

cd build

# Compile the server
gcc -o server ../main.c ../src/connection_handler.c ../src/file_transfer.c ../src/logger.c ../src/client_handler.c -I../include -lws2_32

if [ $? -eq 0 ]; then
    echo "Server compilation successful."
else
    echo "Server compilation failed."
    exit 1
fi

# Compile the client
gcc -o client ../client_main.c ../src/connection_handler.c ../src/file_transfer.c ../src/logger.c ../src/client_handler.c -I../include -lws2_32

if [ $? -eq 0 ]; then
    echo "Client compilation successful."
else
    echo "Client compilation failed."
    exit 1
fi

echo "Compilation successful. You can now run the server and client separately."
echo "To run the server: ./run_server.sh"
echo "To run the client: ./run_client.sh <server-ip> <file-to-transfer>"