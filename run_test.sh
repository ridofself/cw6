#!/bin/bash
gcc ./src/test.c -o run -std=c89 -Wall -Wextra -Werror -Wpedantic
./run
rm run
