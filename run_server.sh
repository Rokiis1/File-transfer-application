#!/bin/bash

cd build

if [ -f server ]; then
    echo "Starting server..."
    ./server &
    SERVER_PID=$!
    echo "Server is running with PID $SERVER_PID. Waiting for clients..."
    wait $SERVER_PID
else
    echo "Server executable not found. Please compile first using ./compile.sh."
fi