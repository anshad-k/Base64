#!/bin/bash

set -e

if [ -f .env ]; then
    export $(grep -v '^#' .env | xargs)
else
    echo ".env file not found."
    exit 1
fi

if [ -z "$ACCESS_TOKEN" ]; then
    echo "Access token is not set. Check your .env file."
fi


DIR=$(pwd)
SRC=${DIR}/src
gcc ${SRC}/main.c ${SRC}/base64.c ${SRC}/utils.c -o ${DIR}/target/base64 -lcurl -Wall -Wextra -g -DPLAYGROUND &> ${DIR}/target/compile_logs.txt

${DIR}/target/base64
