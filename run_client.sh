#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <server-ip> <file-to-transfer>"
    exit 1
fi

SERVER_IP=$1
FILE_TO_TRANSFER=$2

cd build

if [ -f client ]; then
    echo "Starting client..."
    ./client $SERVER_IP $FILE_TO_TRANSFER
else
    echo "Client executable not found. Please compile first using ./compile.sh."
fi